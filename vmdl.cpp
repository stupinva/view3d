#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "vmdl.h"

Vector3D CrossProduct(Vector3D v0,Vector3D v1)
{
	Vector3D v2;
	v2.x=v0.y*v1.z-v0.z*v1.y;
	v2.y=v0.z*v1.x-v0.x*v1.z;
	v2.z=v0.x*v1.y-v0.y*v1.x;
	return v2;
}
Vector3D Sub(Vector3D v0,Vector3D v1)
{
	Vector3D v2;
	v2.x=v0.x-v1.x;
	v2.y=v0.y-v1.y;
	v2.z=v0.z-v1.z;
	return v2;
}
Vector3D Add(Vector3D v0,Vector3D v1)
{
	Vector3D v2;
	v2.x=v0.x+v1.x;
	v2.y=v0.y+v1.y;
	v2.z=v0.z+v1.z;
	return v2;
}
Vector3D Mul(Vector3D v0,float f)
{
	Vector3D v1;
	v1.x=v0.x*f;
	v1.y=v0.y*f;
	v1.z=v0.z*f;
	return v1;
}
Vector3D Div(Vector3D v0,float f)
{
	Vector3D v1;
	v1.x=v0.x/f;
	v1.y=v0.y/f;
	v1.z=v0.z/f;
	return v1;
}
float Length(Vector3D v)
{
	float length=v.x*v.x+v.y*v.y+v.z*v.z;
	length=(float)sqrt(length);
	return length;
}
const float DELTA	=0.00001f;
Vector3D Normalize(Vector3D v0)
{
	Vector3D v1;
	float length=v0.x*v0.x+v0.y*v0.y+v0.z*v0.z;
	length=(float)sqrt(length);
	if (length<DELTA)
		return v0;
	v1.x=v0.x/length;
	v1.y=v0.y/length;
	v1.z=v0.z/length;
	return v1;
}

Model::Model(void)
{
	skin_width=0;
	skin_height=0;
	num_skins=0;
	skins=NULL;

	num_points=0;
	points=NULL;

	num_triangles=0;
	triangles=NULL;

	num_frames=0;
	frames=NULL;
	num_vertices=0;
	vertices=NULL;
	normals=NULL;
}

Model::~Model(void)
{
	if (skins)
		free(skins);
	if (points)
		free(points);
	if (triangles)
		free(triangles);
	if (frames)
		free(frames);
	if (vertices)
		free(vertices);
	if (normals)
		free(normals);
}

bool Model::addSkin(unsigned width,unsigned height,RGB *data)
{
	if (num_skins==0)
	{
		skin_width=width;
		skin_height=height;
	}
	if ((skin_width!=width)||(skin_height!=height))
	{
		fprintf(stderr,"Could not add skin, wrong size.\n");
		return false;
	}
	int i=num_skins++;
	if ((skins=(RGB *)realloc(skins,skin_width*skin_height*num_skins*sizeof(RGB)))==NULL)
	{
		num_skins=0;
		fprintf(stderr,"Not enough memory, all skins lost.\n");
		return false;
	}
	memcpy(&skins[i*skin_width*skin_height],data,skin_width*skin_height*sizeof(RGB));
	return true;
}

int Model::addPoint(float s,float t,unsigned vertex_index)
{
	for(int i=0;(unsigned)i<num_points;i++)
		if ((points[i].s==s)&&(points[i].t==t)&&(points[i].vertex_index==vertex_index))
			return i;
	i=num_points++;
	if ((points=(Point *)realloc(points,num_points*sizeof(Point)))==NULL)
	{
		num_points=0;
		fprintf(stderr,"Not enough memory, all points lost.\n");
		return -1;
	}
	points[i].s=s;
	points[i].t=t;
	points[i].vertex_index=vertex_index;
	return i;
}

int Model::addTriangle(int point_index0,int point_index1,int point_index2)
{
	if ((point_index0==-1)||(point_index1==-1)||(point_index2==-1))
	{
		fprintf(stderr,"Could not add triangle, wrong point(s).\n");
		return -1;
	}
	for(int i=0;(unsigned)i<num_triangles;i++)
	{
		if ((triangles[i].point_indexes[0]==point_index0)&&
			(triangles[i].point_indexes[1]==point_index1)&&
			(triangles[i].point_indexes[2]==point_index2))
			return i;
		if ((triangles[i].point_indexes[0]==point_index1)&&
			(triangles[i].point_indexes[1]==point_index2)&&
			(triangles[i].point_indexes[2]==point_index0))
			return i;
		if ((triangles[i].point_indexes[0]==point_index2)&&
			(triangles[i].point_indexes[1]==point_index0)&&
			(triangles[i].point_indexes[2]==point_index1))
			return i;
	}
	i=num_triangles++;
	if ((triangles=(Triangle *)realloc(triangles,num_triangles*sizeof(Triangle)))==NULL)
	{
		num_triangles=0;
		fprintf(stderr,"Not enough memory, all triangles lost.\n");
		return -1;
	}
	triangles[i].point_indexes[0]=point_index0;
	triangles[i].point_indexes[1]=point_index1;
	triangles[i].point_indexes[2]=point_index2;
	return i;
}

void Model::printInfo(void)
{
	fprintf(stdout,"--- dump model ---\n");
	fprintf(stdout,"skin_width\t%d\n"
					"skin_height\t%d\n"
					"num_skins\t%d\t%d\n"
					"num_points\t%d\t%d\n"
					"num_triangles\t%d\t%d\n"
					"num_vertices\t%d\n"
					"num_frames\t%d\t%d\n",
					skin_width,
					skin_height,
					num_skins,skin_width*skin_height*num_skins*sizeof(RGB),
					num_points,num_points*sizeof(Point),
					num_triangles,num_triangles*sizeof(Triangle),
					num_vertices,
					num_frames,num_frames*(num_vertices*2*sizeof(Vector3D)+sizeof(Frame)));
	/*for(unsigned k=0;k<num_skins;k++)
		for(unsigned j=0;j<skin_height;j++)
			for(unsigned i=0;i<skin_width;i++)
			{
				RGB color=skins[k*skin_height*skin_width+j*skin_width+i];
				fprintf(stdout,"%X %X %X\n",color.red,color.green,color.blue);
			}*/
	for(unsigned i=0;i<num_points;i++)
		fprintf(stdout,"point\t%f\t%f\t%d\n",points[i].s,points[i].t,points[i].vertex_index);
	for(i=0;i<num_triangles;i++)
		fprintf(stdout,"triangle\t%d\t%d\t%d\n",
						triangles[i].point_indexes[0],
						triangles[i].point_indexes[1],
						triangles[i].point_indexes[2]);
	for(i=0;i<num_frames;i++)
	{
		fprintf(stdout,"frame\t%s\n"
						"min\t%f\t%f\t%f\n"
						"max\t%f\t%f\t%f\n",
						frames[i].name,
						frames[i].min.x,frames[i].min.y,frames[i].min.z,
						frames[i].max.x,frames[i].max.y,frames[i].max.z);
		for(unsigned j=0;j<num_vertices;j++)
			fprintf(stdout,"vertex\t%f\t%f\t%f\n"
							"normal\t%f\t%f\t%f\n",
							vertices[i*num_vertices+j].x,
							vertices[i*num_vertices+j].y,
							vertices[i*num_vertices+j].z,
							normals[i*num_vertices+j].x,
							normals[i*num_vertices+j].y,
							normals[i*num_vertices+j].z);
	}
}

int Model::addFrame(const char *frame_name,const Vector3D min,const Vector3D max,
					unsigned numvertices,
					const Vector3D *verts,const Vector3D *norms)
{
	if (num_vertices==0)
		num_vertices=numvertices;
	if (num_vertices!=numvertices)
	{
		fprintf(stderr,"Could not add frame, wrong number of vertices.\n");
		return false;
	}
	unsigned i=num_frames++;
	if ((frames=(Frame *)realloc(frames,num_frames*sizeof(Frame)))==NULL)
	{
		if (vertices)
			free(vertices);
		vertices=NULL;
		if (normals)
			free(normals);
		normals=NULL;
		num_frames=0;
		fprintf(stderr,"Not enough memory, all frames, vertices and normals lost.\n");
		return -1;
	}
	if ((vertices=(Vector3D *)realloc(vertices,num_frames*num_vertices*sizeof(Vector3D)))==NULL)
	{
		free(frames);
		frames=NULL;
		if (normals)
			free(normals);
		normals=NULL;
		num_frames=0;
		fprintf(stderr,"Not enough memory, all frames, vertices and normals lost.\n");
		return -1;
	}
	if ((normals=(Vector3D *)realloc(normals,num_frames*num_vertices*sizeof(Vector3D)))==NULL)
	{
		free(frames);
		frames=NULL;
		free(vertices);
		vertices=NULL;
		num_frames=0;
		fprintf(stderr,"Not enough memory, all frames, vertices and normals lost.\n");
		return -1;
	}
	memset(frames[i].name,0,MAX_FRAME_NAME);
	strncpy(frames[i].name,frame_name,MAX_FRAME_NAME);
	frames[i].min=min;
	frames[i].max=max;
	memcpy(&vertices[i*num_vertices],verts,num_vertices*sizeof(Vector3D));
	memcpy(&normals[i*num_vertices],norms,num_vertices*sizeof(Vector3D));
	return i;
}
void Model::calculateBoundBoxes(void)
{
	for(unsigned j=0;j<num_frames;j++)
	{
		Vector3D min=vertices[j*num_vertices];
		Vector3D max=min;
		for(unsigned i=1;i<num_vertices;i++)
		{
			Vector3D vertex=vertices[j*num_vertices+i];
			if (min.x>vertex.x)
				min.x=vertex.x;
			if (min.y>vertex.y)
				min.y=vertex.y;
			if (min.z>vertex.z)
				min.z=vertex.z;
			if (max.x<vertex.x)
				max.x=vertex.x;
			if (max.y<vertex.y)
				max.y=vertex.y;
			if (max.z<vertex.z)
				max.z=vertex.z;
		}
		frames[j].min=min;
		frames[j].max=max;
	}
}
bool Model::calculateNormals(void)
{
	if (!num_vertices)
	{
		fprintf(stderr,"Not enough memory.\n");
		return false;
	}
	unsigned *normal_numbers;
	if ((normal_numbers=(unsigned *)malloc(num_vertices*sizeof(unsigned)))==NULL)
		return false;
	for(unsigned j=0;j<num_frames;j++)
	{
		for(unsigned i=0;i<num_vertices;i++)
		{
			normals[j*num_vertices+i].x=0;
			normals[j*num_vertices+i].y=0;
			normals[j*num_vertices+i].z=0;
			normal_numbers[i]=0;
		}
		for(i=0;i<num_triangles;i++)
		{
			unsigned vertex_index0=points[triangles[i].point_indexes[0]].vertex_index;
			unsigned vertex_index1=points[triangles[i].point_indexes[1]].vertex_index;
			unsigned vertex_index2=points[triangles[i].point_indexes[2]].vertex_index;

			Vector3D vertex0=vertices[j*num_vertices+vertex_index0];
			Vector3D vertex1=vertices[j*num_vertices+vertex_index1];
			Vector3D vertex2=vertices[j*num_vertices+vertex_index2];
			
			Vector3D u=Sub(vertex1,vertex0);
			Vector3D v=Sub(vertex2,vertex0);
			
			Vector3D triangle_normal=Normalize(CrossProduct(v,u));
			
			normals[j*num_vertices+vertex_index0]=
				Add(normals[j*num_vertices+vertex_index0],triangle_normal);
			normals[j*num_vertices+vertex_index1]=
				Add(normals[j*num_vertices+vertex_index1],triangle_normal);
			normals[j*num_vertices+vertex_index2]=
				Add(normals[j*num_vertices+vertex_index2],triangle_normal);
			
			normal_numbers[vertex_index0]++;
			normal_numbers[vertex_index1]++;
			normal_numbers[vertex_index2]++;
		}
		for(i=0;i<num_vertices;i++)
			if (normal_numbers[i])
				normals[j*num_vertices+i]=
					Normalize(Div(normals[j*num_vertices+i],(float)normal_numbers[i]));
	}
	free(normal_numbers);
	return true;
}

bool Model::verifyModel(void)
{
	for(unsigned i=0;i<num_points;i++)
		if (points[i].vertex_index>=num_vertices)
			return false;
	for(i=0;i<num_triangles;i++)
		if ((triangles[i].point_indexes[0]>=num_points)||
			(triangles[i].point_indexes[1]>=num_points)||
			(triangles[i].point_indexes[2]>=num_points))
			return false;
	return true;
}
unsigned Model::getNumFrames(void)
{
	return num_frames;
}
unsigned Model::getNumSkins(void)
{
	return num_skins;
}
bool Model::drawModel(unsigned skin_index,unsigned frame_index,bool draw_skin,bool lighting)
{
	if (frame_index>=num_frames)
		return false;
	if (skin_index>=num_skins)
		return false;
	Vector3D *frame_vertices=&vertices[frame_index*num_vertices];
	Vector3D *frame_normals=&normals[frame_index*num_vertices];
	if (draw_skin)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		//glDrawPixels(skin_width,skin_height,GL_RGB,GL_UNSIGNED_BYTE,
		//	&skins[skin_width*skin_height*skin_index]);
		glTexImage2D(GL_TEXTURE_2D,0,3,skin_width,skin_height,0,GL_RGB,GL_UNSIGNED_BYTE,
			&skins[skin_width*skin_height*skin_index]);

		//gluBuild2DMipmaps(GL_TEXTURE_2D,3,skin_width,skin_height,GL_RGB,GL_UNSIGNED_BYTE,
		//	&skins[skin_width*skin_height*skin_index]));
		glBegin(GL_TRIANGLES);
		if (lighting)
			for(unsigned i=0;i<num_triangles;i++)
			{
				Triangle triangle=triangles[i];
				for(unsigned j=0;j<3;j++)
				{
					Point point=points[triangle.point_indexes[j]];
					unsigned vertex_index=points[triangle.point_indexes[j]].vertex_index;
					glTexCoord2f(point.s,point.t);
					glNormal3fv((const float *)&frame_normals[vertex_index]);
					glVertex3fv((const float *)&frame_vertices[vertex_index]);
				}
			}
		else
			for(unsigned i=0;i<num_triangles;i++)
			{
				Triangle triangle=triangles[i];
				for(unsigned j=0;j<3;j++)
				{
					Point point=points[triangle.point_indexes[j]];
					unsigned vertex_index=points[triangle.point_indexes[j]].vertex_index;
					glTexCoord2f(point.s,point.t);
					glVertex3fv((const float *)&frame_vertices[vertex_index]);
				}
			}
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLES);
		if (lighting)
			for(unsigned i=0;i<num_triangles;i++)
			{
				Triangle triangle=triangles[i];
				for(unsigned j=0;j<3;j++)
				{
					unsigned vertex_index=points[triangle.point_indexes[j]].vertex_index;
					glNormal3fv((const float *)&frame_normals[vertex_index]);
					glVertex3fv((const float *)&frame_vertices[vertex_index]);
				}
			}
		else
			for(unsigned i=0;i<num_triangles;i++)
			{
				Triangle triangle=triangles[i];
				for(unsigned j=0;j<3;j++)
				{
					unsigned vertex_index=points[triangle.point_indexes[j]].vertex_index;
					glVertex3fv((const float *)&frame_vertices[vertex_index]);
				}
			}
		glEnd();
	}
	return true;
}
bool Model::drawNormals(unsigned frame_index)
{
	if (frame_index>=num_frames)
		return false;

	Vector3D *frame_vertices=&vertices[frame_index*num_vertices];
	Vector3D *frame_normals=&normals[frame_index*num_vertices];
	glBegin(GL_LINES);
	for(unsigned i=0;i<num_vertices;i++)
	{
		Vector3D vertex=Add(frame_vertices[i],frame_normals[i]);
		glVertex3fv((const float *)&frame_vertices[i]);
		glVertex3fv((const float *)&vertex);
	}
	glEnd();
	return true;
}
void Model::getMinMax(Vector3D *min,Vector3D *max)
{
	min->x=0;
	min->y=0;
	min->z=0;
	max->x=0;
	max->y=0;
	max->z=0;
	for(unsigned i=0;i<num_frames;i++)
	{
		if (min->x>frames[i].min.x)
			min->x=frames[i].min.x;
		if (min->y>frames[i].min.y)
			min->y=frames[i].min.y;
		if (min->z>frames[i].min.z)
			min->z=frames[i].min.z;
		if (max->x<frames[i].max.x)
			max->x=frames[i].max.x;
		if (max->y<frames[i].max.y)
			max->y=frames[i].max.y;
		if (max->z<frames[i].max.z)
			max->z=frames[i].max.z;
	}
}