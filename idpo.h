#ifndef __IDPO__
#define __IDPO__

#include "vmdl.h"

#ifndef __RGB__
#define __RGB__
struct RGB
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};
#endif

const long IDPO_IDENT	= 0x4F504449;
struct IDPOHeader
{
	long id;                    // 0x4F504449 = "IDPO" for IDPOLYGON
	long version;               // Version = 6
	Vector3D scale;               // Model scale factors.
	Vector3D origin;              // Model origin.
	float radius;            // Model bounding radius.
	Vector3D offsets;             // Eye position (useless?)
	long numskins ;             // the number of skin textures
	long skinwidth;             // Width of skin texture
		                        //           must be multiple of 8
	long skinheight;            // Height of skin texture
								//           must be multiple of 8
	long numverts;              // Number of vertices
	long numtris;				// Number of triangles surfaces
	long numframes;             // Number of frames
	long synctype;              // 0= synchron, 1= random
	long flags;                 // 0 (see Alias models)
	float size;              // average size of triangles
};
/*
struct skin_t
{
	long group;							// value = 0
	unsigned char skin[skinwidth*skinheight];	// the skin picture
};

struct skingroup_t
{
	long group;                 // value = 1
	long nb;                    // number of pictures in group
	float time[nb];             // time values, for each picture
	unsigned char skin[nb][skinwidth*skinheight]; // the pictures 
};
*/
struct IDPOPoint
{
	long onseam;				// 0 or 0x20
	long s;						// position, horizontally
								//  in range [0,skinwidth[
	long t;						// position, vertically
								//  in range [0,skinheight[
};

//stvert_t vertices[numverts];

struct IDPOTriangle
{
	long facesfront;			// boolean
	long vertices[3];			// Index of 3 triangle vertices
								// in range [0,numverts[
};
/*
  for(j=0; j < numtris; j++)
  {
    for(i=0; i < 3 ; i++)
    { 
      vertex = triangles[j].vertices[i]
      s = vertices[vertex].s;
      t = vertices[vertex].t;
      if( (vertices[vertex].onseam) && (!triangle[j].facesfront))
      { 
        s += skinwidth / 2;
      }
      // use s and t as the coordinates of the vertex
    }
  }
*/
struct IDPOVertex
{
	unsigned char packedposition[3];    // X,Y,Z coordinate, packed on 0-255
	unsigned char lightnormalindex;     // index of the vertex normal
};
/*
struct simpleframe_t
{
	trivertx_t min;              // minimum values of X,Y,Z
	trivertx_t max;              // maximum values of X,Y,Z
	char name[16];               // name of frame
	trivertx_t frame[numverts];  // array of vertices
};
*/
const unsigned IDPO_MAX_FRAME_NAME	=16;
struct IDPOFrameHeader
{
	IDPOVertex min;              // minimum values of X,Y,Z
	IDPOVertex max;              // maximum values of X,Y,Z
	char name[IDPO_MAX_FRAME_NAME];// name of frame
};
/*
struct frame_t
{
	long group;			         // Value = 0
	simpleframe_t frame;         // a single frame definition
};

struct framegroup_t
{
	long group;                  // Value != 0
	trivertx_t min;              // min position in all simple frames
	trivertx_t max;              // max position in all simple frames
	long nb;                     // number of frames in group
	float time[nb]               // time for each of the single frames
	simpleframe_t frames[nb];    // a group of simple frames
};
*/

extern const RGB quake_palette[];
extern Vector3D quake_normals[162];
#endif