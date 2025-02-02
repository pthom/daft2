cmake_minimum_required(VERSION 3.15...3.27)
project(litgen_template VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 20)

# Define Emscripten and WebGL2 flags
set(EMSCRIPTEN_COMPILE_FLAGS
  "-sUSE_SDL=2"
)
set(EMSCRIPTEN_LINK_FLAGS
  "-sUSE_SDL=2"
  "-sMAX_WEBGL_VERSION=2"
)


add_subdirectory(external/hello_imgui)


# Build C++ library
add_library(daft2 STATIC src/daft2/hello.cpp)
target_include_directories(daft2 PUBLIC src/daft2)
target_compile_options(daft2 PUBLIC ${EMSCRIPTEN_COMPILE_FLAGS} -Wall -Wextra -Werror)
target_link_options(daft2 PUBLIC ${EMSCRIPTEN_LINK_FLAGS})
# target_link_libraries(daft2 PUBLIC SDL2::SDL2)
target_link_libraries(daft2 PUBLIC hello_imgui)


# Build bindings
find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development.Module)
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_daft2 src/python_bindings/module.cpp)
target_link_libraries(_daft2 PRIVATE daft2)
install(TARGETS _daft2 DESTINATION daft2)
target_compile_options(_daft2 PUBLIC ${EMSCRIPTEN_COMPILE_FLAGS} -Wall -Wextra -Werror)
target_link_options(_daft2 PUBLIC ${EMSCRIPTEN_LINK_FLAGS})


# Important: SDL2 link notes
# ==========================
# SDL2 must be linked to this library. For whatever reason, this does not work with find_package.
# This has something to do with the fact that this is a SIDE library, with dynamic linking.
# Also, libsdl2.a is not in the default search path, so we need to specify the path to it.

# This will not work:
# find_package(SDL2 REQUIRED)
# target_link_libraries(_daft2 PUBLIC SDL2::SDL2)

# instead we link manually libSDL2.a:
set(sdl_lib_path ${EMSCRIPTEN_SYSROOT}/lib/wasm32-emscripten/lto/)
target_link_directories(_daft2 PUBLIC ${sdl_lib_path})
target_link_libraries(_daft2 PUBLIC SDL2)
