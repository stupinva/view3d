#include <GL/gl.h>
#include "model.h"

void Model::DrawModel(unsigned lighting,unsigned frame)
{
	if (frame<num_frames && num_triangles && num_vertices)
	{
		Vector3D *frame_vertex_positions=&vertex_positions[frame*num_vertices];
		Vector3D *frame_vertex_normals=&vertex_normals[frame*num_vertices];
		Vector3D *frame_triangle_normals=&triangle_normals[frame*num_triangles];
		unsigned i;
		glBegin(GL_TRIANGLES);
		switch (lighting)
		{
		case 0:
			for(i=0;i<num_triangles;i++)
			{
				for(unsigned j=0;j<3;j++)
					glVertex3fv(frame_vertex_positions[triangles[i].vertex_indexes[j]]);
			}
			break;
		case 1:
			for(i=0;i<num_triangles;i++)
			{
				glNormal3fv(frame_triangle_normals[i]);
				for(unsigned j=0;j<3;j++)
					glVertex3fv(frame_vertex_positions[triangles[i].vertex_indexes[j]]);
			}
			break;
		case 2:
			for(i=0;i<num_triangles;i++)
			{
				for(unsigned j=0;j<3;j++)
				{
					unsigned vertex_index=triangles[i].vertex_indexes[j];
					glNormal3fv(frame_vertex_normals[vertex_index]);
					glVertex3fv(frame_vertex_positions[vertex_index]);
				}
			}
			break;
		}
		glEnd();
	}
}
