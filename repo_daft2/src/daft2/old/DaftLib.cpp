#include <SDL.h>
#include "DaftLib.h"
#include <string>
#include <stdexcept>
#include <iostream>

// WEBGGL2 or WEBGL1: it works with WebGL1 but not WebGL2...
#define USE_WEBGL2

int DummyHandleAppEvents(void *, SDL_Event *)
{
    return 0;
}

void My_SDL_GL_SetAttribute(SDL_GLattr attr, int value) {
    const char* attrName;
    switch (attr) {
        case SDL_GL_RED_SIZE: attrName = "SDL_GL_RED_SIZE"; break;
        case SDL_GL_GREEN_SIZE: attrName = "SDL_GL_GREEN_SIZE"; break;
        case SDL_GL_BLUE_SIZE: attrName = "SDL_GL_BLUE_SIZE"; break;
        case SDL_GL_ALPHA_SIZE: attrName = "SDL_GL_ALPHA_SIZE"; break;
        case SDL_GL_BUFFER_SIZE: attrName = "SDL_GL_BUFFER_SIZE"; break;
        case SDL_GL_ACCUM_RED_SIZE: attrName = "SDL_GL_ACCUM_RED_SIZE"; break;

        case SDL_GL_CONTEXT_FLAGS: attrName = "SDL_GL_CONTEXT_FLAGS"; break;
        case SDL_GL_CONTEXT_PROFILE_MASK: attrName = "SDL_GL_CONTEXT_PROFILE_MASK"; break;
        case SDL_GL_CONTEXT_MAJOR_VERSION: attrName = "SDL_GL_CONTEXT_MAJOR_VERSION"; break;
        case SDL_GL_CONTEXT_MINOR_VERSION: attrName = "SDL_GL_CONTEXT_MINOR_VERSION"; break;
        case SDL_GL_DEPTH_SIZE: attrName = "SDL_GL_DEPTH_SIZE"; break;
        case SDL_GL_STENCIL_SIZE: attrName = "SDL_GL_STENCIL_SIZE"; break;
        case SDL_GL_DOUBLEBUFFER: attrName = "SDL_GL_DOUBLEBUFFER"; break;
        //case : attrName = ""; break;
        // Add other cases as necessary
        default: attrName = "Unknown Attribute";
    }
    std::cerr << "About to set " << attrName << " to " << value << std::endl;
    if (SDL_GL_SetAttribute(attr, value) != 0) {
        std::cerr << "Failed to set " << attrName << " with value " << value 
                  << ": " << SDL_GetError() << std::endl;
    } else {
        std::cout << "My_SDL_GL_SetAttribute set " << attrName << " to " << value << " => OK" <<  std::endl;
    }
}

void dummy_sdl_call()
{
    // Impl_InitPlatformBackend
    auto flags = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER;
#ifdef __EMSCRIPTEN__
    flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
#endif
    printf("About to call SDL_init\n");
    return;
    if (SDL_Init(flags) != 0)
        fprintf(stderr, "Error: %s\n", SDL_GetError());

    //printf("About to call SDL_SetHint\n");
    //SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    //printf("After SDL_SetHint\n");

    //printf("About to call SDL_SetEventFilter\n");
    //SDL_SetEventFilter(DummyHandleAppEvents, nullptr);
    //printf("After SDL_SetEventFilter\n");

    // Impl_Select_Gl_Version
    My_SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    // unfortunately, we cannot use SDL_GL_CONTEXT_DEBUG_FLAG, as it leads to an error
    //    "context attributes are not supported"
    //My_SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    My_SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    #ifdef USE_WEBGL2
    // This currently will cause SDL_GL_CreateContext to fail:
    //    "could not create EGL context (call to eglCreateContext failed, reporting an error of EGL_BAD_CONFIG)"
    My_SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    My_SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #endif

    // My_SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    // My_SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // My_SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

    // Impl_CreateWindow
    int window_flags = 0;
    window_flags |= SDL_WINDOW_OPENGL;
    window_flags |= SDL_WINDOW_ALLOW_HIGHDPI;    
    auto window = SDL_CreateWindow("my window", 0, 0, 600, 400, window_flags);
    if (!window)
    {
        std::string msg = "SDL_CreateWindow returned NULL";
        printf("%s\n", msg.c_str());
        throw std::runtime_error(msg.c_str());
    }
    else
        printf("SDL_CreateWindow OK!\n");

    // Impl_CreateGlContext
    auto glContext = SDL_GL_CreateContext((SDL_Window *)window);
    if (glContext == nullptr)
    {
        std::string msg = std::string("Impl_CreateGlContext error: ") + SDL_GetError();
        printf("%s\n", msg.c_str());
        throw std::runtime_error(msg.c_str());
    }
    else
        printf("SDL_GL_CreateContext: OK!\n");
    SDL_GL_MakeCurrent((SDL_Window *)window, glContext); // KK No
    SDL_GL_SetSwapInterval(1);  // Enable vsync


    // The call to SQL_Quit succeeds
    printf("About to call SDL_Quit\n");
    SDL_Quit();
    printf("After SDL_Quit\n");
}