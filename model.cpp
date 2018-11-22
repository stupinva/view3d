#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include "model.h"

Model::Model(void)
{
    name[0] = 0;
    num_skins = 0;
    num_points = 0;
    num_meshes = 0;
    num_triangles = 0;
    num_frames = 0;
    num_vertices = 0;
}

Model::~Model(void)
{
    FreeModel();
}

void Model::FreeModel(void)
{
    if (num_points)
    {
        free(points);
        num_points = 0;
    }

    if (num_meshes)
    {
        free(meshes);
        num_meshes = 0;
    }

    if (num_triangles)
    {
        free(triangles);
        num_triangles = 0;
    }

    if (num_frames)
    {
        free(frame_names);
        free(triangle_normals);
        free(vertex_normals);
        free(vertex_positions);
        num_frames = 0;
        num_vertices = 0;
    }
}

bool Model::NewModel(unsigned arg_num_skins,
                     unsigned arg_num_points,
                     unsigned arg_num_meshes,
                     unsigned arg_num_triangles,
                     unsigned arg_num_frames,
                     unsigned arg_num_vertices)
{
    // Удаляем уже загруженную модель, если она есть
    FreeModel();

    // Выделяем память под имена текстур
    skin_indexes = (unsigned *)malloc(arg_num_skins * sizeof(*skin_indexes));
    if (skin_indexes == NULL)
    {
        fprintf(stderr, "Could not get memory for skin names.\n");
        return false;
    }

    // Выделяем память под точки - координаты вершин на текстуре
    points = (Vector2D *)malloc(arg_num_points * sizeof(*points));
    if (points == NULL)
    {
        free(skin_indexes);
        fprintf(stderr, "Could not get memory for points.\n");
        return false;
    }

    // Выделяем память под группы треугольников
    meshes = (Mesh *)malloc(arg_num_meshes * sizeof(*meshes));
    if (meshes == NULL)
    {
        free(points);
        free(skin_indexes);
        fprintf(stderr, "Could not get memory for meshes.\n");
        return false;
    }

    // Выделяем память под треугольники
    triangles = (Triangle *)malloc(arg_num_triangles * sizeof(*triangles));
    if (triangles == NULL)
    {
        free(meshes);
        free(points);
        free(skin_indexes);
        fprintf(stderr, "Could not get memory for triangles.\n");
        return false;
    }

    // Выделяем память под кадры:
    // Выделяем память под имена кадров
    frame_names = (ModelName *)malloc(arg_num_frames * sizeof(*frame_names));
    if (frame_names == NULL)
    {
        free(triangles);
        free(meshes);
        free(points);
        free(skin_indexes);
        fprintf(stderr, "Could not get memory for frame names.\n");
        return false;
    }

    // Выделяем память под координаты вершин
    vertex_positions = (Vector3D *)malloc(arg_num_frames * arg_num_vertices * sizeof(*vertex_positions));
    if (vertex_positions == NULL)
    {
        free(frame_names);
        free(triangles);
        free(meshes);
        free(points);
        free(skin_indexes);
        fprintf(stderr, "Could not get memory for vertex positions.\n");
        return false;
    }

    // Выделяем память под нормали к вершинам
    vertex_normals = (Vector3D *)malloc(arg_num_frames * arg_num_vertices * sizeof(*vertex_normals));
    if (vertex_normals == NULL)
    {
        free(vertex_positions);
        free(frame_names);
        free(triangles);
        free(meshes);
        free(points);
        free(skin_indexes);
        fprintf(stderr, "Could not get memory for vertex normals.\n");
        return false;
    }

    // Выделяем память под нормали к треугольникам
    triangle_normals = (Vector3D *)malloc(arg_num_frames * arg_num_triangles * sizeof(*triangle_normals));
    if (triangle_normals == NULL)
    {
        free(vertex_normals);
        free(vertex_positions);
        free(frame_names);
        free(triangles);
        free(meshes);
        free(points);
        free(skin_indexes);
        fprintf(stderr,"Could not get memory for triangle normals.\n");
        return false;
    }

    // Наконец-то память подо все массивы успешно выделена, запоминаем новое состояноие модели
    num_skins = arg_num_skins;
    num_points = arg_num_points;
    num_meshes = arg_num_meshes;
    num_triangles = arg_num_triangles;
    num_frames = arg_num_frames;
    num_vertices = arg_num_vertices;
    return true;
}

bool Model::CalculateNormals(void)
{
    if ((num_triangles == 0) || (num_frames == 0) || (num_vertices == 0))
    {
        return false;
    }

    unsigned *vertex_normal_denominators = (unsigned *)malloc(num_vertices * sizeof(*vertex_normal_denominators));
    if (vertex_normal_denominators == NULL)
    {
        fprintf(stderr, "Could not calculate normals.\n");
        return false;
    }

    for(unsigned i = 0; i < num_frames; i++)
    {
        Vector3D *frame_vertex_positions = &vertex_positions[i * num_vertices];
        Vector3D *frame_vertex_normals = &vertex_normals[i * num_vertices];
        Vector3D *frame_triangle_normals = &triangle_normals[i * num_triangles];

        // Обнуляем нормали к вершинам текущего кадра и делитель
        for(unsigned j = 0; j < num_vertices; j++)
        {
            frame_vertex_normals[j][0] = 0.0f;
            frame_vertex_normals[j][1] = 0.0f;
            frame_vertex_normals[j][2] = 0.0f;
            vertex_normal_denominators[j] = 0;
        }

        for(unsigned j = 0; j < num_triangles; j++)
        {
            unsigned vertex_indexes[3];
            for(unsigned k = 0; k < 3; k++)
            {
                vertex_indexes[k] = triangles[j].vertex_indexes[k];
            }

            // Вычисляем нормали к треугольникам текущего кадра
            Vector3D u;
            VectorSub
            (
                frame_vertex_positions[vertex_indexes[1]],
                frame_vertex_positions[vertex_indexes[0]],
                u
            );
            Vector3D v;
            VectorSub
            (
                frame_vertex_positions[vertex_indexes[2]],
                frame_vertex_positions[vertex_indexes[1]],
                v
            );
            Vector3D triangle_normal;
            CrossProduct(u, v, triangle_normal);
            VectorNormalize(triangle_normal, triangle_normal);
            VectorCopy(triangle_normal, frame_triangle_normals[j]);

            // Добавляем к нормалям вершин, принадлежащих текущему треугольнику его нормаль
            for(unsigned k = 0; k < 3; k++)
            {
                VectorAdd
                (
                    frame_vertex_normals[vertex_indexes[k]],
                    triangle_normal,
                    frame_vertex_normals[vertex_indexes[k]]
                );
                vertex_normal_denominators[vertex_indexes[k]]++;
            }
        }

        // Вычисляем норамли к вершинам текущего кадра
        for(unsigned j = 0; j < num_vertices; j++)
        {
            VectorScale
            (
                frame_vertex_normals[j],
                1.0f / (float)vertex_normal_denominators[j],
                frame_vertex_normals[j]
            );
        }
    }
    free(vertex_normal_denominators);
    return true;
}

void VectorCopy(Vector3D a, Vector3D ans)
{
    ans[0] = a[0];
    ans[1] = a[1];
    ans[2] = a[2];
}

void CrossProduct(Vector3D a, Vector3D b, Vector3D ans)
{
    ans[0] = a[1] * b[2] - a[2] * b[1];
    ans[1] = a[2] * b[0] - a[0] * b[2];
    ans[2] = a[0] * b[1] - a[1] * b[0];
}

void VectorSub(Vector3D a, Vector3D b, Vector3D ans)
{
    ans[0] = a[0] - b[0];
    ans[1] = a[1] - b[1];
    ans[2] = a[2] - b[2];
}

void VectorAdd(Vector3D a, Vector3D b, Vector3D ans)
{
    ans[0] = a[0] + b[0];
    ans[1] = a[1] + b[1];
    ans[2] = a[2] + b[2];
}

void VectorScale(Vector3D a, float f, Vector3D ans)
{
    ans[0] = a[0] * f;
    ans[1] = a[1] * f;
    ans[2] = a[2] * f;
}

float VectorLength(Vector3D v)
{
    float length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    length = (float)sqrt(length);
    return length;
}

const float DELTA = 0.00001f;
void VectorNormalize(Vector3D a, Vector3D ans)
{
    float length = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    length = (float)sqrt(length);
    if (length < DELTA)
    {
        return;
    }
    ans[0] = a[0] / length;
    ans[1] = a[1] / length;
    ans[2] = a[2] / length;
}

float Model::GetBoundRadius(void)
{
    if (num_vertices && num_vertices)
    {
        Vector3D min;
        Vector3D max;
        VectorCopy(vertex_positions[0], min);
        VectorCopy(min, max);
        for(unsigned i = 1; i < num_frames * num_vertices; i++)
        {
            if (min[0] > vertex_positions[i][0])
            {
                min[0] = vertex_positions[i][0];
            }

            if (min[1] > vertex_positions[i][1])
            {
                min[1] = vertex_positions[i][1];
            }

            if (min[2] > vertex_positions[i][2])
            {
                min[2] = vertex_positions[i][2];
            }
            
            if (max[0] < vertex_positions[i][0])
            {
                max[0] = vertex_positions[i][0];
            }

            if (max[1] < vertex_positions[i][1])
            {
                max[1] = vertex_positions[i][1];
            }

            if (max[2] < vertex_positions[i][2])
            {
                max[2] = vertex_positions[i][2];
            }
        }
        float min_length = VectorLength(min);
        float max_length = VectorLength(max);

        if (max_length > min_length)
        {
            return max_length;
        }
        else
        {
            return min_length;
        }
    }
    return 0.0f;
}

void Model::DrawModel(unsigned lighting, unsigned frame)
{
    if (frame < num_frames && num_triangles && num_vertices)
    {
        Vector3D *frame_vertex_positions = &vertex_positions[frame * num_vertices];
        Vector3D *frame_vertex_normals = &vertex_normals[frame * num_vertices];
        Vector3D *frame_triangle_normals = &triangle_normals[frame * num_triangles];

        unsigned i;

        glBegin(GL_TRIANGLES);
        switch (lighting)
        {
            case 0:
                for(i = 0; i < num_triangles; i++)
                {
                    for(unsigned j = 0; j < 3; j++)
                    {
                        glVertex3fv(frame_vertex_positions[triangles[i].vertex_indexes[j]]);
                    }
                }
                break;

            case 1:
                for(i = 0; i < num_triangles; i++)
                {
                    glNormal3fv(frame_triangle_normals[i]);
                    for(unsigned j = 0; j < 3; j++)
                    {
                        glVertex3fv(frame_vertex_positions[triangles[i].vertex_indexes[j]]);
                    }
                }
                break;

            case 2:
                for(i = 0; i < num_triangles; i++)
                {
                    for(unsigned j = 0; j < 3; j++)
                    {
                        unsigned vertex_index = triangles[i].vertex_indexes[j];
                        glNormal3fv(frame_vertex_normals[vertex_index]);
                        glVertex3fv(frame_vertex_positions[vertex_index]);
                    }
                }
                break;
        }
        glEnd();
    }
}

unsigned Model::GetNumFrames(void)
{
    return num_frames;
}

/*
void Model::DumpModel(void)
{
    fprintf(stdout, "--- model ---\n%s\n", name);

    fprintf(stdout, "--- skins ---\n");
    for(unsigned i = 0; i < num_skins; i++)
    {
        fprintf(stdout, "%s\n", skin_names[i]);
    }

    fprintf(stdout, "--- meshes ---\n");
    for(i = 0; i < num_meshes; i++)
    {
        fprintf(stdout, "%s\n", meshes[i].name);
    }

    fprintf(stdout, "--- frames ---\n");
    for(i = 0; i < num_frames; i++)
    {
        fprintf(stdout, "%s\n", frame_names[i]);
    }
}
*/
