#include <algorithm>
#include "ScanLine.h"


EdgeElement::EdgeElement(glm::vec3 p1, glm::vec3 p2, int _face_id)
{
	face_id = _face_id;
	if (p1.y < p2.y)
	{
		ymax = p2.y;
		x = p2.x;
		dy = p2.y - p1.y;
		dx = (p1.x - p2.x) / dy;
	}
	else if (p2.y < p1.y)
	{
		ymax = p1.y;
		x = p1.x;
		dy = p1.y - p2.y;
		dx = (p2.x - p1.x) / dy;
	}
	else
	{
		ymax = p1.y;
		x = p1.x;
		dy = p1.y - p2.y;
		dx = 0;
	}
}

TriangleElements::TriangleElements(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int _face_id, glm::vec3 _color) 
	: face_id(_face_id), is_in(false), color(_color)
{
	glm::vec3 p1p2 = p2 - p1;
	glm::vec3 p2p3 = p3 - p1;
	glm::vec3 normal = glm::cross(p1p2, p2p3);
	normal = glm::normalize(normal);
	a = normal.x; b = normal.y; c = normal.z;
	d = a * p1.x + b * p1.y + p1.z;
}

void EdgeRow::insert(EdgeElement e)
{
	//if (edge_row.empty())
	//{
	//	edge_row.push_back(e);
	//	return;
	//}
	for (auto it = edge_row.begin(); it != edge_row.end(); ++it)
	{
		if (e < *(it))
		{
			edge_row.insert(it, e);
			return;
		}
	}
	edge_row.push_back(e);
}


void ScanLine::init(glm::vec3 * vertex_buffer, vector<glm::ivec3>& faces, vector<glm::vec3>& colors)
{
	size_t face_id = 0;
	PT.resize(faces.size());
	for (auto& er : ET)
	{
		er.clear();
	}
	for (auto f : faces)
	{
		glm::vec3 p1, p2, p3;
		p1 = vertex_buffer[f[0]];
		p2 = vertex_buffer[f[1]];
		p3 = vertex_buffer[f[2]];
		EdgeElement e1(p1, p2, face_id);
		EdgeElement e2(p2, p3, face_id);
		EdgeElement e3(p3, p1, face_id);
		ET[e1.getYmax()].add(e1);
		ET[e2.getYmax()].add(e2);
		ET[e3.getYmax()].add(e3);

		TriangleElements t(p1, p2, p3, face_id, colors[face_id]);
		PT[face_id] = t;
		face_id++;
	}
}

const int ScanLine::closest_face_id(float x, float y)
{
	float min_z = FLT_MAX;
	int face_id = -1;
	for (auto pair : IPL)
	{
		float z = pair.second->getZvalue(x, y);
		if (z < min_z)
			face_id = pair.first;
	}
	return face_id;
}

//scan 1 line, and set the frame_buffer
void ScanLine::scan_one_line(GLubyte * frame_buffer, size_t x_res)
{
	IPL.clear();
	auto e1 = AET.getE();
	set_PT_flag(e1);
	for (auto e2 = AET.getE(); e2 != AET.End();)
	{
		int face_id = closest_face_id((*e1).x + (*e2).x / 2, scan_y);
		if(face_id >= 0)
			render_seg(frame_buffer, (*e1).x, (*e2).x, PT[face_id].getColor());
		//if e2 face is in, that means we are leaving the triangle
		e1 = e2;
		e2 = AET.getE();
		set_PT_flag(e1);
	}
}

//after scan line scan_y, update the AET, including adding new edges and remove the edge
void ScanLine::update_AET()
{
	AET.update();
}

void ScanLine::update_IPL()
{
	for (auto it = IPL.begin(); it != IPL.end(); ++it)
	{

		if (!PT[(*it).first].is_in)
		{
			IPL.erase(it);
		}
	}
}

void ScanLine::scan(GLubyte * frame_buffer, size_t x_res)
{
	for (; scan_y >= 0; scan_y--)
	{
		if (!ET[scan_y].empty())
		{
			for (auto e : ET[scan_y].edge_row)
			{
				AET.insert(e);
			}
		}
		GLubyte * frame_buffer_line = &frame_buffer[3 * scan_y * x_res];
		if (AET.is_update || !ET[scan_y].empty())
			scan_one_line(frame_buffer_line, x_res);
		else
			;//
	}
}

void ScanLine::render_seg(GLubyte * frame_buffer, int x1, int x2, const glm::vec3& color)
{
	for (int i = x1; i < x2; i++)
	{
		GLubyte* frame = &frame_buffer[3 * i];
		frame[0] = color[0];
		frame[1] = color[1];
		frame[2] = color[2];
	}
}

void ScanLine::set_PT_flag(list<EdgeElement>::iterator e)
{
	int face_id = (*e).face_id;
	PT[face_id].is_in++;
	if (PT[face_id].is_in)
	{
		IPL[face_id] = &PT[face_id];
	}
	//it means that e is the second edge
	else
	{
		auto it = IPL.find(face_id);
		assert(it != IPL.end());
		IPL.erase(it);
	}
}
