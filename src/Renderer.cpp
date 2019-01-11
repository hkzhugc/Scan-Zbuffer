#include "Renderer.h"
#include <iostream>

Renderer::Renderer() : frame_buffer(RES, RES), scan_line(RES)
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

void Renderer::change_view(float scale, float angle, glm::vec3 position, glm::vec3 view_dir, glm::vec3 up)
{
	//position.x = m_model->center.x;
	//position.y = m_model->center.y;
	//position.z = m_model->center.z;

	//scale = RES / std::fmax(std::fmax(m_model->max.x, m_model->max.y), m_model->max.z);
	scale = std::fmin(frame_buffer.w / m_model->max.x, frame_buffer.h / m_model->max.y);
	if (vertex_buffer == nullptr)
		vertex_buffer = new glm::vec3[m_model->vertexCount];
	camera_ptr->set_camera();
	//camera_ptr->translate(-m_model->min);
	//camera_ptr->set_camera(position, view_dir, up);
	camera_ptr->translate(m_model->center * scale);
	camera_ptr->rotate(angle, glm::vec3(0, 1, 0));
	camera_ptr->translate(-m_model->center * scale);
	camera_ptr->scale(scale);
	//auto temp_center = camera_ptr->transform(m_model->center);
	//camera_ptr->translate(temp_center - glm::vec3(frame_buffer.w / 2, frame_buffer.h / 2, 0));
	//camera_ptr->translate((-scale * m_model->center + glm::vec3(frame_buffer.w / 2, frame_buffer.h / 2, 0)));
	//camera_ptr->translate(-position);
	
	for (int i = 0; i < m_model->vertexCount; i++)
	{
		auto v = camera_ptr->transform(m_model->positions[i]);
		//v = camera_ptr->pspt_prj(v);
		//v[0] = std::fmax(0.f, v[0]); v[1] = std::fmax(0.f, v[1]); ;
		vertex_buffer[i] = v;
	}
	is_change = true;
}

void Renderer::myPerspective(float fovy, float aspect, float znear, float zfar)
{
	camera_ptr->set_pspt_prj(fovy, aspect, znear, zfar);
}

void Renderer::render()
{
	if (is_change)
	{
		frame_buffer.setColor(255);
		scan_line.init(vertex_buffer, m_model->faces, m_model->colors);
		scan_line.scan(frame_buffer.getBuffer(), frame_buffer.w);
	}
}

void Renderer::init(PLYModel* m, Camera * c)
{
	m_model = m;
	camera_ptr = c;

	m_model->max -= m_model->min;
	m_model->center -= m_model->min;
	camera_ptr->set_camera();
	camera_ptr->translate(-m_model->min);
	for (int i = 0; i < m_model->vertexCount; i++)
	{
		auto v = camera_ptr->transform(m_model->positions[i]);
		m_model->positions[i] = v;
	}
}

