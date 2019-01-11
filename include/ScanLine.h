#pragma once
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <glm/glm.hpp>
#include <gl/glut.h>

//#define DEBUG_SCAN
//#define PRINT_IPL
using namespace std;
typedef glm::u8vec3 Color;
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
	bool operator<(const EdgeElement& e)
	{
		return x < e.x;
	}
	bool operator>(const EdgeElement& e)
	{
		return x > e.x;
	}
	bool operator==(const EdgeElement& e)
	{
		return x == e.x;
	}
	~EdgeElement() {}
	float x;
	int face_id;
	int dy;
	float dx;
private:
	float ymax;
};

class TriangleElements
{
public:
	TriangleElements(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int face_id, Color _color);
	TriangleElements() {}
	~TriangleElements() {}
	inline const Color& getColor()
	{
		float _c = (c + 1.f) / 2.f;
		return Color(255 * _c, 255 * _c, 255 * _c);
		//return color;
	}
	inline float getZvalue(float x, float y) 
	{
		if (c == 0) return -FLT_MAX; //assert all z should less than zero
		return (d - a * x - b * y) / c;
	}
	inline void print_normal()
	{
		printf("face%d normal = %f %f %f\n", face_id, a, b, c);
	}
	bool is_in;
private:
	float a, b, c, d;
	int face_id;
	//int dy;
	Color color;
};

class EdgeRow
{
public:
	EdgeRow() { iter = edge_row.begin(); };
	~EdgeRow() {};
	void clear() { edge_row.clear(); }
	bool empty() { return edge_row.empty(); }
	inline void reset() { iter = edge_row.begin(); }
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
		for (auto &e : edge_row)
		{
			e.update();
		}
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
		edge_row.sort();
	}
	void print()
	{
		printf("size = %d\n", edge_row.size());
		for (auto e : edge_row)
		{
			printf("e belong to %d face, intersect with %f, remain %d scanline, dx = %f\n", e.face_id, e.x, e.dy, e.dx);
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
	ScanLine(int _yres) : ET(_yres), BG_color(255, 255, 255), yres(_yres){ };
	~ScanLine() {};
	void init(glm::vec3 * vertex_buffer, vector<glm::ivec3> &faces, vector<Color>& colors);
	const int closest_face_id(float x, float y);
	void scan_one_line(GLubyte * frame_buffer, size_t x_res, bool is_rerender = false);
	void update_AET();
	void update_IPL();
	void scan(GLubyte * frame_buffer, size_t x_res);
	void reset(int _yres)
	{
		ET.clear();
		ET.resize(_yres);
		yres = _yres;
	}
	void printIPL()
	{
#ifdef PRINT_IPL
		printf("\n-----printIPL begin-----\n");
		for (auto p : IPL)
		{
			printf("face: %d\n", p.first);
		}
		printf("-----printIPL   end-----\n\n");
#endif
	}
private:
	void render_seg(GLubyte * frame_buffer, int x1, int x2, const Color& color);
	void rerender_seg();
	void set_PT_flag(list<EdgeElement>::iterator e);
	inline void clearIPL()
	{
		for (auto p : IPL)
		{
			p.second->is_in = false;//set IPL to be idle
		}
		IPL.clear();
	}
	EdgeRow AET;
	map<int, TriangleElements*> IPL;
	vector<EdgeRow> ET;
	vector<TriangleElements> PT;
	vector<Color> last_color;
	Color BG_color;
	int scan_y;
	int yres;
};

