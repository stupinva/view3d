#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <stdio.h>
#include "texture.h"
#include "model.h"

#pragma warning(disable:4244)

const float ZBUFFER_MIN = 10.0f;

unsigned lighting = 2;
unsigned frame = 0;
unsigned num_frames = 0;
float radius;

float zbuffer_min = ZBUFFER_MIN;
float zbuffer_max = zbuffer_min+2.0f*radius;

float camera_half_width = 4.0f;
float camera_half_height = 4.0f;
float camera_yaw = 0.0f;
float camera_angle = 0.0f;
float camera_dist = radius;

Model model;
static void CALLBACK Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glFrustum(-camera_half_width, camera_half_width,
        -camera_half_height, camera_half_height, zbuffer_min, zbuffer_max);
    glTranslatef(0.0f, 0.0f, -camera_dist);
    glRotatef(camera_yaw, 1.0f, 0.0f, 0.0f);
    glRotatef(90.0f - camera_angle, 0.0f, 1.0f, 0.0f);
    
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    const float xyz[] =
    {
        -1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    glLoadMatrixf(xyz);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    if (lighting)
    {
        glEnable(GL_LIGHTING);

        const float direction0[] = {0.0f, -1.0f, 0.0f};
        const float position0[] = {0.0f, camera_dist, 0.0f, 1.0f};
        const float specular0[] = {5.0f, 5.0f, 5.0f, 5.0f};
        const float diffuse0[] = {5.0f, 5.0f, 5.0f, 5.0f};
        const float ambient0[] = {3.0f, 3.0f, 3.0f, 3.0f};
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.0f);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction0);
        glLightfv(GL_LIGHT0, GL_POSITION, position0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
        glEnable(GL_LIGHT0);

        const float direction1[] = {0.0f, 1.0f, 0.0f};
        const float position1[] = {0.0f, -camera_dist, 0.0f, 1.0f};
        const float specular1[] = {5.0f, 5.0f, 5.0f, 5.0f};
        const float diffuse1[] = {5.0f, 5.0f, 5.0f, 5.0f};
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.0f);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
        glLightfv(GL_LIGHT1, GL_POSITION, position1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
        glEnable(GL_LIGHT1);
    }
    else
        glDisable(GL_LIGHTING);
    model.DrawModel(lighting, frame);

    glFinish();
    auxSwapBuffers();
}

static void SwitchLighting(void)
{
    lighting++;
    if (lighting > 2)
        lighting = 0;
}
static void DistUp(void)
{
    camera_dist -= 1.0f;
    if (camera_dist < zbuffer_min)
        camera_dist = zbuffer_min;
}
static void DistDown(void)
{
    camera_dist += 1.0f;
    if (camera_dist > zbuffer_max)
        camera_dist = zbuffer_max;
}
static void PrevFrame(void)
{
    frame--;
    if ((int)frame < 0)
        frame = 0;
}
static void NextFrame(void)
{
    frame++;
    if (frame >= num_frames)
        frame = num_frames - 1;
}
static void YawUp(void)
{
    camera_yaw += 1.0f;
    if (camera_yaw > 90.0f)
        camera_yaw = 90.0f;
}
static void YawDown(void)
{
    camera_yaw -= 1.0f;
    if (camera_yaw < -90.0f)
        camera_yaw = -90.0f;
}
static void AngleRight(void)
{
    camera_angle -= 1.0f;
    if (camera_angle < -180.0f)
        camera_angle += 360.0f;
}
static void AngleLeft(void)
{
    camera_angle += 1.0f;
    if (camera_angle > 180.0f)
        camera_angle -= 360.0f;
}
void InitKeyboard(void)
{
    auxKeyFunc(AUX_q, (AUXKEYPROC)DistUp);
    auxKeyFunc(AUX_a, (AUXKEYPROC)DistDown);
    auxKeyFunc(AUX_z, (AUXKEYPROC)PrevFrame);
    auxKeyFunc(AUX_x, (AUXKEYPROC)NextFrame);

    auxKeyFunc(AUX_l, (AUXKEYPROC)SwitchLighting);

    auxKeyFunc(AUX_UP, (AUXKEYPROC)YawUp);
    auxKeyFunc(AUX_DOWN, (AUXKEYPROC)YawDown);
    auxKeyFunc(AUX_LEFT, (AUXKEYPROC)AngleLeft);
    auxKeyFunc(AUX_RIGHT, (AUXKEYPROC)AngleRight);
}

static void Init(void)
{
    InitKeyboard();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(zbuffer_min);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}
static void CALLBACK Reshape(unsigned width,unsigned height)
{
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera_half_width = (float)width / (float)height * radius * zbuffer_min / camera_dist;
    camera_half_height = radius * zbuffer_min / camera_dist;
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
    if (!model.LoadIDP2Model(varg[1]))
        return;
        
    num_frames = model.GetNumFrames();
    radius = model.GetBoundRadius();

    camera_dist = zbuffer_min + radius;
    zbuffer_max = zbuffer_min + 2.0f * radius;

    auxInitPosition(0, 0, 800, 600);
    auxInitDisplayMode(AUX_RGB | AUX_DOUBLE);
    if (auxInitWindow("view3d") == GL_FALSE)
        auxQuit();
    Init();
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxMainLoop(Draw);
}
