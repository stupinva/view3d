#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include <math.h>
#pragma warning(disable:4244)

const float M_PI	=3.14159265f;

float camera_yaw=0.0f;
float camera_angle=0.0f;
float camera_dist=5.0f;

//unsigned frame_num=0;

static void CALLBACK Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glFrustum(-4.0f,4.0f,-3.0f,3.0f,4.0f,25.0f);
	glTranslatef(0.0f,0.0f,-camera_dist);
	glRotatef(camera_yaw,1.0f,0.0f,0.0f);
	glRotatef(90.0f-camera_angle,0.0f,1.0f,0.0f);
	
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	const float xyz[]=
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	glLoadMatrixf(xyz);
/*
	glEnable(GL_LIGHTING);

	const float direction0[]={0.0f,-1.0f,0.0f};
	const float position0[]={0.0f,3.0f,0.0f,1.0f};
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,0.0f);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,direction0);
	glLightfv(GL_LIGHT0,GL_POSITION,position0);
	glEnable(GL_LIGHT0);

	const float direction1[]={0.0f,1.0f,0.0f};
	const float position1[]={0.0f,-3.0f,0.0f,1.0f};
	const float specular1[]={1.0f,1.0f,1.0f,1.0f};
	const float diffuse1[]={1.0f,1.0f,1.0f,1.0f};
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0.0f);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,direction1);
	glLightfv(GL_LIGHT1,GL_POSITION,position1);
	glLightfv(GL_LIGHT1,GL_SPECULAR,specular1);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse1);
	glEnable(GL_LIGHT1);
*/
	glPolygonMode(GL_FRONT,GL_FILL);
	//glPolygonMode(GL_BACK,GL_LINE);

	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f,0.0f,2.0f);
	glNormal3f(0.0f,0.0f,1.0f);
	for(int i=0;i<5;i++)
	{
		float x=cos(i*M_PI/2.0f);
		float y=sin(i*M_PI/2.0f);
		glVertex3f(x,y,0.0f);
		glNormal3f(x,y,0.0f);
	}
	glEnd();

	glColor3f(0.8f,0.8f,0.8f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f,0.0f,-2.0f);
	glNormal3f(0.0f,0.0f,-1.0f);
	for(i=0;i<5;i++)
	{
		float x=cos(i*M_PI/2.0f);
		float y=sin(i*M_PI/2.0f);
		glVertex3f(x,y,0.0f);
		glNormal3f(x,y,0.0f);
	}
	glEnd();
	glFinish();
	auxSwapBuffers();
}

static void PrevFrame(void)
{
	camera_dist-=0.1f;//frame_num--;
}
static void NextFrame(void)
{
	camera_dist+=0.1f;//frame_num++;
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
	auxKeyFunc(/*AUX_Z*/'z',(AUXKEYPROC)PrevFrame);
	auxKeyFunc(/*AUX_X*/'x',(AUXKEYPROC)NextFrame);
	auxKeyFunc(AUX_UP,(AUXKEYPROC)YawUp);
	auxKeyFunc(AUX_DOWN,(AUXKEYPROC)YawDown);
	auxKeyFunc(AUX_LEFT,(AUXKEYPROC)AngleLeft);
	auxKeyFunc(AUX_RIGHT,(AUXKEYPROC)AngleRight);
}

static void Init(void)
{
	InitKeyboard();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}
static void CALLBACK Reshape(unsigned width,unsigned height)
{
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
void main(void)
{
	auxInitPosition(0,0,400,300);
	auxInitDisplayMode(AUX_RGB|AUX_DOUBLE);
	if (auxInitWindow("view3d")==GL_FALSE)
	{
		auxQuit();
	}
	Init();
	auxReshapeFunc((AUXRESHAPEPROC)Reshape);
	auxMainLoop(Draw);
}