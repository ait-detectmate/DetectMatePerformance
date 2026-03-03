#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


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
        .def("size", &ParsedMessages::size)
        .def("shape", &ParsedMessages::shape);
}