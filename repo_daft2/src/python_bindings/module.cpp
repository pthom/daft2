#include <pybind11/pybind11.h>

namespace py = pybind11;

void hello();


// This builds the native python module `_daft_lib`
// it will be wrapped in a standard python module `daft_lib`
PYBIND11_MODULE(_daft2, m)
{
    m.def("hello", hello);
}
