#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include "vmdl.h"

#pragma warning(disable:4244)

const float ZBUFFER_MIN	=10.0f;
const float M_PI		=3.14159265f;

float zbuffer_min=ZBUFFER_MIN;
float zbuffer_max=2.0f*ZBUFFER_MIN;

float camera_half_width=4.0f;
float camera_half_height=4.0f;
float camera_yaw=0.0f;
float camera_angle=0.0f;
float camera_dist=(zbuffer_max-zbuffer_min)/2.0f;
float length;
Vector3D min;
Vector3D max;

bool draw_skin=true;
bool draw_normals=false;
bool lighting=true;

unsigned num_skins=0;
unsigned skin_num=0;

unsigned num_frames=0;
unsigned frame_num=0;

Model model;
static void CALLBACK Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glFrustum(-camera_half_width,camera_half_width,
		-camera_half_height,camera_half_height,zbuffer_min,zbuffer_max);
	glTranslatef(0.0f,0.0f,-camera_dist);
	glRotatef(camera_yaw,1.0f,0.0f,0.0f);
	glRotatef(90.0f-camera_angle,0.0f,1.0f,0.0f);
	
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	const float xyz[]=
	{
		-1.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	glLoadMatrixf(xyz);
	
	glPolygonMode(GL_FRONT,GL_FILL);
	if (lighting)
	{
		glEnable(GL_LIGHTING);

		const float direction0[]={0.0f,-1.0f,0.0f};
		const float position0[]={0.0f,camera_dist,0.0f,1.0f};
		const float specular0[]={5.0f,5.0f,5.0f,5.0f};
		const float diffuse0[]={5.0f,5.0f,5.0f,5.0f};
		const float ambient0[]={3.0f,3.0f,3.0f,3.0f};
		glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,0.0f);
		glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,direction0);
		glLightfv(GL_LIGHT0,GL_POSITION,position0);
		glLightfv(GL_LIGHT1,GL_SPECULAR,specular0);
		glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse0);
		glLightfv(GL_LIGHT0,GL_AMBIENT,ambient0);
		glEnable(GL_LIGHT0);

		const float direction1[]={0.0f,1.0f,0.0f};
		const float position1[]={0.0f,-camera_dist,0.0f,1.0f};
		const float specular1[]={5.0f,5.0f,5.0f,5.0f};
		const float diffuse1[]={5.0f,5.0f,5.0f,5.0f};
		glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0.0f);
		glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,direction1);
		glLightfv(GL_LIGHT1,GL_POSITION,position1);
		glLightfv(GL_LIGHT1,GL_SPECULAR,specular1);
		glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse1);
		glEnable(GL_LIGHT1);
	}
	else
		glDisable(GL_LIGHTING);
	if (draw_skin)
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		
		if (lighting)
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		else
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0f,1.0f,1.0f);
	}
	model.drawModel(skin_num,frame_num,draw_skin,lighting);
	if (draw_normals)
	{
		glDisable(GL_TEXTURE_2D);
		glColor3f(1.0f,1.0f,1.0f);
		model.drawNormals(frame_num);
	}
	//model.drawModel(0,frame_num);
	glFinish();
	auxSwapBuffers();
}
static void SwitchDrawSkin(void)
{
	draw_skin=!draw_skin;
}
static void SwitchLighting(void)
{
	lighting=!lighting;
}
static void SwitchDrawNormals(void)
{
	draw_normals=!draw_normals;
}
static void DistUp(void)
{
	camera_dist-=1.0f;
	if (camera_dist<zbuffer_min)
		camera_dist=zbuffer_min;
}
static void DistDown(void)
{
	camera_dist+=1.0f;
	if (camera_dist>zbuffer_max)
		camera_dist=zbuffer_max;
}
static void PrevFrame(void)
{
	frame_num--;
	if ((int)frame_num<0)
		frame_num=0;
}
static void NextFrame(void)
{
	frame_num++;
	if (frame_num>=num_frames)
		frame_num=num_frames-1;
}
static void PrevSkin(void)
{
	skin_num--;
	if ((int)skin_num<0)
		skin_num=0;
}
static void NextSkin(void)
{
	skin_num++;
	if (skin_num>=num_skins)
		skin_num=num_skins-1;
}
static void YawUp(void)
{
	camera_yaw+=1.0f;
	if (camera_yaw>90.0f)
		camera_yaw=90.0f;
}
static void YawDown(void)
{
	camera_yaw-=1.0f;
	if (camera_yaw<-90.0f)
		camera_yaw=-90.0f;
}
static void AngleRight(void)
{
	camera_angle-=1.0f;
	if (camera_angle<-180.0f)
		camera_angle+=360.0f;
}
static void AngleLeft(void)
{
	camera_angle+=1.0f;
	if (camera_angle>180.0f)
		camera_angle-=360.0f;
}
void InitKeyboard(void)
{
	auxKeyFunc(AUX_q,(AUXKEYPROC)DistUp);
	auxKeyFunc(AUX_a,(AUXKEYPROC)DistDown);
	auxKeyFunc(AUX_z,(AUXKEYPROC)PrevFrame);
	auxKeyFunc(AUX_x,(AUXKEYPROC)NextFrame);
	auxKeyFunc(AUX_c,(AUXKEYPROC)PrevSkin);
	auxKeyFunc(AUX_v,(AUXKEYPROC)NextSkin);
	
	auxKeyFunc(AUX_l,(AUXKEYPROC)SwitchLighting);
	auxKeyFunc(AUX_s,(AUXKEYPROC)SwitchDrawSkin);
	auxKeyFunc(AUX_n,(AUXKEYPROC)SwitchDrawNormals);

	auxKeyFunc(AUX_UP,(AUXKEYPROC)YawUp);
	auxKeyFunc(AUX_DOWN,(AUXKEYPROC)YawDown);
	auxKeyFunc(AUX_LEFT,(AUXKEYPROC)AngleLeft);
	auxKeyFunc(AUX_RIGHT,(AUXKEYPROC)AngleRight);
}

static void Init(void)
{
	InitKeyboard();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearDepth(zbuffer_min);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}
static void CALLBACK Reshape(unsigned width,unsigned height)
{
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	camera_half_width=(float)width/(float)height*length*zbuffer_min/camera_dist;
	camera_half_height=length*zbuffer_min/camera_dist;
}
void main(int carg,char **varg)
{
	if (carg!=2)
	{
		fprintf(stdout,"view3d 1.1 (08-04-2003) by Stupin W.A.\n"
						"Program for viewing Quake models with using OpenGL.\n"
						"Usage: view3d <model>\n"
						"\t<model>\t- name of Quake MDL file.\n"
						"Example: view3d quake/id1/pak0/progs/player.mdl\n"
						"Hot keys:\n"
						"\tModel:\n"
						"\t\tz\t\t- switch to previous model frame,\n"
						"\t\tx\t\t- switch to next model frame,\n"
						"\t\tc\t\t- switch to previous model skin,\n"
						"\t\tv\t\t- switch to next model skin,\n"
						"\tRender modes:\n"
						"\t\tl\t\t- on/off lighting (default - on),\n"
						"\t\ts\t\t- on/off model skin (default - on),\n"
						"\t\tn\t\t- on/off model vertex normals (default - off),\n"
						"\tCamera:\n"
						"\t\tq\t\t- move camera nearer,\n"
						"\t\ta\t\t- move camera farther,\n"
						"\t\tLeft arrow\t- rotate camera left,\n"
						"\t\tRight arrow\t- rotate camera right,\n"
						"\t\tUp arrow\t- rotate camera up,\n"
						"\t\tDown arrow\t- rotate camera down.\n");
		return;
	}
	if (!loadIDPO(&model,varg[1]))
		return;
	num_skins=model.getNumSkins();
	num_frames=model.getNumFrames();

	model.getMinMax(&min,&max);
	float min_length=Length(min);
	float max_length=Length(max);
	if (max_length>min_length)
		length=max_length;
	else
		length=min_length;

	camera_dist=length+zbuffer_min;
	zbuffer_max=length*2+zbuffer_min;

	auxInitPosition(0,0,400,300);
	auxInitDisplayMode(AUX_RGB|AUX_DOUBLE);
	if (auxInitWindow("view3d")==GL_FALSE)
		auxQuit();
	Init();
	auxReshapeFunc((AUXRESHAPEPROC)Reshape);
	auxMainLoop(Draw);
}
