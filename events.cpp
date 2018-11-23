#include <math.h>
#include <GL/gl.h>
#include "events.h"
#include "vmdl.h"

const float ZBUFFER_MIN = 10.0f;
#ifndef M_PI
const float M_PI = 3.14159265f;
#endif

float zbuffer_min;
float zbuffer_max;

float camera_half_width;
float camera_half_height;
float camera_yaw;
float camera_angle;
float camera_dist;
float length;
Vector3D min;
Vector3D max;

bool draw_skin;
bool draw_normals;
bool lighting;

unsigned num_skins;
unsigned skin_num;

unsigned num_frames;
unsigned frame_num;

Model mdl;

void Draw(void)
{
    // Очищаем буферы цвета (буфер изображения) и глубины (z-буфер)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Настраиваем буфер глубины (z-буфер)
    glClearDepth(zbuffer_min);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Настраиваем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glFrustum(-camera_half_width, camera_half_width,
              -camera_half_height, camera_half_height, zbuffer_min, zbuffer_max);
    glTranslatef(0.0f, 0.0f, -camera_dist);
    glRotatef(camera_yaw, 1.0f, 0.0f, 0.0f);
    glRotatef(90.0f - camera_angle, 0.0f, 1.0f, 0.0f);
    
    // Настраиваем матрицу отрисовки моделей
    glMatrixMode(GL_MODELVIEW);
    // Меняем систему координат с glLoadIdentity():
    // ось x направлена вправо, ось y направлена вверх, ось z направлена вглубь экрана
    // на систему координат модели:
    // ось x направлена влево, y направлена вглубь, z направлена вверх (?)
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

    // Если нужно освещение, настраиваем и включаем его
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
    // Если освещение не нужно, выключаем его
    else
    {
        glDisable(GL_LIGHTING);
    }

    // Если нужно текстурировать модель, настраиваем параметры текстурирования
    if (draw_skin)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glEnable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);
        
        // Если нужно освещение, то текстура меняет яркость
        // в зависимости от степени освещённости
        if (lighting)
        {
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        // Если освещение не нужно, то цвет текстуры не зависит от освещения
        else
        {
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        }
    }
    // Если текстурировать не нужно, то отключаем текстурирование и
    // включаем заливку треугольников белым цветом
    else
    {
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    // Рисуем модель
    mdl.drawModel(skin_num, frame_num, draw_skin, lighting);

    // Если нужно показать нормали, то рисуем их белым цветом
    if (draw_normals)
    {
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        mdl.drawNormals(frame_num);
    }

    // Ждём выполнения всех отправленных команд OpenGL
    glFinish();
}

void SwitchDrawSkin(void)
{
    draw_skin = !draw_skin;
}

void SwitchLighting(void)
{
    lighting = !lighting;
}

void SwitchDrawNormals(void)
{
    draw_normals = !draw_normals;
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
    frame_num--;
    if ((int)frame_num < 0)
    {
        frame_num = 0;
    }
}

void NextFrame(void)
{
    frame_num++;
    if (frame_num >= num_frames)
    {
        frame_num = num_frames - 1;
    }
}

void PrevSkin(void)
{
    skin_num--;
    if ((int)skin_num < 0)
    {
        skin_num = 0;
    }
}

void NextSkin(void)
{
    skin_num++;
    if (skin_num >= num_skins)
    {
        skin_num = num_skins - 1;
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
    if (!loadIDPO(&mdl, filename))
    {
        return -1;
    }

    draw_skin = true;
    draw_normals = false;
    lighting = true;

    num_skins = mdl.getNumSkins();
    skin_num = 0;

    num_frames = mdl.getNumFrames();
    frame_num = 0;

    mdl.getMinMax(&min, &max);
    float min_length = Length(min);
    float max_length = Length(max);
    if (max_length > min_length)
    {
        length = max_length;
    }
    else
    {
        length = min_length;
    }

    zbuffer_min = ZBUFFER_MIN;
    //zbuffer_max = 2.0f * ZBUFFER_MIN;
    zbuffer_max = length * 2 + zbuffer_min;

    camera_half_width = 4.0f;
    camera_half_height = 4.0f;
    camera_yaw = 0.0f;
    camera_angle = 0.0f;
    //camera_dist = (zbuffer_max - zbuffer_min) / 2.0f;
    camera_dist = length + zbuffer_min;

    return 0;
}

void Reshape(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera_half_width = (float)width / (float)height * length * zbuffer_min / camera_dist;
    camera_half_height = length * zbuffer_min / camera_dist;
}
