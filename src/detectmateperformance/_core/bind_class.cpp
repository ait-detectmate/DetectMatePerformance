#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <memory>
#include <utility>

#include <arrow/c/bridge.h>
#include <arrow/record_batch.h>
#include <arrow/table.h>

#include "parsers/aecid_pg.h"
#include "parsers/drain.h"
#include "parsers/auto_parser.h"
#include "parsers/brain.h"

#include "template_matcher/match_tree.h"
#include "_type/element.h"
#include "_type/templates.h"
#include "_type/parsed.h"


namespace py = pybind11;

namespace {

arrow::Status brainFromArrowStream(ArrowArrayStream* input, ArrowArrayStream* output) {
    ARROW_ASSIGN_OR_RAISE(auto reader, arrow::ImportRecordBatchReader(input));
    ARROW_ASSIGN_OR_RAISE(auto table, reader->ToTable());
    ARROW_ASSIGN_OR_RAISE(auto result, brain(*table));
    return arrow::ExportRecordBatchReader(
        std::make_shared<arrow::TableBatchReader>(std::move(result)), output
    );
}

py::object brainFromPython(py::handle table) {
    const auto export_stream = py::getattr(table, "__arrow_c_stream__", py::none());
    if (!PyCallable_Check(export_stream.ptr())) {
        throw py::type_error("brain() expects a table that exports an Arrow C stream");
    }

    const py::object capsule = export_stream();
    auto* stream = static_cast<ArrowArrayStream*>(
        PyCapsule_GetPointer(capsule.ptr(), "arrow_array_stream")
    );
    if (stream == nullptr) {
        throw py::error_already_set();
    }

    auto output = std::make_unique<ArrowArrayStream>();
    const py::capsule result_capsule(
        output.get(), "arrow_array_stream", [](void* pointer) {
            auto* output_stream = static_cast<ArrowArrayStream*>(pointer);
            if (output_stream->release != nullptr) {
                output_stream->release(output_stream);
            }
            delete output_stream;
        }
    );
    const auto status = brainFromArrowStream(stream, output.release());
    if (!status.ok()) {
        throw py::value_error(status.ToString());
    }

    const auto reader = py::module_::import("pyarrow")
        .attr("RecordBatchReader").attr("_import_from_c_capsule")(result_capsule);
    return reader.attr("read_all")();
}

}  // namespace


PYBIND11_MODULE(bind_class, m) {
    m.def("drain_generator", &drainGenerator);
    m.def("auto_parser", &doAutoParse);
    m.def("brain", &brainFromPython, py::arg("table"));

    constexpr AecidPgThresholds aecid_thresholds{};
    m.def(
        "aecid_pg_generator",
        &aecidPgGenerator,
        py::arg("logs"),
        py::arg("theta_1") = aecid_thresholds.theta_1,
        py::arg("theta_2") = aecid_thresholds.theta_2,
        py::arg("theta_3") = aecid_thresholds.theta_3
    );

    py::class_<Templates>(m, "Templates")
        .def(py::init<std::deque<std::string>>())
        .def("get_next_template", &Templates::getNext)
        .def("size", &Templates::size)
        .def("shape", &Templates::shape);

    py::class_<ParsedElement>(m, "ParsedElement")
        .def_readonly("event_id", &ParsedElement::event_id)
        .def_readonly("log_template", &ParsedElement::log_template)
        .def_readonly("variables", &ParsedElement::variables);

    py::class_<ParsedMessages>(m, "Parsed")
        .def(py::init<Templates*, int>())
        .def("get_elem_id", &ParsedMessages::getElemID)
        .def("get_elem", &ParsedMessages::getElem)
        .def("set_elem", &ParsedMessages::setElem)
        .def("get_elem_with_var", &ParsedMessages::getElemWithVar)
        .def("set_elem_with_var", &ParsedMessages::setElemWithVar)
        .def("get_all_ids", &ParsedMessages::getAllIDs)
        .def("get_all_elem", &ParsedMessages::getAllElemts)
        .def("get_all_var", &ParsedMessages::getAllVar)
        .def("size", &ParsedMessages::size)
        .def("shape", &ParsedMessages::shape);

    py::class_<MatchTree>(m, "MatchTree")
        .def(py::init<Templates*>())
        .def("match_string", &MatchTree::match_string)
        .def("match_string_with_var", &MatchTree::match_string_with_var)
        .def("match_batch", &MatchTree::match_batch)
        .def("match_batch_with_var", &MatchTree::match_batch_with_var);
}
