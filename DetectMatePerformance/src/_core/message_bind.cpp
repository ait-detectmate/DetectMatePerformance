#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


#include "_type/message.h"


namespace py = pybind11;


PYBIND11_MODULE(message_class, m) {
    py::class_<Messages>(m, "Messages")
        .def(py::init<std::deque<std::string>>())
        .def("get_next_message", &Messages::getNextMessage)
        .def("size", &Messages::size)
        .def("shape", &Messages::shape);
}