cmake_minimum_required(VERSION 3.15...3.27)
project(litgen_template VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 20)


# Add emscripten-glfw
# This is a port of the main API of GLFW to Emscripten, using js
add_subdirectory(external/emscripten-glfw)  # will publish a target called glfw3

# Build C++ library
add_library(daft2 STATIC src/daft2/hello.cpp) # src/daft2/glfw_js_load.cpp src/daft2/glfw_js_rawstr.cpp)
target_include_directories(daft2 PUBLIC src/daft2)
target_link_options(daft2 PUBLIC "-sMAX_WEBGL_VERSION=2" "-sMIN_WEBGL_VERSION=2")
target_link_libraries(daft2 PUBLIC glfw3)
set(glfw_js /home/pascal/dvp/_Bundle/_Pyodide/pyodide/packages/daft2/repo_daft2/external/emscripten-glfw/src/js/lib_emscripten_glfw3.js)
# target_link_options(daft2 PUBLIC "--js-library ${glfw_js}")
target_link_options(daft2 PUBLIC "--post-js ${glfw_js}")

# Build bindings
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_daft2 src/python_bindings/module.cpp)
target_link_libraries(_daft2 PRIVATE daft2)
install(TARGETS _daft2 DESTINATION daft2)
# target_link_options(_daft2 PUBLIC "--js-library ${glfw_js}")
target_link_options(_daft2 PUBLIC "--post-js ${glfw_js}")
