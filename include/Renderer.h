#pragma once
#include <gl/glut.h>
#include "plyloader.h"
#include "Camera.h"
class Renderer
{
public:
	Renderer();
	~Renderer();
	void init(PLYModel* m, Camera * c);
	void start_render(int *argcp, char **argv);
	void setDisplayFunc(void(*func)(void)) { display_func = func; }
	void setReshapeFunc(void(*func)(int width, int height)) { reshape_func = func; }
	void change_view(float angle = 0.0f, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 view_dir = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 0, 0));
	void myPerspective(float fovy, float aspect, float znear, float zfar);
	void render_face(int f_idx);
	void render_face();
private:
	void(*display_func) (void);// store the display_func
	void(*reshape_func) (int width, int height);
	glm::vec3 * vertex_buffer;
	Camera * camera_ptr;
	PLYModel* m_model;
	//PLYModel* m_model;
};

void defalut_display_func();
void defalut_reshape_func(int width, int height);


