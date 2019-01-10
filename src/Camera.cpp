#include "Camera.h"
#include <iostream>

Camera::Camera()
{
}


Camera::~Camera()
{
}

glm::vec3 Camera::transform(glm::vec3 v)
{
	/*
	//view_mat = glm::rotate(view_mat, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		view_mat[0][0], view_mat[0][1], view_mat[0][2], view_mat[0][3], 
		view_mat[1][0], view_mat[1][1], view_mat[1][2], view_mat[1][3],
		view_mat[2][0], view_mat[2][1], view_mat[2][2], view_mat[2][3],
		view_mat[3][0], view_mat[3][1], view_mat[3][2], view_mat[3][3]);
	*/
	//auto return_v = glm::vec4(v, 1.0f);
	//std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
	v = view_mat * glm::vec4(v, 1.0f);
	//std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
	//getchar();
	return v;
}


glm::vec3 Camera::pspt_prj(glm::vec3 v)
{
	glm::vec3 res_v = pspt_prj_mat * glm::vec4(v, 1.0f);
	res_v.z = v.z;
	/*if (res_v.x != 0 && res_v.y != 0)
	{
		std::cout << "input v : " << v.x << ", " << v.y << ", " << v.z << std::endl;
		std::cout << "output v : " << res_v.x << ", " << res_v.y << ", " << res_v.z << std::endl;
		getchar();
	}*/
	return res_v;
}
