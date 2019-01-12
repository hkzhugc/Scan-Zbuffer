#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/type_ptr.hpp>
class Camera
{
public:
	Camera(){};
	~Camera(){};
	inline void set_camera(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 view_dir = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 0, 0)) 
	{
		_position = position;
		_view_dir = view_dir;
		_up = up;
		float mat_src[16] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			-_position.x, -_position.y, -_position.z, 1
		};//simple translate, no rotation
		view_mat = glm::make_mat4(mat_src);
		//view_mat = glm::translate(view_mat, position);
	}
	inline void scale(float scale_fac)
	{
		view_mat = glm::scale(view_mat, glm::vec3(scale_fac, scale_fac, scale_fac));
	}
	inline void rotate(float angle, glm::vec3 axis)
	{
		view_mat = glm::rotate(view_mat, angle, axis);
	}
	inline void translate(glm::vec3 position)
	{
		view_mat = glm::translate(view_mat, position);
	}
	inline void set_pspt_prj(float fovy, float aspect, float znear, float zfar)
	{
		float ymax = znear * tanf(glm::radians(fovy));
		float xmax = ymax * aspect;

		float temp, temp2, temp3, temp4;
		temp = 2.0f * znear;
		temp2 = 2.0f * xmax;
		temp3 = 2.0f * ymax;
		temp4 = zfar - znear;
		float mat_src[16] = {
			temp / temp2, 0, 0, 0,
			0, temp / temp3, 0, 0,
			0, 0, (-zfar - znear) / temp4, -1,
			0, 0, (-temp * zfar) / temp4, 0
		};
		pspt_prj_mat = glm::make_mat4(mat_src);
	}
	glm::vec3 transform(glm::vec3 v);
	glm::vec3 pspt_prj(glm::vec3 v);
private:
	glm::vec3 _position;
	glm::vec3 _view_dir;
	glm::vec3 _up;
	glm::mat4x4 view_mat;
	glm::mat4x4 pspt_prj_mat;
};

