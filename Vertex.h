#pragma once
#include <iostream>
#include "gl_core_4_4.h"
#include <glfw3.h>
#include "glm\glm\glm.hpp"
#include "glm\glm\ext.hpp"


struct RenderObject
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
};

struct Vertex 
{
	glm::vec4 position;
	glm::vec4 colour;
};
RenderObject MakeGrid(unsigned int rows, unsigned int columns);