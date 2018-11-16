#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include "events.h"
#include "vmdl.h"

#pragma warning(disable:4244)

static void CALLBACK CALLBACK_Draw(void)
{
    Draw();

    // Меняем видеостраницы местами, чтобы показать на экране изображение
    // нарисованное в фоновом буфере
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
        fprintf(stdout, "view3d 1.1 (08-04-2003) by Stupin W.A.\n"
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
        return;
    }

    auxInitPosition(0, 0, 400, 300);
    auxInitDisplayMode(AUX_RGB | AUX_DOUBLE);
    if (auxInitWindow("view3d") == GL_FALSE)
    {
        auxQuit();
        return;
    }

    if (Init(varg[1]) != 0)
    {
        auxQuit();
        return;
    }

    auxKeyFunc(AUX_q, (AUXKEYPROC)DistUp);
    auxKeyFunc(AUX_a, (AUXKEYPROC)DistDown);
    auxKeyFunc(AUX_z, (AUXKEYPROC)PrevFrame);
    auxKeyFunc(AUX_x, (AUXKEYPROC)NextFrame);
    auxKeyFunc(AUX_c, (AUXKEYPROC)PrevSkin);
    auxKeyFunc(AUX_v, (AUXKEYPROC)NextSkin);
    
    auxKeyFunc(AUX_l, (AUXKEYPROC)SwitchLighting);
    auxKeyFunc(AUX_s, (AUXKEYPROC)SwitchDrawSkin);
    auxKeyFunc(AUX_n, (AUXKEYPROC)SwitchDrawNormals);

    auxKeyFunc(AUX_UP, (AUXKEYPROC)YawUp);
    auxKeyFunc(AUX_DOWN, (AUXKEYPROC)YawDown);
    auxKeyFunc(AUX_LEFT, (AUXKEYPROC)AngleLeft);
    auxKeyFunc(AUX_RIGHT, (AUXKEYPROC)AngleRight);

    auxReshapeFunc((AUXRESHAPEPROC)CALLBACK_Reshape);
    auxMainLoop(CALLBACK_Draw);
}
