#include "Renderer.h"
#include <iostream>
float aangley = 2.1;
float aanglex = 2.1;

void defalut_display_func()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0, 0, -10);
	glRotatef(aangley, 0, 1, 0);
	glRotatef(aanglex, 1, 0, 0);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//drawthings

	glPopMatrix();
	glFlush();
}

void defalut_reshape_func(int width, int height)
{

}

Renderer::Renderer()
{
	display_func = nullptr;
	reshape_func = nullptr;
	m_model = nullptr;
	vertex_buffer = nullptr;
}


Renderer::~Renderer()
{
	if (vertex_buffer)
		delete[] vertex_buffer;
}

void Renderer::change_view(float angle, glm::vec3 position, glm::vec3 view_dir, glm::vec3 up)
{
	//position.x = m_model->center.x;
	//position.y = m_model->center.y;
	//position.z = m_model->center.z;
	if (vertex_buffer == nullptr)
		vertex_buffer = new glm::vec3[m_model->vertexCount];
	camera_ptr->set_camera();
	camera_ptr->translate(-m_model->center);
	//camera_ptr->set_camera(position, view_dir, up);
	camera_ptr->translate(-position);
	camera_ptr->rotate(angle, glm::vec3(0, 1, 0));
	
	for (int i = 0; i < m_model->vertexCount; i++)
	{
		auto v = camera_ptr->transform(m_model->positions[i]);
		v = camera_ptr->pspt_prj(v);
		vertex_buffer[i] = v;
	}
	
}

void Renderer::myPerspective(float fovy, float aspect, float znear, float zfar)
{
	camera_ptr->set_pspt_prj(fovy, aspect, znear, zfar);
}

void Renderer::render_face(int f_idx)
{
	auto f = m_model->faces[f_idx];
	auto c = m_model->colors[f_idx];
	//auto v1 = camera_ptr->transform(m_model->positions[f[0]]);
	//auto v2 = camera_ptr->transform(m_model->positions[f[1]]);
	//auto v3 = camera_ptr->transform(m_model->positions[f[2]]);
	//auto v1 = vertex_buffer[m_model->positions[f[0]];
	auto v1 = vertex_buffer[f[0]];
	auto v2 = vertex_buffer[f[1]];
	auto v3 = vertex_buffer[f[2]];
	//auto center = (v1 + v2 + v3) / 3.f;
	glColor3f(c[0], c[1], c[2]);
	glBegin(GL_TRIANGLES);
	glVertex3f(v1[0], v1[1], v1[2]);
	glVertex3f(v2[0], v2[1], v2[2]);
	glVertex3f(v3[0], v3[1], v3[2]);
	glEnd();
}

void Renderer::render_face()
{
	for (int i = 0; i < m_model->faceCount; i++)
	{
		render_face(i);
	}
}

void Renderer::start_render(int *argcp, char **argv)
{
}

void Renderer::init(PLYModel* m, Camera * c)
{
	m_model = m;
	camera_ptr = c;
}

