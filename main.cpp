#include <windows.h>
#include <GL/glaux.h>
#include <stdio.h>
#include "events.h"

static void CALLBACK CALLBACK_Draw(void)
{
    Draw();
    auxSwapBuffers();
}

static void CALLBACK CALLBACK_Reshape(unsigned width, unsigned height)
{
    Reshape(width, height);
}

void main(int carg, char **varg)
{
    if (carg != 2)
    {
        fprintf(stdout, "view3d 2.0 beta 1 (05-07-2003) by Stupin W.A.\n"
                        "Program for viewing Quake 2 models with using OpenGL.\n"
                        "Usage: view3d <model>\n"
                        "\t<model>\t- name of Quake MDL file.\n"
                        "Example: view3d quake2\\baseq2\\pak0\\models\\player.md2\n"
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
        return;
    }
    if (Init(varg[1]) != 0)
    {
        fprintf(stderr, "main: failed to load model\n");
        return 1;
    }

    auxInitPosition(0, 0, 800, 600);
    auxInitDisplayMode(AUX_RGB | AUX_DOUBLE);
    if (auxInitWindow("view3d") == GL_FALSE)
    {
        auxQuit();
    }

    auxKeyFunc(AUX_q, (AUXKEYPROC)DistUp);
    auxKeyFunc(AUX_a, (AUXKEYPROC)DistDown);
    auxKeyFunc(AUX_z, (AUXKEYPROC)PrevFrame);
    auxKeyFunc(AUX_x, (AUXKEYPROC)NextFrame);

    auxKeyFunc(AUX_l, (AUXKEYPROC)SwitchLighting);

    auxKeyFunc(AUX_UP, (AUXKEYPROC)YawUp);
    auxKeyFunc(AUX_DOWN, (AUXKEYPROC)YawDown);
    auxKeyFunc(AUX_LEFT, (AUXKEYPROC)AngleLeft);
    auxKeyFunc(AUX_RIGHT, (AUXKEYPROC)AngleRight);

    auxReshapeFunc((AUXRESHAPEPROC)CALLBACK_Reshape);
    auxMainLoop(CALLBACK_Draw);
}
