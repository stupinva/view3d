#ifndef __VMDL__
#define __VMDL__

#ifndef __RGB__
#define __RGB__
struct RGB
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};
#endif

struct Point
{
	unsigned short s;
	unsigned short t;
	unsigned short vertex_index;
};

struct Triangle
{
	unsigned short point_indexes[3];
};

struct Vector3D
{
	float x;
	float y;
	float z;
};

const unsigned MAX_FRAME_NAME	=16;
struct Frame
{
	char name[MAX_FRAME_NAME];
	Vector3D min;
	Vector3D max;
};

class Model
{
	unsigned skin_width;
	unsigned skin_height;
	unsigned num_skins;
	RGB *skins;

	unsigned num_points;
	Point *points;

	unsigned num_triangles;
	Triangle *triangles;

	unsigned num_frames;
	Frame *frames;
	unsigned num_vertices;
	Vector3D *vertices;
	Vector3D *normals;
public:
	Model(void);
	~Model(void);
	bool addSkin(unsigned width,unsigned height,RGB *data);
	int addPoint(unsigned s,unsigned t,unsigned vertex_index);
	int addTriangle(int point_index0,int point_index1,int point_index2);
	int addFrame(const char *frame_name,const Vector3D min,const Vector3D max,
					unsigned numvertices,
					const Vector3D *verts,const Vector3D *norms);
	void calculateBoundBoxes(void);
	bool calculateNormals(void);
	bool verifyModel(void);
	void printInfo(void);
};

#endif