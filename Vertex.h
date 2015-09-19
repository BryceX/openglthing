#pragma once
#include <iostream>
#include "gl_core_4_4.h"
#include <glfw3.h>
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "FBXFile.h"
#include <fstream>


struct RenderObject
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;

	unsigned int indexCount;
	
	
	void Draw();
	FBXFile* m_FBX;
	unsigned int program;
};
unsigned int loadSubShader(unsigned int type, const char* path);
RenderObject MakeGrid(unsigned int rows, unsigned int columns);
RenderObject LoadFBX(const char* fileName);
unsigned int loadShader(const char* v_path, const char* f_path);
void CreateOpenGLBuffers(FBXFile* name);
void DeleteOpenGLBuffers(FBXFile* name);

struct Vertex 
{
	glm::vec4 position;
	glm::vec4 colour;
};


// LoadTexture	- OK!
// LoadShader	- OK!
// LoadFBX		- OK!
// LoadObj		- don't *need* but recommended