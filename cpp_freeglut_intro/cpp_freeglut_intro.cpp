#include <iostream>
#include <chrono>
#include <GL/freeglut.h>

int wx=600; int wy=600;
float ang = 0;
float rotSpd = 30; //degrees per second

std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();

void cbDisplay();

float getDeltaTime()
{
	auto now = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
	lastUpdate = now;
	return deltaTime;
}

void cbSpecial(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1:
		rotSpd -= 10; break;
	case GLUT_KEY_F2:
		rotSpd += 10;; break;
	}
}

void cbIdle()
{
	cbDisplay();
}


void cbReshape(int x, int y)
{
	wx = x;
	wy = y;
	glViewport(0,0,wx,wy);
}


//our function resposible for rendering
void cbDisplay()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double rwx = 0; double rwy = 0;

	if (wx > wy)
	{
		rwy = 2;
		rwx = 2 * ((double)wx/(double)wy);
	}
	else
	{
		rwx = 2;
		rwy = 2 * ((double)wy/(double)wx);
	}

	double oLeft =   -(rwx / 2);
	double oRight =   (rwx / 2);
	double oBottom = -(rwy / 2);
	double oTop =     (rwy / 2);

	//printf("L=%f; R=%f; B=%f; T=%f; wx/wy=%f;\n", oLeft, oRight, oBottom, oTop, ((double)wx / (double)wy));

	gluOrtho2D(oLeft, oRight, oBottom, oTop);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float dt = getDeltaTime();

	ang += rotSpd * dt;
	if (ang > 360) ang -= 360;

	glRotatef(ang,0,0,1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//here we draw
	
	glColor3f(0.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(-0.5f,-0.5f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.0f, 0.5f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.5f, -0.5f);

	glEnd();

	//here we stop drawing and put what we did on screen
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	//GLUT system init block
	glutInit(&argc, argv); //initializing GLUT

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitWindowSize(wx, wy);

	glutCreateWindow("GLUT WINDOW");

	//OpenGL setup block
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//GLUT launch block
	glutSpecialFunc(cbSpecial);
	glutIdleFunc(cbIdle);
	glutReshapeFunc(cbReshape);
	glutDisplayFunc(cbDisplay);

	glutMainLoop();

	return 0;
}