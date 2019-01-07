#include "gl/glut.h"
#include <iostream>
#include <stdio.h>
#include "plyloader.h"
#include "Renderer.h"
#include "Camera.h"

void init();
void display();
void reshape(GLsizei w, GLsizei h);
void keyboard(GLubyte key, GLint x, GLint y);
void draw_scene();
float angley = 0;
float anglex = 2.1;

Renderer my_render;
Camera camera;

int main(int argc, char **argv)
{
	PLYModel myModel("../assets/bunny.ply", false, false);
	my_render.init(&myModel, &camera);
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 1;
}


void draw_scene()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	glEnd();
}

void init()
{
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Scan-Zbuffer");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	GLfloat AmbientLight[4] = { 1, 1, 1, 1 };
	GLfloat light_position[] = { 5, 50, 50, 1 };
	GLfloat mat_diffuse[] = { 0.85f, 0.65f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	my_render.change_view(angley, glm::vec3(0, 0, +0.00));
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	my_render.render_face();

	glPopMatrix();
	glFlush();
}

void reshape(GLsizei w, GLsizei h)
{
	GLfloat fovy = 30;
	GLfloat aspect = 1;
	if (0 == h) { h = 1; }
	glViewport(0, 0, w, h);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	fovy = 10;
	//aspect = (GLfloat)w / h;
	if (w < h)
	{
		fovy = fovy * (GLfloat)h / w;
	}
	my_render.myPerspective(fovy, aspect, 0.1, 10.0);
	//gluPerspective(fovy, aspect, 0.1, 100.0);
	//glOrtho(, 0.1, 100.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void keyboard(GLubyte key, GLint x, GLint y)
{
	switch (key) {

	case 'a':
		angley -= 0.1;
		break;
	case 'd':
		angley += 0.1;
		break;
	case 'w':
		anglex -= 1.7;
		break;
	case 's':
		anglex += 1.7;
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	my_render.change_view(angley, glm::vec3(0, 0, 0));
	glutPostRedisplay();
}