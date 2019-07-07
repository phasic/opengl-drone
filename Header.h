#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #include <GL/glut.h>
#include <GLUT/glut.h>

#define WIDTH	50
#define	FLYROTATIONSPEED 1
#define PROPELLERSPEED 5
// #define M_PI 3.14159265358979323846


#define PROPELLER 1
#define FRAME 2
#define BODY 3

/*Keyboard Variables*/
int as = 1, wire = 0, show = 1, anim = 0, fly = 0, pilot = 0;

/*DRAW PROPERTIES*/
#pragma region DrawProperties
/*Type of projection*/
char typ;

/*Camera Variables*/
GLfloat oog[3] = { 75.0, 75.0, 75.0 };
GLfloat ref[3] = { 0.0, 0.0, 0.0 };
GLfloat up[3] = { 0.0, 100.0, 0.0 };
GLdouble xmin = -100.0, xmax = 100.0, ymin = -100.0, ymax = 100.0;
GLdouble near = 1.0, far = 10000.0;

/*Propellor variabelen*/
GLUquadric* cyl;

/*Scaling*/
GLfloat scale[3] = { 1.0, 1.0, 1.0 };

/*rotate camera*/
GLfloat camAngleX = -135, camAngleY = -45;
GLfloat distance;

/*Bezier Controlpoints*/
GLfloat bezierctrlpoints[4][4][3] = {
	{ { 0, -0.10, 0 }, { 0, -0.3, 0 },	{ 0, 0.3, 0 }, { 0, 1.0, 0 } },
	{ { 2, -0.866, -0.5 }, { 2, -0.26, -0.15 },	{ 2, 0.866, 0.5 }, { 2, 0.26, 0.15 } },
	{ { 4, -0.5, -0.866 }, { 4, -0.15, -0.26 },	{ 4, 0.5, 0.866 }, { 4, 0.15, 0.26 } },
	{ { 6, 0, -0.10 }, { 6, 0, -0.3 },	{ 6, 0, 0.3 }, { 6, 0, 0.10 } }
};

/*B-spline Controlpoints*/
GLfloat splinectrlpoints[4][4][3] = {
	{ { 10, 0, 0 }, { 5, 0, 0 }, { 0, 0, 5 }, { 0, 0, 10 } },
	{ { 5, 0, 0 }, { 5, 5, 0 }, { 0, 5, 5 }, { 0, 0, 5 } },
	{ { 0, 0, -5 }, { 0, 5, -5 }, { -5, 5, 0 }, { -5,0 , 0 } },
	{ { 0, 0, -10 }, { 0, 0, -5 }, { -5, 0, 0 }, { -10, 0, 0 } }
};
/*Nurbs*/
GLUnurbsObj* theNurb = gluNewNurbsRenderer();
#pragma endregion
/*ANIMATION PROPERTIES*/
#pragma region AnimationProperties
GLfloat animHoek, animFlyHoek, animFlyTilt;
GLint tijd = 6, tijd2 = 10;

int maxHeight = 50;
int heightAchieved = 0;
int changeHeight = 0;
int height = 1;

#pragma endregion

/*LIGHTING PROPERTIES*/
#pragma region LightingProperties
/*Lighting Locations*/
GLfloat lp0[] = { -50.0, 25.0, 0.0, 0.0 }; 
GLfloat lp1[] = { 0.0, 0.0, 50.0, 0.0 };
GLfloat lp2[] = { 50.0, 50.0, 0.0, 1.0 };

GLfloat lp3[] = { 0, 0,0,0 };
GLfloat lp3_direction[] = { 0, -1, 0 };

/*Light Colours*/
GLfloat wit[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat grblauw[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat rood[] = { 1.0, 0.4, 0.4, 1.0 };
GLfloat grijs[] = { 0.5, 0.5, 0.5, 1.0 };

GLfloat zwart[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat blauw[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat roodblauw[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat groen[] = { 0.0, 1.0, 0.0, 1.0 };




GLfloat direc[] = { 1.0, 0, 0.0 };
GLfloat shini = 60.0;
GLfloat hoek = 50.0;
GLfloat expo = 5.0;
GLfloat bocht = 0.0;    /* roterende bol */
GLfloat zwak = 1.0;    /* lineaire verzwakking */
int	welk = 1;

GLint flat = 1;


/*Light Control*/

int masterSwitch = 0;

int zeroSwitch = 0, oneSwitch = 0, twoSwitch = 0;
int droneSwitch = 0;



/*Material*/

GLfloat ambient_r, ambient_g, ambient_b,
diffuse_r, diffuse_g, diffuse_b,
specular_r, specular_g, specular_b;

float mat_specular[] = { 0.7569, 0.8039, 0.08039, 1 };
float shininess = 1000;
float mat_specular_2[] = { 0.992157, 0.941176, 0.807843, 1.0 };
float shininess_2 = 100;

#pragma endregion