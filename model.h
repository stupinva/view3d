#ifndef __MODEL__
#define __MODEL__
#include "texture.h"
////////////////////////////////////////////////
//04-07-2003                                  //
//Структуры модели, формат и кое-какие методы.//
////////////////////////////////////////////////
typedef float Vector2D[2];
typedef float Vector3D[3];

typedef char ModelName[16];

struct Mesh
{
	ModelName name;
	unsigned first_skin;
	unsigned num_skins;
};

struct Triangle
{
	unsigned mesh_index;
	unsigned vertex_indexes[3];
	unsigned point_indexes[3];
};

class Model
{
	//ModelName skin_names[num_skins];
	//Vector2D points[num_points];
	//Mesh meshes[num_meshes];
	//Triangle triangles[num_triangles];
	
	//ModelName frame_names[num_frames];
	//Vector3D traingle_normals[num_frames][num_triangles];
	//Vector3D vertex_normals[num_frames][num_vertices];
	//Vector3D vertex_positions[num_frames][num_vertices];
	
	ModelName name;
	unsigned num_skins;
	unsigned num_points;
	unsigned num_meshes;
	unsigned num_triangles;
	unsigned num_frames;
	unsigned num_vertices;
	
	unsigned *skin_indexes;
	Vector2D *points;
	Mesh *meshes;
	Triangle *triangles;
	ModelName *frame_names;
	Vector3D *triangle_normals;
	Vector3D *vertex_normals;
	Vector3D *vertex_positions;
public:
	Model(void);
	~Model(void);
	bool NewModel(unsigned arg_num_skins,
					unsigned arg_num_points,
					unsigned arg_num_meshes,
					unsigned arg_num_triangles,
					unsigned arg_num_frames,
					unsigned arg_num_vertices
					);
	void FreeModel(void);
	bool LoadIDP2Model(const char *file_name);
	bool CalculateNormals(void);
	float GetBoundRadius(void);
	//Освещение: 0 - нет, 1 - Ламберта, 2 - Гуро, (3 - Фонга).
	void DrawModel(unsigned lighting,unsigned frame);
	unsigned GetNumFrames(void);
	void DumpModel(void);
};

void VectorCopy(Vector3D a,Vector3D ans);
void CrossProduct(Vector3D a,Vector3D b,Vector3D ans);
void VectorSub(Vector3D a,Vector3D b,Vector3D ans);
void VectorAdd(Vector3D a,Vector3D b,Vector3D ans);
void VectorScale(Vector3D a,float f,Vector3D ans);
float VectorLength(Vector3D v);
void VectorNormalize(Vector3D a,Vector3D ans);

//Кэш текстур, здесь хранятся все текстуры для всех загруженных моделей.
//extern Textures textures_cashe;
#endif