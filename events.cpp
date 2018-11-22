#include <GL/gl.h>
#include "events.h"
#include "model.h"

const float ZBUFFER_MIN = 10.0f;

unsigned lighting;
unsigned frame;
unsigned num_frames;
float radius;

float zbuffer_min;
float zbuffer_max;

float camera_half_width;
float camera_half_height;
float camera_yaw;
float camera_angle;
float camera_dist;

Model model;

void Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearDepth(zbuffer_min);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glFrustum(-camera_half_width, camera_half_width,
              -camera_half_height, camera_half_height,
              zbuffer_min, zbuffer_max);
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
    {
        glDisable(GL_LIGHTING);
    }
    model.DrawModel(lighting, frame);

    glFinish();
}

void SwitchLighting(void)
{
    lighting++;
    if (lighting > 2)
    {
        lighting = 0;
    }
}

void DistUp(void)
{
    camera_dist -= 1.0f;
    if (camera_dist < zbuffer_min)
    {
        camera_dist = zbuffer_min;
    }
}

void DistDown(void)
{
    camera_dist += 1.0f;
    if (camera_dist > zbuffer_max)
    {
        camera_dist = zbuffer_max;
    }
}

void PrevFrame(void)
{
    frame--;
    if ((int)frame < 0)
    {
        frame = 0;
    }
}

void NextFrame(void)
{
    frame++;
    if (frame >= num_frames)
    {
        frame = num_frames - 1;
    }
}

void YawUp(void)
{
    camera_yaw += 1.0f;
    if (camera_yaw > 90.0f)
    {
        camera_yaw = 90.0f;
    }
}

void YawDown(void)
{
    camera_yaw -= 1.0f;
    if (camera_yaw < -90.0f)
    {
        camera_yaw = -90.0f;
    }
}

void AngleRight(void)
{
    camera_angle -= 1.0f;
    if (camera_angle < -180.0f)
    {
        camera_angle += 360.0f;
    }
}

void AngleLeft(void)
{
    camera_angle += 1.0f;
    if (camera_angle > 180.0f)
    {
        camera_angle -= 360.0f;
    }
}

int Init(const char *filename)
{
    if (!model.LoadIDP2Model(filename))
    {
        return -1;
    }
        
    lighting = 2;
    frame = 0;

    num_frames = model.GetNumFrames();
    radius = model.GetBoundRadius();

    zbuffer_min = ZBUFFER_MIN;
    zbuffer_max = zbuffer_min + 2.0f * radius;

    camera_half_width = 4.0f;
    camera_half_height = 4.0f;
    camera_yaw = 0.0f;
    camera_angle = 0.0f;
    camera_dist = zbuffer_min + radius;

    return 0;
}

void Reshape(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera_half_width = (float)width / (float)height * radius * zbuffer_min / camera_dist;
    camera_half_height = radius * zbuffer_min / camera_dist;
}
