#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "template_matcher/match_tree.h"
#include "_type/templates.h"
#include "_type/parsed.h"


namespace py = pybind11;


PYBIND11_MODULE(message_class, m) {
    py::class_<Templates>(m, "Templates")
        .def(py::init<std::deque<std::string>>())
        .def("get_next_template", &Templates::getNext)
        .def("size", &Templates::size)
        .def("shape", &Templates::shape);
    py::class_<ParsedMessages>(m, "Parsed")
        .def(py::init<Templates*, int>())
        .def("get_elem", &ParsedMessages::getElem)
        .def("set_elem", &ParsedMessages::setElem)
        .def("get_elem_with_var", &ParsedMessages::getElemWithVar)
        .def("set_elem_with_var", &ParsedMessages::setElemWithVar)
        .def("size", &ParsedMessages::size)
        .def("shape", &ParsedMessages::shape);
    py::class_<MatchTree>(m, "MatchTree")
        .def(py::init<Templates*>())
        .def("match_string", &MatchTree::match_string)
        .def("match_string_with_var", &MatchTree::match_string_with_var)
        .def("match_batch", &MatchTree::match_batch)
        .def("match_batch_with_var", &MatchTree::match_batch_with_var);
}