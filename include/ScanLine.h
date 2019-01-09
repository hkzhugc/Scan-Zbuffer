#pragma once
#include <list>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <gl/glut.h>

using namespace std;

class EdgeElement
{
public:
	EdgeElement(glm::vec3 p1, glm::vec3 p2, int _face_id);
	inline int getYmax() { return (int)ymax; }
	inline void update() 
	{
		dy--;
		x += dx;
	}
	inline bool operator<(const EdgeElement e)
	{
		return x < e.x;
	}
	~EdgeElement() {}
	float x;
	int face_id;
	int dy;
private:
	float ymax;
	float dx;
};

class TriangleElements
{
public:
	TriangleElements(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int face_id, glm::vec3 _color);
	TriangleElements() {}
	~TriangleElements() {}
	inline const glm::vec3& getColor() 
	{
		return color;
	}
	inline float getZvalue(float x, float y) 
	{
		if (c == 0) return 1; //assert all z should less than zero
		return -(a * x + b * y + d) / c;
	}
	bool is_in;
private:
	float a, b, c, d;
	int face_id;
	//int dy;
	glm::vec3 color;
};

class EdgeRow
{
public:
	EdgeRow() { iter = edge_row.begin(); };
	~EdgeRow() {};
	void clear() { edge_row.clear(); }
	bool empty() { return edge_row.empty(); }
	inline list<EdgeElement>::iterator getE()
	{
		list<EdgeElement>::iterator ret = iter;
		if (iter != edge_row.end())
			++iter;
		return ret;
	}
	inline list<EdgeElement>::iterator End()
	{
		return edge_row.end();
	}
	inline void add(EdgeElement e) { edge_row.push_back(e); }
	void insert(EdgeElement e);
	inline void update()
	{
		is_update = false;
		for (auto it = edge_row.begin(); it != edge_row.end();)
		{
			auto tmp = it;
			it++;
			if ((*tmp).dy == 0)
			{
				edge_row.erase(tmp);
				is_update = true;
			}
		}
		for (auto &e : edge_row)
		{
			e.update();
		}
	}
	bool is_update;
	list<EdgeElement> edge_row;
private:
	list<EdgeElement>::iterator iter;
};


class ScanLine
{
public:
	ScanLine(int yres) : ET(yres) { scan_y = yres - 1; };
	~ScanLine() {};
	void init(glm::vec3 * vertex_buffer, vector<glm::ivec3> &faces, vector<glm::vec3>& colors);
	const int closest_face_id(float x, float y);
	void scan_one_line(GLubyte * frame_buffer, size_t x_res);
	void update_AET();
	void update_IPL();
	void scan(GLubyte * frame_buffer, size_t x_res);
private:
	void render_seg(GLubyte * frame_buffer, int x1, int x2, const glm::vec3& color);
	void set_PT_flag(list<EdgeElement>::iterator e);
	EdgeRow AET;
	map<int, TriangleElements*> IPL;
	vector<EdgeRow> ET;
	vector<TriangleElements> PT;
	EdgeRow Left;
	EdgeRow Right;
	int scan_y;
};

