#include "gl/glut.h"
#include <iostream>
#include <stdio.h>
#include "plyloader.h"
#include "Renderer.h"
#include "Camera.h"

void init();
void display();
void reshape(GLsizei w, GLsizei h);
void init2();
void display2();
void reshape2(GLsizei w, GLsizei h);
void keyboard(GLubyte key, GLint x, GLint y);
void draw_scene();
float angley = 0;
float scale = 1.1;

Renderer my_render;
Camera camera;

int main(int argc, char **argv)
{
	PLYModel myModel("../assets/bunny.ply", false, false);
	my_render.init(&myModel, &camera);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(my_render.frame_buffer.w, my_render.frame_buffer.h);
	glutCreateWindow("Scan-Zbuffer");
	init2();
	glutDisplayFunc(display2);
	glutReshapeFunc(reshape2);
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
	my_render.myPerspective(10, 1, 0.1, 10.0);
	my_render.change_view(scale, angley, glm::vec3(0, 0, 0));
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

void init2()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, my_render.frame_buffer.w, 0, my_render.frame_buffer.h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	my_render.change_view(scale, angley, glm::vec3(0, 0, 0));
}

void display2()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glRasterPos2i(0, 0);
	//printf("w = %d, h = %d\n", my_render.frame_buffer.w, my_render.frame_buffer.h);
	my_render.render();
	glDrawPixels(my_render.frame_buffer.w, my_render.frame_buffer.h,
		GL_RGB, GL_UNSIGNED_BYTE, my_render.frame_buffer.getBuffer());

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
}

void reshape2(GLsizei w, GLsizei h)
{
	my_render.reset(w, h);
	//init2();
	display2();
}

void keyboard(GLubyte key, GLint x, GLint y)
{
	bool flag = true;
	switch (key) {

	case 'a':
		angley -= 0.1;
		break;
	case 'd':
		angley += 0.1;
		break;
	case 'w':
		scale += 0.1;
		break;
	case 's':
		scale -= 0.1;
		break;
	case 27:
		exit(0);
		break;
	default:
		flag = false;
		break;
	}
	if (flag)
	{
		printf("angley = %f\n", angley);
		my_render.change_view(scale, angley, glm::vec3(0, 0, 0));
		glutPostRedisplay();
		//display2();
	}
}