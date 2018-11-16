#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "events.h"

int main(int carg, char *varg[])
{
    if (carg != 2)
    {
        fprintf(stderr, "view3d 1.2 (16-11-2018) by Vladimir Stupin\n"
                        "Program for viewing Quake models with using OpenGL.\n"
                        "Usage: view3d <model>\n"
                        "\t<model>\t- name of Quake MDL file.\n"
                        "Example: view3d quake/id1/pak0/progs/player.mdl\n"
                        "Hot keys:\n"
                        "\tModel:\n"
                        "\t\tz\t\t- switch to previous model frame,\n"
                        "\t\tx\t\t- switch to next model frame,\n"
                        "\t\tc\t\t- switch to previous model skin,\n"
                        "\t\tv\t\t- switch to next model skin,\n"
                        "\tRender modes:\n"
                        "\t\tl\t\t- on/off lighting (default - on),\n"
                        "\t\ts\t\t- on/off model skin (default - on),\n"
                        "\t\tn\t\t- on/off model vertex normals (default - off),\n"
                        "\tCamera:\n"
                        "\t\tq\t\t- move camera nearer,\n"
                        "\t\ta\t\t- move camera farther,\n"
                        "\t\tLeft arrow\t- rotate camera left,\n"
                        "\t\tRight arrow\t- rotate camera right,\n"
                        "\t\tUp arrow\t- rotate camera up,\n"
                        "\t\tDown arrow\t- rotate camera down.\n");
        return 1;
    }
    if (Init(varg[1]) != 0)
    {
        fprintf(stderr, "main: failed to load model\n");
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "main: failed to initialize SDL, %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) < 0)
    {
        fprintf(stderr, "main: failed to set red size, %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) < 0)
    {
        fprintf(stderr, "main: failed to set green size, %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) < 0)
    {
        fprintf(stderr, "main: failed to set blue size, %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
    {
        fprintf(stderr, "main: failed to enable double buffer, %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16) < 0)
    {
        fprintf(stderr, "main: failed to set depth buffer size, %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("View3D-SDL", SDL_WINDOWPOS_UNDEFINED,
                                                        SDL_WINDOWPOS_UNDEFINED,
                                                        800,
                                                        600,
                                                        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        fprintf(stderr, "main: failed to create window, %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    if (glcontext == NULL)
    {
        fprintf(stderr, "main: failed to create OpenGL context, %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (SDL_GL_MakeCurrent(window, glcontext) < 0)
    {
        fprintf(stderr, "main: failed to switch OpenGL context, %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int quit = 0;
    while (quit != 1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            Reshape(event.window.data1, event.window.data2);
                        break;
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = 1;
                            break;

                        case SDLK_q:
                            DistUp();
                            break;

                        case SDLK_a:
                            DistDown();
                            break;

                        case SDLK_z:
                            PrevFrame();
                            break;

                        case SDLK_x:
                            NextFrame();
                            break;

                        case SDLK_c:
                            PrevSkin();
                            break;

                        case SDLK_v:
                            NextSkin();
                            break;

                        case SDLK_l:
                            SwitchLighting();
                            break;

                        case SDLK_s:
                            SwitchDrawSkin();
                            break;

                        case SDLK_n:
                            SwitchDrawNormals();
                            break;

                        case SDLK_UP:
                            YawUp();
                            break;

                        case SDLK_DOWN:
                            YawDown();
                            break;

                        case SDLK_LEFT:
                            AngleLeft();
                            break;

                        case SDLK_RIGHT:
                            AngleRight();
                            break;
                    }
                    break;
            } 
        }

        Draw();

        /* Меняем буферы местами */
        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}
