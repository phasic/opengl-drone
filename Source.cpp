#include "Header.h"

void lightInit(void);
void lightWorld(void);
void lightDrone(void);

void lightBody(int start);
void droneMaterial(void);
void materialDrone(int object);

void lightControl(int controlAll);
void printKeybindInfo(void);
void myInit(void)
{
	glBlendFunc(GL_ONE, GL_SRC_ALPHA);
	glClearColor(0.8, 0.8, 0.8, 0.0);
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
					0, 1, 12, 4, &bezierctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	/*cylinders*/
	cyl = gluNewQuadric();
	gluQuadricNormals(cyl, GLU_SMOOTH);

	/*LIGHTING*/
	lightInit();

	printKeybindInfo();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		axis = !axis;
		break;
	case '2':
		wire = !wire;
		break;
	case '3':
		if (fly)
			anim = 1;
		else
			anim = !anim;
		break;
	case '4':
		fly = !fly;
		anim = 1;
		break;

	case '5':
		zeroSwitch = !zeroSwitch;
		lightControl(0);
		break;
	case '6':
		oneSwitch = !oneSwitch;
		lightControl(0);
		break;
	case '7':
		twoSwitch = !twoSwitch;
		lightControl(0);
		break;
	case 'r':
		scale[0] = 1.0, scale[1] = 1.0, scale[2] = 1.0, /*reset*/
				eye[0] = 75.0, eye[1] = 75.0, eye[2] = 75.0,
		camAngleX = -135, camAngleY = -45;
		break;
	case 'a':
		camAngleX -= 1;
		break; /*rotate*/
	case 'd':
		camAngleX += 1;
		break;
	case 's':
		camAngleY += 1;
		break;
	case 'w':
		camAngleY -= 1;
		break;
	case 'h':
		changeHeight = 2;
		break;
	case 'H':
		changeHeight = 1;
		break;
	case 'l':
		masterSwitch = !masterSwitch;
		lightControl(1);
		break;

	case 'q':
		scale[0] *= 1.1;
		scale[1] *= 1.1;
		scale[2] *= 1.1;
		break; /*scale*/
	case 'e':
		scale[0] /= 1.1;
		scale[1] /= 1.1;
		scale[2] /= 1.1;
		break;

	case 'v':
		show = !show;
		break;
	case 'f':
		flat = !flat;
		break;
	}

	glutPostRedisplay();
}

#pragma region DisplayFunction
void DisplayAxis(void)
{
	if (axis)
	{
		glLineWidth(1.0);
		//z-axis
		glColor3f(0.8, 0.1, 0.0);
		glLineStipple(2, 0x7070);
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 100);
		glEnd();
		glPushMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		//x-axis
		glColor3f(0.0, 0.1, 0.8);
		glLineStipple(2, 0x0707);
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 100);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		//y-axis
		glColor3f(0.0, 0.8, 0.1);
		glLineStipple(2, 0xFF00);
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 100);
		glEnd();
		glPopMatrix();
	}
}
void DrawFloor(void)
{
	glPushMatrix();
	glTranslatef(0, -15, 0);
	glScalef(1000, 1000, 1000);
	glBegin(GL_QUADS);
	glColor3f(0.3, 0.3, 0.3);
	glVertex3f(-1, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, -1);
	glVertex3f(-1, 0, -1);
	glEnd();
	glPopMatrix();
}
void DrawBeam(void)
{
	glLineWidth(0.5);
	glPushMatrix();

	materialDrone(FRAME);
	glScalef(WIDTH, 1, 1);
	if (wire)
		glutWireCube(1);
	else
		glutSolidCube(1);
	glPopMatrix();
}
void DrawCylinders(void)
{
	glPushMatrix();
	materialDrone(FRAME);
	glRotatef(90, 1, 0, 0);
	glTranslatef(WIDTH / 2, 0, -4);
	gluCylinder(cyl, 1.0, 1.0, 5, 50, 1);
	gluDisk(cyl, 0, 1, 50, 1);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(-WIDTH / 2, 0, -4);
	gluCylinder(cyl, 1.0, 1.0, 5, 50, 1);
	gluDisk(cyl, 0, 1, 50, 1);
	glPopMatrix();
}
void DrawBezier(int angle)
{
	glPushMatrix();
	materialDrone(PROPELLER);
	glTranslatef(0, 0.2, 0);
	glScalef(2, 2, 2);
	glRotatef(angle, 0, 1, 0);
	glRotatef(animAngle, 0, 1, 0);
	glEnable(GL_DEPTH_TEST); //update depth buffer
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &bezierctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	if (wire)
		glEvalMesh2(GL_LINE, 0, 20, 0, 20);
	else
		glEvalMesh2(GL_FILL, 0, 20, 0, 20);
	glPopMatrix();
}
void DrawPropeller(void)
{
	glPushMatrix();
	glTranslatef(WIDTH / 2, 3, 0);
	for (int i = 0; i <= 240; i += 120)
		DrawBezier(i);
	glRotatef(180, 0, 1, 0);
	glTranslatef(WIDTH, 0, 0);
	for (int i = 0; i <= 240; i += 120)
		DrawBezier(i);
	glPopMatrix();
}
void DrawSkeleton(void)
{
	glPushMatrix();
	DrawBeam();
	DrawCylinders();
	DrawPropeller();
	glRotatef(90, 0, 1, 0);
	DrawBeam();
	DrawCylinders();
	DrawPropeller();
	glPopMatrix();
}
void DrawTorus(void)
{
	glLineWidth(0.5);
	glPushMatrix();
	materialDrone(FRAME);
	glRotatef(90, 1, 0, 0);
	if (wire)
		glutWireTorus(1, 15, 40, 100);
	else
		glutSolidTorus(1, 15, 40, 100);
	glPopMatrix();
}
void DrawCone(void)
{
	glPushMatrix();
	materialDrone(FRAME);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -0.5);

	gluCylinder(cyl, 5.0, 7.5, 5, 100, 100);
	glPopMatrix();
}
void DrawNurbs(void)
{
	glPushMatrix();

	GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};

	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 8, knots, 8, knots, 4 * 3, 3, &splinectrlpoints[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb);
	glPopMatrix();
}
void DrawBody(void)
{
	lightBody(1);
	glPushMatrix();

	glScalef(2.5, 2.2, 2.5);

	DrawNurbs();
	glScalef(1, 1.5, 1);
	glRotatef(180, 1, 0, 0);

	DrawNurbs();
	lightBody(0);
	glPopMatrix();
}
void DrawDrone(void)
{
	glPushMatrix();
	DrawSkeleton();
	DrawTorus();
	DrawCone();
	DrawBody();
	glDisable(GL_LIGHTING);
	glPopMatrix();
}
void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	distance = sqrt(pow(eye[0], 2) + pow(eye[1], 2) + pow(eye[2], 2));

	eye[0] = distance * -sinf(camAngleX * (M_PI / 180)) * cosf((camAngleY) * (M_PI / 180));
	eye[1] = distance * -sinf((camAngleY) * (M_PI / 180));
	eye[2] = -distance * cosf((camAngleX) * (M_PI / 180)) * cosf((camAngleY) * (M_PI / 180));

	gluLookAt(eye[0], eye[1], eye[2], ref[0], ref[1], ref[2], up[0], up[1], up[2]);
	DisplayAxis();
	glRotatef(animFlyAngle, 0, 1, 0);
	glPushMatrix();
	glRotatef(animFlyTilt, 1, 0, 0);
	glScalef(scale[0], scale[1], scale[2]);
	glTranslatef(75, height, 0);
	if (show)
	{
		lightWorld();
		DrawDrone();
	}

	glPopMatrix();
	/*CENTER*/
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glScalef(scale[0], scale[1], scale[2]);
	glTranslatef(151, 0, 0);
	glutSolidCube(1);
	glPopMatrix();
	glFlush();
}
void rescale(GLint n_w, GLint n_h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, near, far);
	glViewport(0, 0, n_w, n_h);
}
#pragma endregion
#pragma region AnimationFunctions
void animPropeller(int delta)
{

	if (anim)
	{

		animAngle -= anim * PROPELLERSPEED;
		if (animAngle > 360.0)
			animAngle -= 360.0;
		if (animAngle < -360.0)
			animAngle += 360.0;
	}

	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(time_, animPropeller, 0);
}

void animFlyHeight(int delta)
{

	if (!heightAchieved && fly && height < maxHeight)
		height++;

	if (!fly && height > 0)
	{
		animFlyTilt = 0;
		height--;
	}
	if (height == 0)
		heightAchieved = 0;
	if (height == maxHeight)
		heightAchieved = 1;

	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(time2_, animFlyHeight, 0);
}

void animChangeHeight(int delta)
{

	if (heightAchieved && changeHeight == 1)
	{
		height--;
		fprintf(stderr, "Dalen \n");
	}

	if (heightAchieved && changeHeight == 2)
	{
		height++;
		fprintf(stderr, "Stijgen \n");
	}

	changeHeight = 0;

	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(time_, animChangeHeight, 0);
}
void animFlyRotation(int delta)
{
	if (fly)
	{
		animFlyTilt = 10;
		animFlyAngle -= FLYROTATIONSPEED;
		if (animFlyAngle > 360.0)
			animFlyAngle -= 360.0;
		if (animFlyAngle < -360.0)
			animFlyAngle += 360.0;
	}
	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(time_, animFlyRotation, 0);
}
void animDrone(void)
{
	glutTimerFunc(time_, animPropeller, 0);
	glutTimerFunc(time2_, animFlyHeight, 0);
	glutTimerFunc(time2_, animChangeHeight, 0);
	glutTimerFunc(time2_, animFlyRotation, 0);
}

#pragma endregion
#pragma region LightingFunctions

void lightInit(void)
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, grBlue);
	glLightfv(GL_LIGHT2, GL_SPECULAR, red);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);
}

void lightControl(int controllAll)
{
	if (!controllAll)
	{
		if (!zeroSwitch)
			glDisable(GL_LIGHT0);
		if (zeroSwitch)
			glEnable(GL_LIGHT0);
		if (!oneSwitch)
			glDisable(GL_LIGHT1);
		if (oneSwitch)
			glEnable(GL_LIGHT1);
		if (!twoSwitch)
			glDisable(GL_LIGHT2);
		if (twoSwitch)
			glEnable(GL_LIGHT2);
	}
	else
	{
		if (!masterSwitch)
		{
			zeroSwitch = 0, oneSwitch = 0, twoSwitch = 0;
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
		}
		if (masterSwitch)
		{
			zeroSwitch = 1, oneSwitch = 1, twoSwitch = 1;
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHT2);
		}
	}
}
void lightBody(int start)
{
	if (start)
	{
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
	}
	if (!start)
	{
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
	}
}
void materialDrone(int object)
{

	switch (object)
	{
	case PROPELLER:
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor4f(0.6706, 0.6706, 0.6706, 1);

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
		break;

	case FRAME:
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor4f(0.2, 0.2, 0.2, 1.0);

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_2);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess_2);
		break;

	case BODY:
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor4f(0.5, 0.5, 0.5, 1.0);

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_2);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess_2);
		break;
	}
}
void lightWorld(void)
{
	glShadeModel(flat ? GL_FLAT : GL_SMOOTH);
	//lightDrone();
	glLightfv(GL_LIGHT0, GL_POSITION, lp0);
	glLightfv(GL_LIGHT1, GL_POSITION, lp1);
	glLightfv(GL_LIGHT2, GL_POSITION, lp2);

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
}

void printKeybindInfo(void)
{
		fprintf(stderr, "-----KEYBINDINGS-----\n");
		fprintf(stderr, "~~~Rendering~~~\n");
		fprintf(stderr, "\t1: show/hide axis\n");
		fprintf(stderr, "\t2: show/hide wireframe\n");
		fprintf(stderr, "\tv: show/hide drone\n");
		fprintf(stderr, "~~~Lighting~~~\n");
		fprintf(stderr, "\tl: show/hide all lights\n");
		fprintf(stderr, "\t5: show/hide ambient light\n");
		fprintf(stderr, "\t6: show/hide diffuse light\n");
		fprintf(stderr, "\t7: show/hide specular light\n");
		fprintf(stderr, "\tf: flatten lighting\n");
		fprintf(stderr, "~~~Animation~~~\n");
		fprintf(stderr, "\t3: start/stop propellors\n");
		fprintf(stderr, "\t4: start/stop flying\n");
		fprintf(stderr, "\tr: reset drone\n");
		fprintf(stderr, "~~~Camera control~~~\n");
		fprintf(stderr, "\tw/a/s/d: rotate viewpoint\n");
		fprintf(stderr, "\tq/e: zoom in/out\n");
		fprintf(stderr, "\th/H: raise/lower drone\n");
}

#pragma endregion
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Drone");
	glutKeyboardFunc(keyboard);

	glutReshapeFunc(rescale);
	myInit();
	glutDisplayFunc(draw);
	animDrone();
	glutMainLoop();
}