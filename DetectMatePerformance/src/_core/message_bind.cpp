#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


#include "_type/message.h"
#include "_type/templates.h"
#include "_type/parsed.h"


namespace py = pybind11;


PYBIND11_MODULE(message_class, m) {
    py::class_<Messages>(m, "Messages")
        .def(py::init<std::deque<std::string>>())
        .def("get_next_message", &Messages::getNext)
        .def("size", &Messages::size)
        .def("shape", &Messages::shape);
    py::class_<Templates, Messages>(m, "Templates")
        .def(py::init<std::deque<std::string>>())
        .def("get_next_template", &Templates::getNext)
        .def("size", &Templates::size)
        .def("shape", &Templates::shape);
    py::class_<ParsedMessages>(m, "Parsed")
        .def(py::init<Templates*, int>())
        .def("get_next_parsed", &ParsedMessages::getNext)
        .def("set_next_parsed", &ParsedMessages::setNext)
        .def("size", &ParsedMessages::size)
        .def("shape", &ParsedMessages::shape);
}