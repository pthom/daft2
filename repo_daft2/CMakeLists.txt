cmake_minimum_required(VERSION 3.15...3.27)
project(litgen_template VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 20)


add_compile_options(--use-port=contrib.glfw3)
add_link_options(--use-port=contrib.glfw3 "-sLINKABLE")

# Compile SDL
# add_subdirectory(external/SDL)

# Build C++ library
add_library(daft2 STATIC src/daft2/hello.cpp)
target_include_directories(daft2 PUBLIC src/daft2)
if (EMSCRIPTEN)
    # target_compile_options(DaftLib PUBLIC "-sUSE_SDL=2")
    # target_link_options(DaftLib PUBLIC "-sUSE_SDL=2")
    # target_link_options(DaftLib PUBLIC "-sMAX_WEBGL_VERSION=2")
    # target_link_options(DaftLib PUBLIC "-sMIN_WEBGL_VERSION=2")    
endif()


# Build bindings
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_daft2 src/python_bindings/module.cpp)
target_link_libraries(_daft2 PRIVATE daft2)
install(TARGETS _daft2 DESTINATION daft2)
