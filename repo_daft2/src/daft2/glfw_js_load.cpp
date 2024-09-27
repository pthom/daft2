// load_js_lib_emscripten_glfw3.cpp
#include <emscripten.h>
#include <stdio.h>

// Embed the JS library code as a raw string

extern const char* glfw_js_code;

extern "C" void init_load_js_lib_emscripten_glfw3() {
    static bool loaded = false;
    if(loaded) return;
    loaded = true;
    printf("Loading emscripten_glfw3.js\n");
    EM_ASM({
        eval(UTF8ToString($0));
    }, glfw_js_code);
    printf("Loaded emscripten_glfw3.js\n");
}





// Implementing emscripten_glfw3_context_get_now
extern "C" double emscripten_glfw3_context_get_now() {
    init_load_js_lib_emscripten_glfw3();
    return EM_ASM_DOUBLE({
        //return performance.now();
        return window.emscripten_glfw3_impl.emscripten_glfw3_context_get_now();
    });
}
