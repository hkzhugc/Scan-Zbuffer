#include "gl/glut.h"
#include <iostream>
#include <stdio.h>
#include "plyloader.h"
#include "Renderer.h"
#include "Camera.h"

void init2();
void display2();
void reshape2(GLsizei w, GLsizei h);
void keyboard(GLubyte key, GLint x, GLint y);
void draw_scene();
float angley = 0.0;
float scale = 1.1;

Renderer my_render;
Camera camera;

int main(int argc, char **argv)
{
	PLYModel myModel("../assets/cube2.ply", false, false);
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
	my_render.render();
	glDrawPixels(my_render.frame_buffer.w, my_render.frame_buffer.h,
		GL_RGB, GL_UNSIGNED_BYTE, my_render.frame_buffer.getBuffer());

	glutSwapBuffers();
}

void reshape2(GLsizei w, GLsizei h)
{
	my_render.reset(w, h);
	my_render.change_view(scale, angley, glm::vec3(0, 0, 0));
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
		my_render.change_view(scale, angley, glm::vec3(0, 0, 0));
		glutPostRedisplay();
	}
}