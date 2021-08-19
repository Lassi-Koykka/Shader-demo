
//My files
#include "include/Functions.h"

// std
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// opengl
#include <GL/glew.h>

// sdl
#include <SDL2/SDL.h>
// #include <SDL2/SDL_timer.h>
// #include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//Starts up SDL, creates window, and initializes OpenGL
int init();




//Shader loading utility programs
void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);

// ----- Create window
SDL_Window *window = NULL;

// ----- SDL OpenGL context
SDL_GLContext glContext;

//Render flag
bool gRenderQuad = true;

//Graphics program
GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

int init() {
    // ----- Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    // ----- Create window
    window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 2;
    }

    // ----- SDL OpenGL settings
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // ----- SDL OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL)
    {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return 3;
    }

    // ----- SDL v-sync
    if (SDL_GL_SetSwapInterval(1) < 0)
    {
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }

    // ----- GLEW
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    return 0;
}


int main(int argc, char *argv[])
{
    init();

    //Seed rand
    srand(time(NULL));

    // Colors
    float red = randomColor();
    float green = randomColor();
    float blue = randomColor();

    // For measuring fps
    unsigned int lastTime = SDL_GetTicks();
    int nbFrames = 0;

    // ----- Game loop
    bool quit = false;
    while (quit == false)
    {

        unsigned int currentTime = SDL_GetTicks();
        unsigned int delta = currentTime - lastTime;
        nbFrames++;

        // FPS counter
        if (delta >= 1000)
        { // If last prinf() was more than 1 sec ago
            // Set window counter and reset timer
            char *fpsCountText = (char *)malloc(50 * sizeof(char));
            sprintf(fpsCountText, "My Game   %d fps\n", nbFrames);
            SDL_SetWindowTitle(window, fpsCountText);
            nbFrames = 0;
            lastTime = currentTime;
        }

        SDL_Event windowEvent;
        while (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            else if (windowEvent.type == SDL_KEYDOWN)
            {
                if (windowEvent.key.keysym.sym == SDLK_SPACE)
                {
                    red = randomColor();
                    green = randomColor();
                    blue = randomColor();
                }
            }
        }

        /*
            do drawing here
        */

        // Fill with color
        glClearColor(red, green, blue, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        //Swap background to foreground or something I don't know...
        SDL_GL_SwapWindow(window);
    }

    printf("%s\n", glGetString(GL_VERSION));

    // ----- Clean up
    SDL_GL_DeleteContext(glContext);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}