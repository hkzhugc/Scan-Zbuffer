#pragma once
#include <gl/glut.h>
#include "plyloader.h"
#include "Camera.h"
#include "ScanLine.h"
#define RES 600
class FrameBuffer
{
public:
	FrameBuffer() : w(0), h(0){};
	FrameBuffer(size_t _w, size_t _h) : w(_w), h(_h), data(_w * _h * 3, 255){}
	~FrameBuffer() {}
	void reset(size_t _w, size_t _h) 
	{
		w = _w; h = _h;
		data.clear();
		data.resize(_w * _h * 3, 255);
	}
	GLubyte* getBuffer() { return &data[0]; }
	void setColor(GLubyte color) { memset(&data[0], color, data.size()); }
	vector<GLubyte> data;
	size_t w;
	size_t h;
};

class Renderer
{
public:
	Renderer();
	~Renderer();
	void init(PLYModel* m, Camera * c);
	void change_view(float angle = 0.0f, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 view_dir = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 0, 0));
	void myPerspective(float fovy, float aspect, float znear, float zfar);
	void render();
	void reset(int _w, int _h)
	{
		frame_buffer.reset(_w, _h);
		scan_line.reset(_h);
		is_change = true;
	}
	FrameBuffer frame_buffer;
private:
	glm::vec3 * vertex_buffer;
	Camera * camera_ptr;
	PLYModel* m_model;
	bool is_change;
	ScanLine scan_line;
};



