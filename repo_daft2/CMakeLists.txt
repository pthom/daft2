cmake_minimum_required(VERSION 3.15...3.27)
project(litgen_template VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 20)

# Define Emscripten and WebGL2 flags
set(EMSCRIPTEN_FLAGS
  "-sUSE_SDL=2"
  "-sMIN_WEBGL_VERSION=2"
  "-sMAX_WEBGL_VERSION=2"
  #"-sFULL_ES3=1"
)

# Important: SDL2 link notes
# ==========================
# SDL2 must be linked to this library. For whatever reason, this does not work with find_package.
# This has something to do with the fact that this is a SIDE library, with dynamic linking.
# Anyhow, SDL2 is linked via flags in meta.yaml, see below:
# build:
# ldflags: |
#     -L$(WASM_LIBRARY_DIR)/lib
#     -lSDL2

#find_package(SDL2 REQUIRED)

# Add Hello ImGui
add_subdirectory(external/hello_imgui)


# Build C++ library
add_library(daft2 STATIC src/daft2/hello.cpp)
target_include_directories(daft2 PUBLIC src/daft2)
target_compile_options(daft2 PUBLIC ${EMSCRIPTEN_FLAGS})
target_link_options(daft2 PUBLIC ${EMSCRIPTEN_FLAGS})
# target_link_libraries(daft2 PUBLIC SDL2::SDL2)


# Build bindings
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_daft2 src/python_bindings/module.cpp)
target_link_libraries(_daft2 PRIVATE daft2)
install(TARGETS _daft2 DESTINATION daft2)
target_compile_options(_daft2 PUBLIC ${EMSCRIPTEN_FLAGS})
target_link_options(_daft2 PUBLIC ${EMSCRIPTEN_FLAGS})
# target_link_libraries(_daft2 PUBLIC SDL2::SDL2)
