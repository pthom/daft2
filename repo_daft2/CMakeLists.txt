cmake_minimum_required(VERSION 3.15...3.27)
project(litgen_template VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 20)

# Define Emscripten and WebGL2 flags
set(EMSCRIPTEN_FLAGS
  "-sUSE_SDL=2"
  "-sMIN_WEBGL_VERSION=2"
  "-sMAX_WEBGL_VERSION=2"
  "-sFULL_ES3=1"
  #"-sALLOW_MEMORY_GROWTH=1"
  #"-O2"
)

# Build C++ library
add_library(daft2 STATIC src/daft2/hello.cpp)
target_include_directories(daft2 PUBLIC src/daft2)
target_compile_options(daft2 PUBLIC ${EMSCRIPTEN_FLAGS})
target_link_options(daft2 PUBLIC ${EMSCRIPTEN_FLAGS})


# Build bindings
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_daft2 src/python_bindings/module.cpp)
target_link_libraries(_daft2 PRIVATE daft2)
install(TARGETS _daft2 DESTINATION daft2)
