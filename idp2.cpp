#include <stdio.h>
#include <string.h>
#include "misc.h"
#include "model.h"

/////////////////////////////////////////////////////////////////////////////////////
//04-07-2003                                                                       //
//Загрузка моделей Quake 2 из MD2-файлов, формат с идентификатором IDP2, версия №8.//
/////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)
struct IDP2Header
{
	char ident[4];				//Идентификатор, должен быть "IDP2".
	unsigned long version;		//Версия 8.

	unsigned long skin_width;	//Ширина текстуры.
	unsigned long skin_height;	//Высота текстуры.
	unsigned long frame_size;	//Размер кадра в байтах.

	unsigned long num_skins;	//Количество текстур.
	unsigned long num_vertices;	//Количество вершин.
	unsigned long num_points;	//Количество точек на текстуре.
	unsigned long num_triangles;//Количество треугольников.
	unsigned long num_glcmds;	//Количество двойных слов в списке команд полос/вееров.
	unsigned long num_frames;	//Количество кадров.

	unsigned long ofs_skins;	//Смещение текстур - строк из 64 символов.
	unsigned long ofs_points;	//Смещение массива точек в файле (в байтах).
	unsigned long ofs_triangles;//Смещение треугольников.
	unsigned long ofs_frames;	//Смещение первого кадра.
	unsigned long ofs_glcmds;	//Смещение списка команд полос/вееров.
	unsigned long ofs_end;		//Конец файла.
};
struct IDP2Point
{
	unsigned short s;
	unsigned short t;
};
struct IDP2Triangle
{
	unsigned short vertex_indexes[3];
	unsigned short point_indexes[3];
};
struct IDP2Vertex
{
	unsigned char packed_xyz[3];
	unsigned char light_normal_index;
};
struct IDP2FrameHeader
{
	Vector3D scale;		//Каждую координату упакованной вершины нужно
						//умножить на соответствующую координату этого вектора...
	Vector3D translate;	//и затем прибавить соответствующую координату этого вектора.
	char name[16];
};
#pragma pack(pop)

bool Model::LoadIDP2Model(const char *file_name)
{
	fprintf(stdout,"Loading Quake 2 MD2-file \"%s\".\n",file_name);
	//Открываем файл модели.
	FILE *file;
	if ((file=fopen(file_name,"rb"))==NULL)
	{
		fprintf(stderr,"Could not open file \"%s\".\n",file_name);
		return false;
	}
	//Читаем заголовок, проверяем идентификатор и версию.
	IDP2Header header;
	if (fread(&header,sizeof(header),1,file)!=1)
	{
		fclose(file);
		fprintf(stderr,"Could not read MD2-file header.\n");
		return false;
	}
	if (memcmp(header.ident,"IDP2",sizeof(header.ident)))
	{
		fclose(file);
		fprintf(stderr,"Not a Quake 2 MD2-file.\n");
		return false;
	}
	if (header.version!=8)
	{
		fclose(file);
		fprintf(stderr,"Supports only version #8 of Quake 2 MD2-file.\n");
		return false;
	}
	//Выделяем память подо все массивы модели.
	if (!NewModel(header.num_skins,header.num_points,1,header.num_triangles,header.num_frames,
			header.num_vertices))
	{
		fclose(file);
		return false;
	}
	ExtractName(file_name,name);
	//Грузим текстуры модели.
	fseek(file,header.ofs_skins,SEEK_SET);
	for(unsigned i=0;i<header.num_skins;i++)
	{
		//fprintf(stdout,"%d skins loaded.\r",i);
		char skin_name[64];
		if (fread(&skin_name,sizeof(skin_name),1,file)!=1)
		{
			FreeModel();
			fclose(file);
			fprintf(stderr,"Could not read skin name.\n");
			return false;
		}
		/*if ((skin_indexes=LoadTexture(skin_name))==-1)
		{
			FreeModel();
			fclose(file);
			return false;
		}*/
	}
	fprintf(stdout,"%d skins loaded.\n",num_skins);
	//Грузим точки - координаты вершин на текстуре.
	fseek(file,header.ofs_points,SEEK_SET);
	float scale_width=1.0f/(float)header.skin_width;
	float scale_height=1.0f/(float)header.skin_height;
	for(unsigned i=0;i<header.num_points;i++)
	{
		//fprintf(stdout,"%d points loaded.\r",i);
		IDP2Point point;
		if (fread(&point,sizeof(point),1,file)!=1)
		{
			FreeModel();
			fclose(file);
			fprintf(stderr,"Could not read point.\n");
			return false;
		}
		points[i][0]=(float)point.s*scale_width;
		points[i][1]=(float)point.t*scale_height;
	}
	fprintf(stdout,"%d points loaded.\n",num_points);
	//Грузим треугольники.
	strcpy(meshes[0].name,name);
	meshes[0].first_skin=0;
	meshes[0].num_skins=num_skins;

	fseek(file,header.ofs_triangles,SEEK_SET);
	for(unsigned i=0;i<header.num_triangles;i++)
	{
		//fprintf(stdout,"%d triangles loaded.\r",i);
		IDP2Triangle triangle;
		if (fread(&triangle,sizeof(triangle),1,file)!=1)
		{
			FreeModel();
			fclose(file);
			fprintf(stderr,"Could not read triangle.\n");
			return false;
		}
		triangles[i].mesh_index=0;
		for(unsigned j=0;j<3;j++)
		{
			triangles[i].vertex_indexes[j]=triangle.vertex_indexes[j];
			triangles[i].point_indexes[j]=triangle.point_indexes[j];
		}
	}
	fprintf(stdout,"%d triangles loaded.\n",num_triangles);
	//Грузим кадры.
	fseek(file,header.ofs_frames,SEEK_SET);
	for(unsigned i=0;i<header.num_frames;i++)
	{
		fprintf(stderr,"%d frames loaded.   \r",i);
		IDP2FrameHeader frame_header;
		if (fread(&frame_header,sizeof(frame_header),1,file)!=1)
		{
			FreeModel();
			fclose(file);
			fprintf(stderr,"Could not read frame header.\n");
			return false;
		}
		strcpy(frame_names[i],frame_header.name);
		Vector3D *frame_vertex_positions=&vertex_positions[i*num_vertices];
		for(unsigned j=0;j<header.num_vertices;j++)
		{
			IDP2Vertex vertex;
			if (fread(&vertex,sizeof(vertex),1,file)!=1)
			{
				FreeModel();
				fclose(file);
				fprintf(stderr,"Could not read frame vertex.\n");
				return false;
			}
			/*Vector3D *frame_vertex_position=&frame_vertex_positions[j];
			for(unsigned k=0;k<3;k++)
				frame_vertex_position[k]=frame_header.scale[k]*vertex[k]+frame_header.translate[k];
			*/
			for(unsigned k=0;k<3;k++)
				frame_vertex_positions[j][k]=frame_header.scale[k]*(float)vertex.packed_xyz[k]
					+frame_header.translate[k];
		}
	}
	fclose(file);
	fprintf(stdout,"%d frames loaded.   \n",num_frames);
	//Теперь можно посчитать нормали, проверить модель, хлебнуть пивка или ещё что-нибудь...
	fprintf(stderr,"Calculating normals.\r");;
	if (CalculateNormals())
		fprintf(stdout,"Normals calculated. \n");
	else
		FreeModel();
	fprintf(stdout,"Memory usage:\n"
		"%d\tskins\t\t%lu\n"
		"%d\tpoints\t\t%lu\n"
		"%d\tmeshes\t\t%lu\n"
		"%d\ttrianles\t%lu\n"
		"%d\tframes\t\t%lu\n",
		num_skins,num_skins*sizeof(*skin_indexes),
		num_points,num_points*sizeof(*points),
		num_meshes,num_meshes*sizeof(*meshes),
		num_triangles,num_triangles*sizeof(*triangles),
		num_frames,num_frames*(num_triangles*sizeof(*triangle_normals)+
				num_vertices*(sizeof(*vertex_positions)+sizeof(*vertex_normals))));
	//DumpModel();
	return true;
}
