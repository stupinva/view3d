#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vmdl.h"
#include "idpo.h"
/*
const unsigned MAX_NAME	=16;
char model_name[MAX_NAME];
struct Skin
{
	char name[MAX_NAME];
};
const unsigned MAX_SKINS	=16;
unsigned num_skins;
Skin skins[MAX_SKINS];

const unsigned MAX_STR	=1024;
bool dump_skins(FILE *idpo_file,unsigned skinwidth,unsigned skinheight,unsigned numskins)
{
	fprintf(stdout,"---skins---\n");
	for(unsigned i=0;i<numskins;i++)
	{
		unsigned long group;
		if (fread(&group,sizeof(group),1,idpo_file)!=1)
		{
			fclose(idpo_file);
			fprintf(stderr,"Could not read group field.\n");
			return false;
		}
		fprintf(stdout,"group\t%d\n",group);
		char skin_name[MAX_STR];
		charImage_t image;
		if (group==0)
		{
			if (num_skins>=MAX_SKINS)
			{
				fclose(idpo_file);
				fprintf(stderr,"Too many skins.\n");
				return false;
			}
			sprintf(skins[num_skins].name,"%s_%d",model_name,i);
			sprintf(skin_name,"%s.tga",skins[num_skins].name);
			if (image.create(skinwidth,skinheight))
			{
				fclose(idpo_file);
				fprintf(stderr,"Could not get memory.\n");
				return false;
			}
			image.setPalette(quake_palette,0,255);
			for(unsigned y=0;y<skinheight;y++)
				for(unsigned x=0;x<skinwidth;x++)
				{
					int index=fgetc(idpo_file);
					if (index!=EOF)
						image.putPixel(x,y,index);
				}
			image.saveTARGA(skin_name);
			num_skins++;
		}
		if (group==1)
		{
			unsigned long nb;
			if (fread(&nb,sizeof(nb),1,idpo_file)!=1)
			{
				fclose(idpo_file);
				fprintf(stderr,"Could not read nb field.\n");
				return false;
			}
			fprintf(stdout,"nb\t%d\n",nb);
			if (num_skins+nb>=MAX_SKINS)
			{
				fclose(idpo_file);
				fprintf(stderr,"Too many skins.\n");
				return false;
			}
			for(unsigned j=0;j<nb;j++)
			{
				float time;
				if (fread(&time,sizeof(time),1,idpo_file)!=1)
				{
					fclose(idpo_file);
					fprintf(stderr,"Could not read time field.\n");
					return false;
				}
				fprintf(stdout,"time\t%f\n",time);
			}
			for(j=0;j<nb;j++)
			{

				sprintf(skins[num_skins].name,"%s_%d_%d",model_name,i,j);
				sprintf(skin_name,"%s.tga",skins[num_skins].name);
				if (image.create(skinwidth,skinheight))
				{
					fclose(idpo_file);
					fprintf(stderr,"Could not get memory.\n");
					return false;
				}
				image.setPalette(quake_palette,0,255);
				for(unsigned y=0;y<skinheight;y++)
					for(unsigned x=0;x<skinwidth;x++)
					{
						int index=fgetc(idpo_file);
						if (index!=EOF)
							image.putPixel(x,y,index);
					}
				image.saveTARGA(skin_name);
				num_skins++;
			}
		}
	}
	return true;
}
*/
FILE *idpo_file;
IDPOHeader idpo_header;
Model model;

bool dump_skins(void)
{
	//fprintf(stdout,"---skins---\n");
	RGB *skin;
	if ((skin=(RGB *)malloc(idpo_header.skinwidth*idpo_header.skinheight*sizeof(RGB)))==NULL)
	{
		fclose(idpo_file);
		fprintf(stderr,"Could not get memory for skin.\n");
		return false;
	}

	for(unsigned i=0;i<(unsigned)idpo_header.numskins;i++)
	{
		unsigned long group;
		if (fread(&group,sizeof(group),1,idpo_file)!=1)
		{
			free(skin);
			fclose(idpo_file);
			fprintf(stderr,"Could not read group field.\n");
			return false;
		}
		//fprintf(stdout,"group\t%d\n",group);
		if (group==0)
		{
			for(unsigned y=0;y<(unsigned)idpo_header.skinheight;y++)
				for(unsigned x=0;x<(unsigned)idpo_header.skinwidth;x++)
				{
					int index=fgetc(idpo_file);
					if (index!=EOF)
						skin[y*idpo_header.skinwidth+x]=quake_palette[index];
					else
						{
							free(skin);
							fclose(idpo_file);
							fprintf(stderr,"Could not read skin.\n");
							return false;
						}
				}
			model.addSkin(idpo_header.skinwidth,idpo_header.skinheight,skin);
		}
		if (group==1)
		{
			unsigned long nb;
			if (fread(&nb,sizeof(nb),1,idpo_file)!=1)
			{
				free(skin);
				fclose(idpo_file);
				fprintf(stderr,"Could not read nb field.\n");
				return false;
			}
			//fprintf(stdout,"nb\t%d\n",nb);
			for(unsigned j=0;j<nb;j++)
			{
				float time;
				if (fread(&time,sizeof(time),1,idpo_file)!=1)
				{
					fclose(idpo_file);
					fprintf(stderr,"Could not read time field.\n");
					return false;
				}
				//fprintf(stdout,"time\t%f\n",time);
			}
			for(j=0;j<nb;j++)
			{
				for(unsigned y=0;y<(unsigned)idpo_header.skinheight;y++)
					for(unsigned x=0;x<(unsigned)idpo_header.skinwidth;x++)
					{
						int index=fgetc(idpo_file);
						if (index!=EOF)
							skin[y*idpo_header.skinwidth+x]=quake_palette[index];
						else
						{
							free(skin);
							fclose(idpo_file);
							fprintf(stderr,"Could not read skin.\n");
							return false;
						}
					}
				model.addSkin(idpo_header.skinwidth,idpo_header.skinheight,skin);
			}
		}
	}
	free(skin);
	return true;
}

unsigned num_idpo_points=0;
IDPOPoint *idpo_points;

bool dump_points(void)
{
	//fprintf(stdout,"---stverts---\n");
	if ((idpo_points=(IDPOPoint *)malloc(idpo_header.numverts*sizeof(IDPOPoint)))==NULL)
	{
		fclose(idpo_file);
		fprintf(stderr,"Could not get memory for stverts.\n");
		return false;
	}
	if (fread(idpo_points,idpo_header.numverts*sizeof(IDPOPoint),1,idpo_file)!=1)
	{
		free(idpo_points);
		fclose(idpo_file);
		fprintf(stderr,"Could not read stverts.\n");
		return false;
	}
	num_idpo_points=idpo_header.numverts;
	return true;
}

unsigned num_idpo_triangles=0;
IDPOTriangle *idpo_triangles;

bool dump_triangles(void)
{
	//fprintf(stdout,"---itriangles---\n");
	if ((idpo_triangles=(IDPOTriangle *)malloc(idpo_header.numtris*sizeof(IDPOTriangle)))==NULL)
	{
		fclose(idpo_file);
		fprintf(stderr,"Could not get memory for triangles.\n");
		return false;
	}
	if (fread(idpo_triangles,idpo_header.numtris*sizeof(IDPOTriangle),1,idpo_file)!=1)
	{
		free(idpo_triangles);
		fclose(idpo_file);
		fprintf(stderr,"Could not read triangles.\n");
		return false;
	}
	num_idpo_triangles=idpo_header.numtris;
	return true;
}

bool convert(void)
{
	for(unsigned i=0;i<num_idpo_triangles;i++)
	{
		int point_indexes[3];
		for(unsigned j=0;j<3;j++)
		{
			unsigned vertex_index=idpo_triangles[i].vertices[j];
			unsigned s=idpo_points[vertex_index].s;
			unsigned t=idpo_points[vertex_index].t;
			if ((!idpo_triangles[i].facesfront)&&(idpo_points[vertex_index].onseam))
				s+=idpo_header.skinwidth>>1;
			point_indexes[j]=model.addPoint(s,t,vertex_index);
		}
		model.addTriangle(point_indexes[0],point_indexes[1],point_indexes[2]);
	}
	return true;
}

bool dump_frames(void)
{
	//fprintf(stdout,"---frames---\n");
	Vector3D *vertices;
	Vector3D *normals;
	if ((vertices=(Vector3D *)malloc(idpo_header.numverts*sizeof(Vector3D)))==NULL)
	{
		fclose(idpo_file);
		fprintf(stderr,"Could not get memory for frame vertices.\n");
		return false;
	}
	if ((normals=(Vector3D *)malloc(idpo_header.numverts*sizeof(Vector3D)))==NULL)
	{
		free(vertices);
		fclose(idpo_file);
		fprintf(stderr,"Could not get memory for frame normals.\n");
		return false;
	}

	for(unsigned i=0;i<(unsigned)idpo_header.numframes;i++)
	{
		unsigned long group;
		if (fread(&group,sizeof(group),1,idpo_file)!=1)
		{
			free(normals);
			free(vertices);
			fclose(idpo_file);
			fprintf(stderr,"Could not read group field.\n");
			return false;
		}
		//fprintf(stdout,"group\t%d\n",group);
		if (group==0)
		{
			IDPOFrameHeader idpo_frame_header;
			if (fread(&idpo_frame_header,sizeof(IDPOFrameHeader),1,idpo_file)!=1)
			{
				free(normals);
				free(vertices);
				fclose(idpo_file);
				fprintf(stderr,"Could not read frame header.\n");
				return false;
			}
			for(unsigned vertex_index=0;vertex_index<(unsigned)idpo_header.numverts;vertex_index++)
			{
				IDPOVertex vertex;
				if (fread(&vertex,sizeof(IDPOVertex),1,idpo_file)!=1)
				{
					free(normals);
					free(vertices);
					fclose(idpo_file);
					fprintf(stderr,"Could not read frame vertex.\n");
					return false;
				}
				vertices[vertex_index].x=vertex.packedposition[0]*idpo_header.scale.x+idpo_header.origin.x;
				vertices[vertex_index].y=vertex.packedposition[1]*idpo_header.scale.y+idpo_header.origin.y;
				vertices[vertex_index].z=vertex.packedposition[2]*idpo_header.scale.z+idpo_header.origin.z;
				normals[vertex_index]=quake_normals[vertex.lightnormalindex];
			}
			Vector3D min;
			Vector3D max;
			min.x=idpo_frame_header.min.packedposition[0]*idpo_header.scale.x+idpo_header.origin.x;
			min.y=idpo_frame_header.min.packedposition[1]*idpo_header.scale.y+idpo_header.origin.y;
			min.z=idpo_frame_header.min.packedposition[2]*idpo_header.scale.z+idpo_header.origin.z;
			max.x=idpo_frame_header.max.packedposition[0]*idpo_header.scale.x+idpo_header.origin.x;
			max.y=idpo_frame_header.max.packedposition[1]*idpo_header.scale.y+idpo_header.origin.y;
			max.z=idpo_frame_header.max.packedposition[2]*idpo_header.scale.z+idpo_header.origin.z;
			model.addFrame(idpo_frame_header.name,min,max,idpo_header.numverts,vertices,normals);
		}
		if (group==1)
		{
			unsigned long nb;
			if (fread(&nb,sizeof(nb),1,idpo_file)!=1)
			{
				free(normals);
				free(vertices);
				fclose(idpo_file);
				fprintf(stderr,"Could not read nb field.\n");
				return false;
			}
			//fprintf(stdout,"nb\t%d\n",nb);
			for(unsigned j=0;j<nb;j++)
			{
				float time;
				if (fread(&time,sizeof(time),1,idpo_file)!=1)
				{
					free(normals);
					free(vertices);
					fclose(idpo_file);
					fprintf(stderr,"Could not read time field.\n");
					return false;
				}
				//fprintf(stdout,"time\t%f\n",time);
			}
			for(j=0;j<nb;j++)
			{
				IDPOFrameHeader idpo_frame_header;
				if (fread(&idpo_frame_header,sizeof(idpo_frame_header),1,idpo_file)!=1)
				{
					free(normals);
					free(vertices);
					fclose(idpo_file);
					fprintf(stderr,"Could not read frame header.\n");
					return false;
				}
				for(unsigned vertex_index=0;vertex_index<(unsigned)idpo_header.numverts;vertex_index++)
				{
					IDPOVertex vertex;
					if (fread(&vertex,sizeof(vertex),1,idpo_file)!=1)
					{
						free(normals);
						free(vertices);
						fclose(idpo_file);
						fprintf(stderr,"Could not read frame vertex.\n");
						return false;
					}
					vertices[vertex_index].x=vertex.packedposition[0]*idpo_header.scale.x+idpo_header.origin.x;
					vertices[vertex_index].y=vertex.packedposition[1]*idpo_header.scale.y+idpo_header.origin.y;
					vertices[vertex_index].z=vertex.packedposition[2]*idpo_header.scale.z+idpo_header.origin.z;
					normals[vertex_index]=quake_normals[vertex.lightnormalindex];
				}
				Vector3D min;
				Vector3D max;
				min.x=idpo_frame_header.min.packedposition[0]*idpo_header.scale.x+idpo_header.origin.x;
				min.y=idpo_frame_header.min.packedposition[1]*idpo_header.scale.y+idpo_header.origin.y;
				min.z=idpo_frame_header.min.packedposition[2]*idpo_header.scale.z+idpo_header.origin.z;
				max.x=idpo_frame_header.max.packedposition[0]*idpo_header.scale.x+idpo_header.origin.x;
				max.y=idpo_frame_header.max.packedposition[1]*idpo_header.scale.y+idpo_header.origin.y;
				max.z=idpo_frame_header.max.packedposition[2]*idpo_header.scale.z+idpo_header.origin.z;
				model.addFrame(idpo_frame_header.name,min,max,idpo_header.numverts,vertices,normals);
			}
		}
	}
	free(normals);
	free(vertices);
	return true;
}

void main(int carg,char **varg)
{
	if (carg!=2)
	{
		fprintf(stderr,"You must specify name of MDL file of Quake.\n");
		return;
	}
	char *file_name=varg[1];
	if ((idpo_file=fopen(file_name,"rb"))==NULL)
	{
		fprintf(stderr,"Could not open file %s.\n",file_name);
		return;
	}
	
	if (fread(&idpo_header,sizeof(idpo_header),1,idpo_file)!=1)
	{
		fclose(idpo_file);
		fprintf(stderr,"Could not read header.\n");
		return;
	}
	if (idpo_header.id!=IDPO_IDENT)
	{
		fclose(idpo_file);
		fprintf(stderr,"Is not a MDL file of Quake.\n");
		return;
	}
	if (idpo_header.version!=6)
	{
		fclose(idpo_file);
		fprintf(stderr,"Not supported version of MDL file of Quake.\n");
		return;
	}
	/*
	fprintf(stdout,
		"scale\t\t%f\t%f\t%f\n"
		"origin\t\t%f\t%f\t%f\n"
		"radius\t\t%f\n"
		"offsets\t\t%f\t%f\t%f\n"
		"numskins\t%d\n"
		"skinwidth\t%d\n"
		"skinheight\t%d\n"
		"numverts\t%d\n"
		"numtris\t\t%d\n"
		"numframes\t%d\n"
		"synctype\t%d\n"
		"flags\t\t%d\n"
		"size\t\t%f\n",
		idpo_header.scale.x,idpo_header.scale.y,idpo_header.scale.z,
		idpo_header.origin.x,idpo_header.origin.y,idpo_header.origin.z,
		idpo_header.radius,
		idpo_header.offsets.x,idpo_header.offsets.y,idpo_header.offsets.z,
		idpo_header.numskins,
		idpo_header.skinwidth,
		idpo_header.skinheight,
		idpo_header.numverts,
		idpo_header.numtris,
		idpo_header.numframes,
		idpo_header.synctype,
		idpo_header.flags,
		idpo_header.size
		);
	*/
	if (!dump_skins())
		return;

	if (!dump_points())
		return;

	if (!dump_triangles())
	{
		if (idpo_points)
			free(idpo_points);
		return;
	}

	convert();
	if (idpo_points)
		free(idpo_points);
	if (idpo_triangles)
		free(idpo_triangles);
	
	if (!dump_frames())
		return;
	
	model.calculateNormals();
	model.calculateBoundBoxes();
	if (!model.verifyModel())
		fprintf(stdout,"Model has wrong indexes.\n");
	model.printInfo();

	fclose(idpo_file);
}
/*
void main(void)
{
	const Vector3D min={0.0f,0.0f,0.0f};
	const Vector3D max={1.0f,1.0f,0.0f};
	const Vector3D vertices[]={{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f}};
	const Vector3D normals[]={{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
	model.addFrame("triangle",min,max,3,vertices,normals);
	model.addTriangle(model.addPoint(0,0,0),
						model.addPoint(0,0,1),
						model.addPoint(0,0,2));
	model.calculateNormals();
	model.printInfo();
}
*/