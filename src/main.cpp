#include "gl/glut.h"
#include <iostream>
#include <stdio.h>
#include "plyloader.h"
#include "Renderer.h"
#include "Camera.h"
#include <time.h>

void init();
void display();
void reshape(GLsizei w, GLsizei h);
void keyboard(GLubyte key, GLint x, GLint y);
void draw_scene();
float angley = 0.0;

Renderer my_render;
Camera camera;

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "please enter the ply location" << endl;
		//return 1;
	}
		
	PLYModel myModel("../assets/bunny.ply", false, false);
	my_render.init(&myModel, &camera);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(my_render.frame_buffer.w, my_render.frame_buffer.h);
	glutCreateWindow("Scan-Zbuffer");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}


void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, my_render.frame_buffer.w, 0, my_render.frame_buffer.h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	my_render.change_view(angley, glm::vec3(0, 0, 0));
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glRasterPos2i(0, 0);
	
	clock_t start = clock();
	my_render.render();
	glDrawPixels(my_render.frame_buffer.w, my_render.frame_buffer.h,
		GL_RGB, GL_UNSIGNED_BYTE, my_render.frame_buffer.getBuffer());
	glutSwapBuffers();
	clock_t end = clock();
	cout << "rendering a frame cost " << difftime(end, start) << " msec" << endl;
}

void reshape(GLsizei w, GLsizei h)
{
	my_render.reset(w, h);
	my_render.change_view(angley, glm::vec3(0, 0, 0));
	display();
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
	case 27:
		exit(0);
		break;
	default:
		flag = false;
		break;
	}
	if (flag)
	{
		my_render.change_view(angley, glm::vec3(0, 0, 0));
		glutPostRedisplay();
	}
}