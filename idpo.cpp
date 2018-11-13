#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

const long IDPO_IDENT	=0x4F504449;
struct IDPOHeader
{
	unsigned long id;			// 0x4F504449 = "IDPO" for IDPOLYGON
	unsigned long version;      // Version = 3, 6
};
struct IDPO3InfoHeader
{
	Vector3D scale;             // Model scale factors.
	Vector3D origin;            // Model origin.
	float radius;				// Model bounding radius.
	Vector3D offsets;           // Eye position (useless?)
	unsigned long numskins ;    // the number of skin textures
	unsigned long skinwidth;    // Width of skin texture
		                        //           must be multiple of 8
	unsigned long skinheight;   // Height of skin texture
								//           must be multiple of 8
	unsigned long numverts;     // Number of vertices
	unsigned long numtris;		// Number of triangles surfaces
	unsigned long numframes;    // Number of frames
	unsigned long synctype;     // 0= synchron, 1= random
};
struct IDPO6InfoHeader
{
	Vector3D scale;             // Model scale factors.
	Vector3D origin;            // Model origin.
	float radius;				// Model bounding radius.
	Vector3D offsets;           // Eye position (useless?)
	unsigned long numskins ;    // the number of skin textures
	unsigned long skinwidth;    // Width of skin texture
		                        //           must be multiple of 8
	unsigned long skinheight;   // Height of skin texture
								//           must be multiple of 8
	unsigned long numverts;     // Number of vertices
	unsigned long numtris;		// Number of triangles surfaces
	unsigned long numframes;    // Number of frames
	unsigned long synctype;     // 0= synchron, 1= random
	unsigned long flags;        // 0 (see Alias models)
	float size;					// average size of triangles
};
const IDPO3_ON_SEAM	=0x01;
const IDPO6_ON_SEAM	=0x20;
struct IDPOPoint
{
	unsigned long onseam;				// 0 or 0x20
	unsigned long s;						// position, horizontally
								//  in range [0,skinwidth[
	unsigned long t;						// position, vertically
								//  in range [0,skinheight[
};
struct IDPOTriangle
{
	unsigned long facesfront;			// boolean
	unsigned long vertices[3];			// Index of 3 triangle vertices
								// in range [0,numverts[
};
struct IDPOVertex
{
	unsigned char packedposition[3];    // X,Y,Z coordinate, packed on 0-255
	unsigned char lightnormalindex;     // index of the vertex normal
};
struct IDPO3FrameHeader
{
	IDPOVertex min;					// minimum values of X,Y,Z
	IDPOVertex max;					// maximum values of X,Y,Z
};
const unsigned IDPO_MAX_FRAME_NAME	=16;
struct IDPO6FrameHeader
{
	IDPOVertex min;					// minimum values of X,Y,Z
	IDPOVertex max;					// maximum values of X,Y,Z
	char name[IDPO_MAX_FRAME_NAME];	// name of frame
};

const RGB quake_palette[]={
	{0x00,0x00,0x00},{0x0F,0x0F,0x0F},{0x1F,0x1F,0x1F},{0x2F,0x2F,0x2F},
	{0x3F,0x3F,0x3F},{0x4B,0x4B,0x4B},{0x5B,0x5B,0x5B},{0x6B,0x6B,0x6B},
	{0x7B,0x7B,0x7B},{0x8B,0x8B,0x8B},{0x9B,0x9B,0x9B},{0xAB,0xAB,0xAB},
	{0xBB,0xBB,0xBB},{0xCB,0xCB,0xCB},{0xDB,0xDB,0xDB},{0xEB,0xEB,0xEB},
	{0x0F,0x0B,0x07},{0x17,0x0F,0x0B},{0x1F,0x17,0x0B},{0x27,0x1B,0x0F},
	{0x2F,0x23,0x13},{0x37,0x2B,0x17},{0x3F,0x2F,0x17},{0x4B,0x37,0x1B},
	{0x53,0x3B,0x1B},{0x5B,0x43,0x1F},{0x63,0x4B,0x1F},{0x6B,0x53,0x1F},
	{0x73,0x57,0x1F},{0x7B,0x5F,0x23},{0x83,0x67,0x23},{0x8F,0x6F,0x23},
	{0x0B,0x0B,0x0F},{0x13,0x13,0x1B},{0x1B,0x1B,0x27},{0x27,0x27,0x33},
	{0x2F,0x2F,0x3F},{0x37,0x37,0x4B},{0x3F,0x3F,0x57},{0x47,0x47,0x67},
	{0x4F,0x4F,0x73},{0x5B,0x5B,0x7F},{0x63,0x63,0x8B},{0x6B,0x6B,0x97},
	{0x73,0x73,0xA3},{0x7B,0x7B,0xAF},{0x83,0x83,0xBB},{0x8B,0x8B,0xCB},
	{0x00,0x00,0x00},{0x07,0x07,0x00},{0x0B,0x0B,0x00},{0x13,0x13,0x00},
	{0x1B,0x1B,0x00},{0x23,0x23,0x00},{0x2B,0x2B,0x07},{0x2F,0x2F,0x07},
	{0x37,0x37,0x07},{0x3F,0x3F,0x07},{0x47,0x47,0x07},{0x4B,0x4B,0x0B},
	{0x53,0x53,0x0B},{0x5B,0x5B,0x0B},{0x63,0x63,0x0B},{0x6B,0x6B,0x0F},
	{0x07,0x00,0x00},{0x0F,0x00,0x00},{0x17,0x00,0x00},{0x1F,0x00,0x00},
	{0x27,0x00,0x00},{0x2F,0x00,0x00},{0x37,0x00,0x00},{0x3F,0x00,0x00},
	{0x47,0x00,0x00},{0x4F,0x00,0x00},{0x57,0x00,0x00},{0x5F,0x00,0x00},
	{0x67,0x00,0x00},{0x6F,0x00,0x00},{0x77,0x00,0x00},{0x7F,0x00,0x00},
	{0x13,0x13,0x00},{0x1B,0x1B,0x00},{0x23,0x23,0x00},{0x2F,0x2B,0x00},
	{0x37,0x2F,0x00},{0x43,0x37,0x00},{0x4B,0x3B,0x07},{0x57,0x43,0x07},
	{0x5F,0x47,0x07},{0x6B,0x4B,0x0B},{0x77,0x53,0x0F},{0x83,0x57,0x13},
	{0x8B,0x5B,0x13},{0x97,0x5F,0x1B},{0xA3,0x63,0x1F},{0xAF,0x67,0x23},
	{0x23,0x13,0x07},{0x2F,0x17,0x0B},{0x3B,0x1F,0x0F},{0x4B,0x23,0x13},
	{0x57,0x2B,0x17},{0x63,0x2F,0x1F},{0x73,0x37,0x23},{0x7F,0x3B,0x2B},
	{0x8F,0x43,0x33},{0x9F,0x4F,0x33},{0xAF,0x63,0x2F},{0xBF,0x77,0x2F},
	{0xCF,0x8F,0x2B},{0xDF,0xAB,0x27},{0xEF,0xCB,0x1F},{0xFF,0xF3,0x1B},
	{0x0B,0x07,0x00},{0x1B,0x13,0x00},{0x2B,0x23,0x0F},{0x37,0x2B,0x13},
	{0x47,0x33,0x1B},{0x53,0x37,0x23},{0x63,0x3F,0x2B},{0x6F,0x47,0x33},
	{0x7F,0x53,0x3F},{0x8B,0x5F,0x47},{0x9B,0x6B,0x53},{0xA7,0x7B,0x5F},
	{0xB7,0x87,0x6B},{0xC3,0x93,0x7B},{0xD3,0xA3,0x8B},{0xE3,0xB3,0x97},
	{0xAB,0x8B,0xA3},{0x9F,0x7F,0x97},{0x93,0x73,0x87},{0x8B,0x67,0x7B},
	{0x7F,0x5B,0x6F},{0x77,0x53,0x63},{0x6B,0x4B,0x57},{0x5F,0x3F,0x4B},
	{0x57,0x37,0x43},{0x4B,0x2F,0x37},{0x43,0x27,0x2F},{0x37,0x1F,0x23},
	{0x2B,0x17,0x1B},{0x23,0x13,0x13},{0x17,0x0B,0x0B},{0x0F,0x07,0x07},
	{0xBB,0x73,0x9F},{0xAF,0x6B,0x8F},{0xA3,0x5F,0x83},{0x97,0x57,0x77},
	{0x8B,0x4F,0x6B},{0x7F,0x4B,0x5F},{0x73,0x43,0x53},{0x6B,0x3B,0x4B},
	{0x5F,0x33,0x3F},{0x53,0x2B,0x37},{0x47,0x23,0x2B},{0x3B,0x1F,0x23},
	{0x2F,0x17,0x1B},{0x23,0x13,0x13},{0x17,0x0B,0x0B},{0x0F,0x07,0x07},
	{0xDB,0xC3,0xBB},{0xCB,0xB3,0xA7},{0xBF,0xA3,0x9B},{0xAF,0x97,0x8B},
	{0xA3,0x87,0x7B},{0x97,0x7B,0x6F},{0x87,0x6F,0x5F},{0x7B,0x63,0x53},
	{0x6B,0x57,0x47},{0x5F,0x4B,0x3B},{0x53,0x3F,0x33},{0x43,0x33,0x27},
	{0x37,0x2B,0x1F},{0x27,0x1F,0x17},{0x1B,0x13,0x0F},{0x0F,0x0B,0x07},
	{0x6F,0x83,0x7B},{0x67,0x7B,0x6F},{0x5F,0x73,0x67},{0x57,0x6B,0x5F},
	{0x4F,0x63,0x57},{0x47,0x5B,0x4F},{0x3F,0x53,0x47},{0x37,0x4B,0x3F},
	{0x2F,0x43,0x37},{0x2B,0x3B,0x2F},{0x23,0x33,0x27},{0x1F,0x2B,0x1F},
	{0x17,0x23,0x17},{0x0F,0x1B,0x13},{0x0B,0x13,0x0B},{0x07,0x0B,0x07},
	{0xFF,0xF3,0x1B},{0xEF,0xDF,0x17},{0xDB,0xCB,0x13},{0xCB,0xB7,0x0F},
	{0xBB,0xA7,0x0F},{0xAB,0x97,0x0B},{0x9B,0x83,0x07},{0x8B,0x73,0x07},
	{0x7B,0x63,0x07},{0x6B,0x53,0x00},{0x5B,0x47,0x00},{0x4B,0x37,0x00},
	{0x3B,0x2B,0x00},{0x2B,0x1F,0x00},{0x1B,0x0F,0x00},{0x0B,0x07,0x00},
	{0x00,0x00,0xFF},{0x0B,0x0B,0xEF},{0x13,0x13,0xDF},{0x1B,0x1B,0xCF},
	{0x23,0x23,0xBF},{0x2B,0x2B,0xAF},{0x2F,0x2F,0x9F},{0x2F,0x2F,0x8F},
	{0x2F,0x2F,0x7F},{0x2F,0x2F,0x6F},{0x2F,0x2F,0x5F},{0x2B,0x2B,0x4F},
	{0x23,0x23,0x3F},{0x1B,0x1B,0x2F},{0x13,0x13,0x1F},{0x0B,0x0B,0x0F},
	{0x2B,0x00,0x00},{0x3B,0x00,0x00},{0x4B,0x07,0x00},{0x5F,0x07,0x00},
	{0x6F,0x0F,0x00},{0x7F,0x17,0x07},{0x93,0x1F,0x07},{0xA3,0x27,0x0B},
	{0xB7,0x33,0x0F},{0xC3,0x4B,0x1B},{0xCF,0x63,0x2B},{0xDB,0x7F,0x3B},
	{0xE3,0x97,0x4F},{0xE7,0xAB,0x5F},{0xEF,0xBF,0x77},{0xF7,0xD3,0x8B},
	{0xA7,0x7B,0x3B},{0xB7,0x9B,0x37},{0xC7,0xC3,0x37},{0xE7,0xE3,0x57},
	{0x7F,0xBF,0xFF},{0xAB,0xE7,0xFF},{0xD7,0xFF,0xFF},{0x67,0x00,0x00},
	{0x8B,0x00,0x00},{0xB3,0x00,0x00},{0xD7,0x00,0x00},{0xFF,0x00,0x00},
	{0xFF,0xF3,0x93},{0xFF,0xF7,0xC7},{0xFF,0xFF,0xFF},{0x9F,0x5B,0x53}
	};

Vector3D quake_normals[162]={
	{-0.5257f,0.0000f,0.8507f},{-0.4429f,0.2389f,0.8642f},{-0.2952f,0.0000f,0.9554f},
	{-0.3090f,0.5000f,0.8090f},{-0.1625f,0.2629f,0.9511f},{0.0000f,0.0000f,1.0000f},
	{0.0000f,0.8507f,0.5257f},{-0.1476f,0.7166f,0.6817f},{0.1476f,0.7166f,0.6817f},
	{0.0000f,0.5257f,0.8507f},{0.3090f,0.5000f,0.8090f},{0.5257f,0.0000f,0.8507f},
	{0.2952f,0.0000f,0.9554f},{0.4429f,0.2389f,0.8642f},{0.1625f,0.2629f,0.9511f},
	{-0.6817f,0.1476f,0.7166f},{-0.8090f,0.3090f,0.5000f},{-0.5878f,0.4253f,0.6882f},
	{-0.8507f,0.5257f,0.0000f},{-0.8642f,0.4429f,0.2389f},{-0.7166f,0.6817f,0.1476f},
	{-0.6882f,0.5878f,0.4253f},{-0.5000f,0.8090f,0.3090f},{-0.2389f,0.8642f,0.4429f},
	{-0.4253f,0.6882f,0.5878f},{-0.7166f,0.6817f,-0.1476f},{-0.5000f,0.8090f,-0.3090f},
	{-0.5257f,0.8507f,0.0000f},{0.0000f,0.8507f,-0.5257f},{-0.2389f,0.8642f,-0.4429f},
	{0.0000f,0.9554f,-0.2952f},{-0.2629f,0.9511f,-0.1625f},{0.0000f,1.0000f,0.0000f},
	{0.0000f,0.9554f,0.2952f},{-0.2629f,0.9511f,0.1625f},{0.2389f,0.8642f,0.4429f},
	{0.2629f,0.9511f,0.1625f},{0.5000f,0.8090f,0.3090f},{0.2389f,0.8642f,-0.4429f},
	{0.2629f,0.9511f,-0.1625f},{0.5000f,0.8090f,-0.3090f},{0.8507f,0.5257f,0.0000f},
	{0.7166f,0.6817f,0.1476f},{0.7166f,0.6817f,-0.1476f},{0.5257f,0.8507f,0.0000f},
	{0.4253f,0.6882f,0.5878f},{0.8642f,0.4429f,0.2389f},{0.6882f,0.5878f,0.4253f},
	{0.8090f,0.3090f,0.5000f},{0.6817f,0.1476f,0.7166f},{0.5878f,0.4253f,0.6882f},
	{0.9554f,0.2952f,0.0000f},{1.0000f,0.0000f,0.0000f},{0.9511f,0.1625f,0.2629f},
	{0.8507f,-0.5257f,0.0000f},{0.9554f,-0.2952f,0.0000f},{0.8642f,-0.4429f,0.2389f},
	{0.9511f,-0.1625f,0.2629f},{0.8090f,-0.3090f,0.5000f},{0.6817f,-0.1476f,0.7166f},
	{0.8507f,0.0000f,0.5257f},{0.8642f,0.4429f,-0.2389f},{0.8090f,0.3090f,-0.5000f},
	{0.9511f,0.1625f,-0.2629f},{0.5257f,0.0000f,-0.8507f},{0.6817f,0.1476f,-0.7166f},
	{0.6817f,-0.1476f,-0.7166f},{0.8507f,0.0000f,-0.5257f},{0.8090f,-0.3090f,-0.5000f},
	{0.8642f,-0.4429f,-0.2389f},{0.9511f,-0.1625f,-0.2629f},{0.1476f,0.7166f,-0.6817f},
	{0.3090f,0.5000f,-0.8090f},{0.4253f,0.6882f,-0.5878f},{0.4429f,0.2389f,-0.8642f},
	{0.5878f,0.4253f,-0.6882f},{0.6882f,0.5878f,-0.4253f},{-0.1476f,0.7166f,-0.6817f},
	{-0.3090f,0.5000f,-0.8090f},{0.0000f,0.5257f,-0.8507f},{-0.5257f,0.0000f,-0.8507f},
	{-0.4429f,0.2389f,-0.8642f},{-0.2952f,0.0000f,-0.9554f},{-0.1625f,0.2629f,-0.9511f},
	{0.0000f,0.0000f,-1.0000f},{0.2952f,0.0000f,-0.9554f},{0.1625f,0.2629f,-0.9511f},
	{-0.4429f,-0.2389f,-0.8642f},{-0.3090f,-0.5000f,-0.8090f},{-0.1625f,-0.2629f,-0.9511f},
	{0.0000f,-0.8507f,-0.5257f},{-0.1476f,-0.7166f,-0.6817f},{0.1476f,-0.7166f,-0.6817f},
	{0.0000f,-0.5257f,-0.8507f},{0.3090f,-0.5000f,-0.8090f},{0.4429f,-0.2389f,-0.8642f},
	{0.1625f,-0.2629f,-0.9511f},{0.2389f,-0.8642f,-0.4429f},{0.5000f,-0.8090f,-0.3090f},
	{0.4253f,-0.6882f,-0.5878f},{0.7166f,-0.6817f,-0.1476f},{0.6882f,-0.5878f,-0.4253f},
	{0.5878f,-0.4253f,-0.6882f},{0.0000f,-0.9554f,-0.2952f},{0.0000f,-1.0000f,0.0000f},
	{0.2629f,-0.9511f,-0.1625f},{0.0000f,-0.8507f,0.5257f},{0.0000f,-0.9554f,0.2952f},
	{0.2389f,-0.8642f,0.4429f},{0.2629f,-0.9511f,0.1625f},{0.5000f,-0.8090f,0.3090f},
	{0.7166f,-0.6817f,0.1476f},{0.5257f,-0.8507f,0.0000f},{-0.2389f,-0.8642f,-0.4429f},
	{-0.5000f,-0.8090f,-0.3090f},{-0.2629f,-0.9511f,-0.1625f},{-0.8507f,-0.5257f,0.0000f},
	{-0.7166f,-0.6817f,-0.1476f},{-0.7166f,-0.6817f,0.1476f},{-0.5257f,-0.8507f,0.0000f},
	{-0.5000f,-0.8090f,0.3090f},{-0.2389f,-0.8642f,0.4429f},{-0.2629f,-0.9511f,0.1625f},
	{-0.8642f,-0.4429f,0.2389f},{-0.8090f,-0.3090f,0.5000f},{-0.6882f,-0.5878f,0.4253f},
	{-0.6817f,-0.1476f,0.7166f},{-0.4429f,-0.2389f,0.8642f},{-0.5878f,-0.4253f,0.6882f},
	{-0.3090f,-0.5000f,0.8090f},{-0.1476f,-0.7166f,0.6817f},{-0.4253f,-0.6882f,0.5878f},
	{-0.1625f,-0.2629f,0.9511f},{0.4429f,-0.2389f,0.8642f},{0.1625f,-0.2629f,0.9511f},
	{0.3090f,-0.5000f,0.8090f},{0.1476f,-0.7166f,0.6817f},{0.0000f,-0.5257f,0.8507f},
	{0.4253f,-0.6882f,0.5878f},{0.5878f,-0.4253f,0.6882f},{0.6882f,-0.5878f,0.4253f},
	{-0.9554f,0.2952f,0.0000f},{-0.9511f,0.1625f,0.2629f},{-1.0000f,0.0000f,0.0000f},
	{-0.8507f,0.0000f,0.5257f},{-0.9554f,-0.2952f,0.0000f},{-0.9511f,-0.1625f,0.2629f},
	{-0.8642f,0.4429f,-0.2389f},{-0.9511f,0.1625f,-0.2629f},{-0.8090f,0.3090f,-0.5000f},
	{-0.8642f,-0.4429f,-0.2389f},{-0.9511f,-0.1625f,-0.2629f},{-0.8090f,-0.3090f,-0.5000f},
	{-0.6817f,0.1476f,-0.7166f},{-0.6817f,-0.1476f,-0.7166f},{-0.8507f,0.0000f,-0.5257f},
	{-0.6882f,0.5878f,-0.4253f},{-0.5878f,0.4253f,-0.6882f},{-0.4253f,0.6882f,-0.5878f},
	{-0.4253f,-0.6882f,-0.5878f},{-0.5878f,-0.4253f,-0.6882f},{-0.6882f,-0.5878f,-0.4253f}
	};

FILE *file;
IDPOHeader hdr;
IDPO6InfoHeader header;
Model *model;

//loading simple and group skins
RGB *skin;
/*
bool loadIDPOSkin(void)
{
	for(unsigned y=0;y<header.skinheight;y++)
		for(unsigned x=0;x<header.skinwidth;x++)
		{
			int index=fgetc(file);
			if (index!=EOF)
				skin[y*header.skinwidth+x]=quake_palette[index];
			else
			{
				free(skin);
				fclose(file);
				fprintf(stderr,"Could not read skin.\n");
				return false;
			}
		}
	if (!model->addSkin(header.skinwidth,header.skinheight,skin))
	{
		free(skin);
		fclose(file);
		return false;
	}
	return true;
}*/
bool loadIDPOSkin(unsigned width,unsigned height)
{
	for(unsigned y=0;y<header.skinheight;y++)
		for(unsigned x=0;x<header.skinwidth;x++)
		{
			int index=fgetc(file);
			if (index!=EOF)
				skin[y*width+x]=quake_palette[index];
			else
			{
				free(skin);
				fclose(file);
				fprintf(stderr,"Could not read skin.\n");
				return false;
			}
		}
	if (!model->addSkin(width,height,skin))
	{
		free(skin);
		fclose(file);
		return false;
	}
	return true;
}
unsigned long nb;
bool loadIDPOGroupSkinHeader(void)
{
	if (fread(&nb,sizeof(nb),1,file)!=1)
	{
		free(skin);
		fclose(file);
		fprintf(stderr,"Could not read nb field.\n");
		return false;
	}
	//fprintf(stdout,"nb\t%d\n",nb);
	for(unsigned j=0;j<nb;j++)
	{
		float time;
		if (fread(&time,sizeof(time),1,file)!=1)
		{
			free(skin);
			fclose(file);
			fprintf(stderr,"Could not read time field.\n");
			return false;
		}
		//fprintf(stdout,"time\t%f\n",time);
	}
	return true;
}
//loading simple and group frames
Vector3D *frame_vertices;
Vector3D *frame_normals;
bool loadIDPOFrame(void)
{
	unsigned frame_header_size=sizeof(IDPO6FrameHeader);
	IDPO6FrameHeader frame_header;
	memset(&frame_header,0,frame_header_size);
	if (hdr.version==3)
		frame_header_size=sizeof(IDPO3FrameHeader);
	if (fread(&frame_header,frame_header_size,1,file)!=1)
	{
		free(frame_normals);
		free(frame_vertices);
		fclose(file);
		fprintf(stderr,"Could not read frame header.\n");
		return false;
	}
	for(unsigned vertex_index=0;vertex_index<header.numverts;vertex_index++)
	{
		IDPOVertex vertex;
		if (fread(&vertex,sizeof(IDPOVertex),1,file)!=1)
		{
			free(frame_normals);
			free(frame_vertices);
			fclose(file);
			fprintf(stderr,"Could not read frame vertex.\n");
			return false;
		}
		frame_vertices[vertex_index].x=vertex.packedposition[0]*header.scale.x+header.origin.x;
		frame_vertices[vertex_index].y=vertex.packedposition[1]*header.scale.y+header.origin.y;
		frame_vertices[vertex_index].z=vertex.packedposition[2]*header.scale.z+header.origin.z;
		frame_normals[vertex_index]=quake_normals[vertex.lightnormalindex];
	}
	Vector3D min;
	Vector3D max;
	min.x=frame_header.min.packedposition[0]*header.scale.x+header.origin.x;
	min.y=frame_header.min.packedposition[1]*header.scale.y+header.origin.y;
	min.z=frame_header.min.packedposition[2]*header.scale.z+header.origin.z;
	max.x=frame_header.max.packedposition[0]*header.scale.x+header.origin.x;
	max.y=frame_header.max.packedposition[1]*header.scale.y+header.origin.y;
	max.z=frame_header.max.packedposition[2]*header.scale.z+header.origin.z;
	if (model->addFrame(frame_header.name,min,max,header.numverts,frame_vertices,frame_normals)==-1)
	{
		free(frame_normals);
		free(frame_vertices);
		fclose(file);
		return false;
	}
	return true;
}
bool loadIDPOGroupFrameHeader(void)
{
	if (fread(&nb,sizeof(nb),1,file)!=1)
	{
		fprintf(stderr,"Could not read nb field.\n");
		free(frame_normals);
		free(frame_vertices);
		fclose(file);
		return false;
	}
	IDPOVertex min;
	if (fread(&min,sizeof(min),1,file)!=1)
	{
		free(frame_normals);
		free(frame_vertices);
		fclose(file);
		fprintf(stderr,"Could not read group frame min field.\n");
		return false;
	}
	IDPOVertex max;
	if (fread(&max,sizeof(max),1,file)!=1)
	{
		free(frame_normals);
		free(frame_vertices);
		fclose(file);
		fprintf(stderr,"Could not read group frame max field.\n");
		return false;
	}
	//fprintf(stdout,"nb\t%d\n",nb);
	for(unsigned j=0;j<nb;j++)
	{
		float time;
		if (fread(&time,sizeof(time),1,file)!=1)
		{
			free(frame_normals);
			free(frame_vertices);
			fclose(file);
			fprintf(stderr,"Could not read time field.\n");
			return false;
		}
		//fprintf(stdout,"time\t%f\n",time);
	}
	return true;
}
bool loadIDPO(Model *mdl,const char *file_name)
{
	model=mdl;
//open file
	if ((file=fopen(file_name,"rb"))==NULL)
	{
		fprintf(stderr,"Could not open file %s.\n",file_name);
		return false;
	}
//read and chek header
	if (fread(&hdr,sizeof(hdr),1,file)!=1)
	{
		fclose(file);
		fprintf(stderr,"Could not read header.\n");
		return false;
	}
	if (hdr.id!=IDPO_IDENT)
	{
		fclose(file);
		fprintf(stderr,"Is not a MDL file of Quake.\n");
		return false;
	}
	if (hdr.version!=3 && hdr.version!=6)
	{
		fclose(file);
		fprintf(stderr,"Not supported version of MDL file of Quake (must be 3 or 6).\n");
		return false;
	}
	unsigned header_size=sizeof(IDPO6InfoHeader);
	memset(&header,0,header_size);
	if (hdr.version==3)
		header_size=sizeof(IDPO3InfoHeader);
	if (fread(&header,header_size,1,file)!=1)
	{
		fclose(file);
		fprintf(stderr,"Could not read header.\n");
		return false;
	}
//read skins
/*
	if ((skin=(RGB *)malloc(header.skinwidth*header.skinheight*sizeof(RGB)))==NULL)
	{
		fclose(file);
		fprintf(stderr,"Could not get memory for skin.\n");
		return false;
	}
	memset(skin,0,header.skinwidth*header.skinheight*sizeof(RGB));
	for(unsigned i=0;i<header.numskins;i++)
	{
		unsigned long group;
		if (fread(&group,sizeof(group),1,file)!=1)
		{
			free(skin);
			fclose(file);
			fprintf(stderr,"Could not read group field.\n");
			return false;
		}
		//fprintf(stdout,"group\t%d\n",group);
		if (group==0)
			if (!loadIDPOSkin())
				return false;
			else;
		else if (group==1)
		{
			if (!loadIDPOGroupHeader())
			{
				free(skin);
				fclose(file);
				return false;
			}
			for(unsigned j=0;j<nb;j++)
				if (!loadIDPOSkin())
					return false;
		}
		else
		{
			free(skin);
			fclose(file);
			fprintf(stderr,"Unknown group identifier %d.\n",group);
			return false;
		}
	}
	free(skin);
*/
	unsigned width=1;
	unsigned height=1;
	while (width<header.skinwidth)
		width<<=1;
	while (height<header.skinheight)
		height<<=1;
	if ((skin=(RGB *)malloc(width*height*sizeof(RGB)))==NULL)
	{
		fclose(file);
		fprintf(stderr,"Could not get memory for skin.\n");
		return false;
	}
	memset(skin,0,width*height*sizeof(RGB));
	for(unsigned i=0;i<header.numskins;i++)
	{
		unsigned long group;
		if (fread(&group,sizeof(group),1,file)!=1)
		{
			free(skin);
			fclose(file);
			fprintf(stderr,"Could not read group field.\n");
			return false;
		}
		//fprintf(stdout,"group\t%d\n",group);
		if (group==0)
			if (!loadIDPOSkin(width,height))
				return false;
			else;
		else if (group==1)
		{
			if (!loadIDPOGroupSkinHeader())
				return false;
			for(unsigned j=0;j<nb;j++)
				if (!loadIDPOSkin(width,height))
					return false;
		}
		else
		{
			free(skin);
			fclose(file);
			fprintf(stderr,"Unknown group identifier %d.\n",group);
			return false;
		}
	}
	free(skin);
//read points	
	unsigned num_st_verts;
	IDPOPoint *st_verts;
	if ((st_verts=(IDPOPoint *)malloc(header.numverts*sizeof(IDPOPoint)))==NULL)
	{
		fclose(file);
		fprintf(stderr,"Could not get memory for stverts.\n");
		return false;
	}
	if (fread(st_verts,header.numverts*sizeof(IDPOPoint),1,file)!=1)
	{
		free(st_verts);
		fclose(file);
		fprintf(stderr,"Could not read st_verts.\n");
		return false;
	}
	num_st_verts=header.numverts;
//read triangles
	unsigned num_tris;
	IDPOTriangle *tris;
	if ((tris=(IDPOTriangle *)malloc(header.numtris*sizeof(IDPOTriangle)))==NULL)
	{
		free(st_verts);
		fclose(file);
		fprintf(stderr,"Could not get memory for triangles.\n");
		return false;
	}
	if (fread(tris,header.numtris*sizeof(IDPOTriangle),1,file)!=1)
	{
		free(st_verts);
		free(tris);
		fclose(file);
		fprintf(stderr,"Could not read triangles.\n");
		return false;
	}
	num_tris=header.numtris;
//convert st_verts to points and tris to triangles
	//float f_width=(float)header.skinwidth;
	//float f_height=(float)header.skinheight;
	float f_width=(float)width;
	float f_height=(float)height;
	unsigned onseam=IDPO6_ON_SEAM;
	if (hdr.version==3)
		onseam=IDPO3_ON_SEAM;
	for(i=0;i<num_tris;i++)
	{
		int point_indexes[3];
		for(unsigned j=0;j<3;j++)
		{
			unsigned vertex_index=tris[i].vertices[j];
			float s=(float)st_verts[vertex_index].s/f_width;
			float t=(float)st_verts[vertex_index].t/f_height;
			if ((!tris[i].facesfront)&&(st_verts[vertex_index].onseam==onseam))
				//s+=0.5f;
				s+=(header.skinwidth>>1)/f_width;
			point_indexes[j]=model->addPoint(s,t,vertex_index);
		}
		if (model->addTriangle(point_indexes[0],point_indexes[1],point_indexes[2])==-1)
		{
			free(st_verts);
			free(tris);
			fclose(file);
			return false;
		}
	}
	free(st_verts);
	free(tris);
//read frames
	if ((frame_vertices=(Vector3D *)malloc(header.numverts*sizeof(Vector3D)))==NULL)
	{
		fclose(file);
		fprintf(stderr,"Could not get memory for frame vertices.\n");
		return false;
	}
	if ((frame_normals=(Vector3D *)malloc(header.numverts*sizeof(Vector3D)))==NULL)
	{
		free(frame_vertices);
		fclose(file);
		fprintf(stderr,"Could not get memory for frame normals.\n");
		return false;
	}
	for(i=0;i<header.numframes;i++)
	{
		unsigned long group;
		if (fread(&group,sizeof(group),1,file)!=1)
		{
			free(frame_normals);
			free(frame_vertices);
			fclose(file);
			fprintf(stderr,"Could not read group field.\n");
			return false;
		}
		//fprintf(stdout,"group\t%d\n",group);
		if (group==0)
			if (!loadIDPOFrame())
				return false;
			else;
		else if (group==1)
		{
			if (!loadIDPOGroupFrameHeader())
				return false;
			for(unsigned j=0;j<nb;j++)
				if (!loadIDPOFrame())
					return false;
		}
		else
		{
			free(frame_normals);
			free(frame_vertices);
			fclose(file);
			fprintf(stderr,"Unknown group identifier %d.\n",group);
			return false;
		}
	}
	free(frame_normals);
	free(frame_vertices);
	fclose(file);
//and last...
//calculate normals and bound boxes
	if (!model->calculateNormals())
		return false;
	model->calculateBoundBoxes();
//verify model
	if (!model->verifyModel())
	{
		fprintf(stderr,"Warning: Model has wrong indexes.\n");
		return false;
	}
//dump loaded model
	//model->printInfo();
	return true;
}
