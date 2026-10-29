//加上三頂帽子
//滑動鼠標放大縮小
//右鍵出現選單可加速、減速、回到正常速度
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265

static int slices = 16;
static int stacks = 16;

double viewSize = 3.0;
double ScaleFactor = 1.0;
int speedMenuVisible = 0;

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-viewSize, viewSize, -viewSize * (GLfloat) h / (GLfloat) w,
            viewSize * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-viewSize * (GLfloat) w / (GLfloat) h,
            viewSize * (GLfloat) w / (GLfloat) h, -viewSize, viewSize, -10.0, 10.0);


	//gluPerspective(	90.0, 1.0, 0, 0);

    glMatrixMode(GL_MODELVIEW);
}

/*calculate the frame duration to determind the display speed*/
double currentTime = 0.0;
double previousTime = 0.0;
double adjustDisplaySpeed(){
	previousTime = currentTime;
	currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	return currentTime - previousTime;
}

static double spin = 0.0;
double spinAngle = 30;

static double earthSpinSpeed = 60;
double earthSpinSpeedMultiply = 1.0;
double earthParam = 0.0;
static double earthSpinRadius = 2.5;
static double earthRadius = 0.3;

static double moonSpinSpeed = 180;
double moonSpinSpeedMultiply = 1.0;
double moonParam = 0.0;
static double moonSpinRadius = 0.5;
static double moonRadius = 0.1;

double XRotate = 30;
double YRotate = 0;
double XTranslate = 0;
double YTranslate = 0;
double Zoom = 1;

double sunSpinSpeedMultiply = 1.0;
double sombreroSpinSpeedMultiply = 1.0;

void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glLineWidth(3);
	glColor3d(0.8,0.8,0.8);
    glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * PI * float(ii) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex3f(x + cx, 0, y + cy);//output vertex

    }
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // view translation
    glTranslated(XTranslate, YTranslate, 0);
    // view rotation
    glRotated(XRotate, 1, 0, 0);
    glRotated(YRotate, 0, 1, 0);
    // zoom in/out
    glOrtho(-Zoom * ScaleFactor, Zoom * ScaleFactor, -Zoom * ScaleFactor, Zoom * ScaleFactor, Zoom, -Zoom);

    double duration = adjustDisplaySpeed(); /*seconds per frame*/

    spin = (spin + spinAngle * duration >= 360.0) ? 0.0 : spin + spinAngle * duration;
    sunSpinSpeedMultiply = (sunSpinSpeedMultiply > 4.0) ? 4.0 : sunSpinSpeedMultiply; // 避免超出速度範圍
    spinAngle = 30 * sunSpinSpeedMultiply; // 將速度因子應用於太陽的旋轉速度

    /*sun display*/
    glColor3d(1, 0.65, 0);
    glPushMatrix();
    glTranslated(0, 0, 0);
    glRotated(spin, 0, 1, 0);
    glutSolidCone(0.5, 1.5, slices, stacks);
    glutSolidTorus(0.3,
                   0.5,
                   20, 20);
    glPopMatrix();

    DrawCircle(0, 0, earthSpinRadius, 50);

    /*earth display*/
    glColor3d(0, 0.3, 1);
    earthParam = (earthParam + earthSpinSpeed * duration * earthSpinSpeedMultiply >= 360.0)
                     ? 0.0
                     : earthParam + earthSpinSpeed * duration * earthSpinSpeedMultiply;
    glPushMatrix();
    double earthX = sin(earthParam * PI / 180.0) * earthSpinRadius;
    double earthY = 0;
    double earthZ = cos(earthParam * PI / 180.0) * earthSpinRadius;
    glTranslated(earthX, earthY, earthZ);
    glRotated(spin, 0, 1, 0);
    glutSolidCone(earthRadius + 0.1, 1, slices, stacks);
    glutSolidTorus(earthRadius,
                   earthRadius - 0.8,
                   20, 20);
    glPopMatrix();

    DrawCircle(earthX, earthZ, moonSpinRadius + 0.5, 50);

    /*moon display*/
    glColor3d(0.4, 0.4, 0.4);
    moonParam = (moonParam + moonSpinSpeed * duration * moonSpinSpeedMultiply >= 360.0)
                    ? 0.0
                    : moonParam + moonSpinSpeed * duration * moonSpinSpeedMultiply;
    glPushMatrix();
    double moonX = sin(moonParam * PI / 180.0) * (moonSpinRadius + 0.5);
    double moonY = 0;
    double moonZ = cos(moonParam * PI / 180.0) * (moonSpinRadius + 0.5);
    glTranslated(earthX + moonX, earthY + moonY, earthZ + moonZ);
    glRotated(spin, 0, 1, 0);
    glutSolidCone(moonRadius + 0.1, 0.5, slices, stacks);
    glutSolidTorus(moonRadius,
                   moonRadius - 0.4,
                   20, 20);
    glPopMatrix();

    // Additional sombreros
    for (int i = 0; i < 3; i++)
    {
        glColor3d(1.0, 0.0, 0.0); // Change color as desired
        glPushMatrix();
        double sombreroX = cos((spin + i * 120.0) * PI / 180.0) * 3.0;
        double sombreroY = sin((spin + i * 120.0) * PI / 180.0) * 3.0;
        double sombreroZ = 0.0;
        glTranslated(sombreroX, sombreroY, sombreroZ);
        sombreroSpinSpeedMultiply = (sombreroSpinSpeedMultiply > 4.0) ? 4.0 : sombreroSpinSpeedMultiply; // 避免超出速度範圍
    	double sombreroSpinAngle = 120 * sombreroSpinSpeedMultiply; // 將速度因子應用於帽子的旋轉速度

    glRotated(spin + i * sombreroSpinAngle, 0, 0, 1);
        glutSolidCone(0.3, 0.2, slices, stacks);
        glutSolidTorus(0.1,
                       0.2,
                       20, 20);
        glPopMatrix();
    }

    glFlush();
    glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}
void speedMenu(int value)
{
    switch (value)
    {
        case 1: // 加速
            if (earthSpinSpeedMultiply <= 4.0) earthSpinSpeedMultiply *= 3;
            if (moonSpinSpeedMultiply <= 4.0) moonSpinSpeedMultiply *= 3;
            if (sunSpinSpeedMultiply <= 4.0) sunSpinSpeedMultiply *= 3;
            if (sombreroSpinSpeedMultiply <= 4.0) sombreroSpinSpeedMultiply *= 3;
            break;

        case 2: // 正常
            earthSpinSpeedMultiply = 1.0;
            moonSpinSpeedMultiply = 1.0;
            sunSpinSpeedMultiply = 1.0;
            sombreroSpinSpeedMultiply = 1.0;
            break;

        case 3: // 放慢
            if (earthSpinSpeedMultiply >= 0.5) earthSpinSpeedMultiply /= 4;
            if (moonSpinSpeedMultiply >= 0.5) moonSpinSpeedMultiply /= 4;
            if (sunSpinSpeedMultiply >= 0.5) sunSpinSpeedMultiply /= 4;
            if (sombreroSpinSpeedMultiply >= 0.5) sombreroSpinSpeedMultiply /= 4;
            break;

        default:
            break;
    }
    speedMenuVisible = 0;
}
int mouseLeftState = 0;
int mouseMiddleState = 0;
int mouseRightState = 0;
void mouse(int btn, int state, int x, int y){
	switch(state){
	case GLUT_DOWN:
		switch(btn){
			case GLUT_LEFT_BUTTON: mouseLeftState = 1; break;
			case GLUT_MIDDLE_BUTTON: mouseMiddleState = 1; break;
			case GLUT_RIGHT_BUTTON: mouseRightState = 1;
			 if (!speedMenuVisible)
                    {
                        speedMenuVisible = 1;
                        glutCreateMenu(speedMenu);
                        glutAddMenuEntry("Speed up!", 1);
                        glutAddMenuEntry("Normal speed.", 2);
                        glutAddMenuEntry("Slow down~", 3);
                        glutAttachMenu(GLUT_RIGHT_BUTTON);
                    }
			break;
			case 3: // 滾輪向上滾動
            ScaleFactor *= 1.1;
            break;
            case 4: // 滾輪向下滾動
            ScaleFactor /= 1.1;
            break;

			default: break;
		}
		break;
	case GLUT_UP:
		switch(btn){
			case GLUT_LEFT_BUTTON: mouseLeftState = 0; break;
			case GLUT_MIDDLE_BUTTON: mouseMiddleState = 0; break;
			case GLUT_RIGHT_BUTTON: mouseRightState = 0; break;
			default: break;
		}
		break;
	default:
		break;
	}
}

int currentMouseX = 0;
int currentMouseY = 0;
int previousMouseX = 0;
int previousMouseY = 0;
void mouseMotion(int x, int y){
	double Xscale, Yscale;
	switch(mouseLeftState){
		case 0: //no press
			break;
		case 1: //pressed
			currentMouseX = previousMouseX = x;
			currentMouseY = previousMouseY = y;
			mouseLeftState = 2;
			mouseMiddleState = 0;
			mouseRightState = 0;
			break;
		case 2: //already pressed and holding
			/*rotate view*/
			currentMouseX = x;
			Xscale = ((double)currentMouseX - (double)previousMouseX) / glutGet(GLUT_WINDOW_WIDTH);
			YRotate += Xscale*90;
			previousMouseX = currentMouseX;

			currentMouseY = y;
			Yscale = ((double)currentMouseY - (double)previousMouseY) / glutGet(GLUT_WINDOW_HEIGHT);
			XRotate += Yscale*90;
			previousMouseY = currentMouseY;
			/*limit x-rotate view degree*/
			if(XRotate > 90) XRotate = 90;
			if(XRotate < -90) XRotate = -90;

			break;
		default: break;
	}
	switch(mouseMiddleState){
		case 0: //no press
			break;
		case 1: //pressed
			currentMouseX = previousMouseX = x;
			currentMouseY = previousMouseY = y;
			mouseLeftState = 0;
			mouseMiddleState = 2;
			mouseRightState = 0;
			break;
		case 2: //already pressed and holding
			/*zoom-in/out view with only y position*/
			currentMouseX = x;
			Xscale = ((double)currentMouseX - (double)previousMouseX) / glutGet(GLUT_WINDOW_WIDTH);
			previousMouseX = currentMouseX;

			currentMouseY = y;
			Yscale = ((double)currentMouseY - (double)previousMouseY) / glutGet(GLUT_WINDOW_HEIGHT);
			Zoom += Yscale*1;
			previousMouseY = currentMouseY;
			/*limit zoom degree*/
			if(Zoom > 2.0) Zoom = 2.0;
			if(Zoom < 0.5) Zoom = 0.5;

			break;
		default: break;
	}
	switch(mouseRightState){
		case 0: //no press
			break;
		case 1: //pressed
			currentMouseX = previousMouseX = x;
			currentMouseY = previousMouseY = y;
			mouseLeftState = 0;
			mouseMiddleState = 0;
			mouseRightState = 2;
			break;
		case 2: //already pressed and holding
			/*translate view x & y*/
			currentMouseX = x;
			Xscale = ((double)currentMouseX - (double)previousMouseX) / glutGet(GLUT_WINDOW_WIDTH);
			XTranslate += Xscale*3;
			previousMouseX = currentMouseX;

			currentMouseY = y;
			Yscale = ((double)currentMouseY - (double)previousMouseY) / glutGet(GLUT_WINDOW_HEIGHT);
			YTranslate -= Yscale*3;
			previousMouseY = currentMouseY;
			/*limit x-translate view degree*/
			if(XTranslate > 2) XTranslate = 2;
			if(XTranslate < -2) XTranslate = -2;
			/*limit y-translate view degree*/
			if(YTranslate > 2) YTranslate = 2;
			if(YTranslate < -2) YTranslate = -2;

			break;

		default: break;
	}
}
static void
key(unsigned char key, int x, int y)
{
    switch (key)
    {
	case 'q':
	case 'Q':
		//increase earth rotation speed
		if(earthSpinSpeedMultiply <= 4.0) earthSpinSpeedMultiply *= 1.25;
		break;
	case 'w':
	case 'W':
		//decrease earth rotation speed
		if(earthSpinSpeedMultiply >= 0.5) earthSpinSpeedMultiply /= 1.25;
		break;
	case 'a':
	case 'A':
		//increase moon rotation speed
		if(moonSpinSpeedMultiply <= 4.0) moonSpinSpeedMultiply *= 1.25;
		break;
	case 's':
	case 'S':
		//decrease moon rotation speed
		if(moonSpinSpeedMultiply >= 0.5) moonSpinSpeedMultiply /= 1.25;
		break;
	case 'r':
	case 'R':
		//reset the view
		XRotate = 30;
		YRotate = 0;
		XTranslate = 0;
		YTranslate = 0;
		Zoom = 1;
		mouseLeftState = 0;
		mouseMiddleState = 0;
		mouseRightState = 0;
		break;
	default:
		break;
    }

    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int
main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1024,768);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("orbit planets");

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
	glutMouseFunc (mouse);
	glutMotionFunc (mouseMotion);

    glClearColor(0,0,0,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}