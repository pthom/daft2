#include <iostream>
#include <SDL2/SDL.h>

int DummyHandleAppEvents(void *, SDL_Event *)
{
    return 0;
}


void hello()
{
    std::cout << "Hello, World!" << std::endl;

    // Impl_InitPlatformBackend
    auto flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
    printf("About to call SDL_init\n");
    if (SDL_Init(flags) != 0)
        fprintf(stderr, "Error: %s\n", SDL_GetError());

    printf("About to call SDL_SetHint\n");
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    printf("After SDL_SetHint\n");

    printf("About to call SDL_SetEventFilter\n");
    SDL_SetEventFilter(DummyHandleAppEvents, nullptr);
    printf("After SDL_SetEventFilter\n");


    // Impl_Select_Gl_Version
    printf("About to call SDL_GL_SetAttribute\n");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    // unfortunately, we cannot use SDL_GL_CONTEXT_DEBUG_FLAG, as it leads to an error
    //    "context attributes are not supported"
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    #ifdef USE_WEBGL2
    // This currently will cause SDL_GL_CreateContext to fail:
    //    "could not create EGL context (call to eglCreateContext failed, reporting an error of EGL_BAD_CONFIG)"
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #endif
    printf("after calls to SDL_GL_SetAttribute\n");

    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    // SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);


    // Impl_CreateWindow
    int window_flags = 0;
    window_flags |= SDL_WINDOW_OPENGL;
    window_flags |= SDL_WINDOW_ALLOW_HIGHDPI;    
    printf("About to call SDL_CreateWindow\n");
    auto window = SDL_CreateWindow("my window", 0, 0, 600, 400, window_flags);
    printf("After SDL_CreateWindow\n");
    if (!window)
    {
        std::string msg = "SDL_CreateWindow returned NULL";
        printf("%s\n", msg.c_str());
        throw std::runtime_error(msg.c_str());
    }
    else
        printf("SDL_CreateWindow OK!\n");


    // Impl_CreateGlContext
    printf("About to call SDL_GL_CreateContext\n");
    auto glContext = SDL_GL_CreateContext((SDL_Window *)window);
    printf("After SDL_GL_CreateContext\n");
    if (glContext == nullptr)
    {
        std::string msg = std::string("Impl_CreateGlContext error: ") + SDL_GetError();
        printf("%s\n", msg.c_str());
        throw std::runtime_error(msg.c_str());
    }
    else
        printf("SDL_GL_CreateContext: OK!\n");

    printf("About to call SDL_GL_MakeCurrent\n");
    SDL_GL_MakeCurrent((SDL_Window *)window, glContext); // KK No
    printf("About to call SDL_GL_SetSwapInterval\n");
    SDL_GL_SetSwapInterval(1);  // Enable vsync


    // The call to SQL_Quit succeeds
    printf("About to call SDL_Quit\n");
    SDL_Quit();
    printf("After SDL_Quit\n");

}
