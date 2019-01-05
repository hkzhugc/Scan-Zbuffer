#include <iostream>
#include <stdio.h>
#include "gl/glew.h"

int main()
{
	GLuint fbo;
	//std::cout << fbo << std::endl;
	glGenFramebuffers(1, &fbo);
	//std::cout << "done" << std::endl;
	return 1;
}