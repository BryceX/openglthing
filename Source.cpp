#include <iostream>
#include "gl_core_4_4.h"
#include <glfw3.h>
#include "Gizmos.h"
#include <math.h>
#include <tiny_obj_loader.h>
#define GLM_SWIZZLE
//#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <stb_image.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Vertex.h"
#include "Camera.h"
#include "FBXFile.h"


using glm::vec3;
using glm::vec4;
using glm::mat4;
float deltaTime = 0;

Vertex testVert;

unsigned int programID;

std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;





//std::string err = tinyobj::LoadObj(shapes, materials, "./Objs/dragon.obj");

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_index_count;
};

std::vector<OpenGLInfo> m_gl_info;

void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{

	m_gl_info.resize(shapes.size());

	for (unsigned int mesh_index = 0; mesh_index < shapes.size(); ++mesh_index)
	{
		unsigned int fCount = shapes[mesh_index].mesh.positions.size();

		glGenVertexArrays(1, &m_gl_info[mesh_index].m_VAO);
		glGenBuffers(1, &m_gl_info[mesh_index].m_VBO);
		glGenBuffers(1, &m_gl_info[mesh_index].m_IBO);
		glBindVertexArray(m_gl_info[mesh_index].m_VAO);
		unsigned int float_count = shapes[mesh_index].mesh.positions.size();
		float_count += shapes[mesh_index].mesh.normals.size();
		float_count += shapes[mesh_index].mesh.texcoords.size();
		std::vector<float> vertex_data;
		vertex_data.reserve(float_count);
		vertex_data.insert(vertex_data.end(),
			shapes[mesh_index].mesh.positions.begin(),
			shapes[mesh_index].mesh.positions.end());
		vertex_data.insert(vertex_data.end(),
			shapes[mesh_index].mesh.normals.begin(),
			shapes[mesh_index].mesh.normals.end());
		m_gl_info[mesh_index].m_index_count =
			shapes[mesh_index].mesh.indices.size();
		glBindBuffer(GL_ARRAY_BUFFER, m_gl_info[mesh_index].m_VBO);
		glBufferData(GL_ARRAY_BUFFER,
			vertex_data.size() * sizeof(float),
			vertex_data.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_info[mesh_index].m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int),
			shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
			(void*)(sizeof(float)*shapes[mesh_index].mesh.positions.size()));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	}
}


void MakeView(GLFWwindow* a, double deltaTime)
{
	glm::vec3 position = vec3(10, 10, 10);
	mat4 viewTransform = glm::lookAt(position, vec3(0), vec3(0, 1, 0));
	mat4 projectionTransform = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	mat4 worldTransform = glm::inverse(viewTransform);
	float speed = 1;
}


void GetDeltaTime()
{
	deltaTime = glfwGetTime();
	glfwSetTime(0);

}

int main()
{

	//Application* myApp = new Application();
	/*if (myApp->Init() == true)
	{
		
			while (myApp->Update() == true)
				myApp->Draw();
				myApp->Shut();
				


	}
	delete myApp;
	return 0;*/

	// Window Creation Code
		if (glfwInit() == false)
		{
			return-1;
		}

		


		GLFWwindow* window = glfwCreateWindow(1280, 720, "test", nullptr, nullptr);
		if (window == nullptr) {
			glfwTerminate();
			return -2;
		}
		glfwMakeContextCurrent(window);
		if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
			glfwDestroyWindow(window);
			glfwTerminate();
			return -3;
		}
		auto major = ogl_GetMajorVersion();
		auto minor = ogl_GetMinorVersion();
		printf("GL: %i.%i\n", major, minor);



			//const char* vsSource = "#version 410\n  in vec4 Position;  in vec4 Colour; out vec4 vColour; uniform mat4 ProjectionView; uniform float time; uniform float heightScale; void main(){vColour = Colour; vec4 P = Position; P.y += cos(time + Position.x)*heightScale; gl_Position = ProjectionView*P; }";
			//char* fsSource = "#version 410\n \in vec4 vColour; \ out vec4 FragColor; \  void main() {FragColor = vColour; FragColor *= vec4(0,.75,.5,1);}";


		// Loading a plane

		// Position     TexCoords
		// X, Y, Z, W,  S,T
		float vertexData[] = { -5, 0, 5, 1,  0,1,
							   5, 0, 5, 1,  1,1,
							   5, 0,-5, 1,  1,0,
							  -5, 0,-5, 1,  0,0, };


		unsigned int indexData[] = { 0,1,2,
									 0,2,3, };

		unsigned int m_vao;
		unsigned int m_ibo;
		unsigned int m_vbo;

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4,
			vertexData, GL_STATIC_DRAW);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
			indexData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 6, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			sizeof(float) * 6, ((char*)0) + 16);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




		// Shader Creation

		const char* vsSource = "#version 410\n \
								layout(location=0) in vec4 Position; \
								layout(location=1) in vec2 TexCoord; \
								out vec2 vTexCoord; \
								uniform mat4 ProjectionView;\
								void main() { \
								vTexCoord = TexCoord;\
								gl_Position = ProjectionView * Position; \
								}";

		const char* fsSource = "#version 410\n \
								in vec2 vTexCoord; \
								out vec4 FragColor; \
								uniform sampler2D diffuse; \
								void main(){ \
								FragColor = texture(diffuse,vTexCoord); \
								}";

		int success = GL_FALSE;
		//taking memory on gpu for shader stuff
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//compiling
		glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
		glCompileShader(vertexShader);
		glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
		glCompileShader(fragmentShader);
		programID = glCreateProgram();
		//attaching shader onto program
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);
		glLinkProgram(programID);
		glGetProgramiv(programID, GL_LINK_STATUS, &success);

		if (success == GL_FALSE)
		{
			int infoLogLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];
			glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
			printf("Error: Failed to link shader program!\n");
			printf("%s\n", infoLog);
			delete[] infoLog;
		}

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		Gizmos::create();




		float time = 0;
		glClearColor(0.25f, 0.25f, 0.25f, 1);
		glEnable(GL_DEPTH_TEST);

		FlyCam planetView;
		planetView.Init();
		planetView.setPerspective(glm::pi<float>()*0.25f, 1280 / 720.f, 0.1f, 1000.f);
		planetView.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
		planetView.setPosition(vec3(10, 10, 10));
		GetDeltaTime();

		

		const int rows = 12;
		const int cols = 12;

		//MakeGrid(rows, cols);

		//RenderObject grid = MakeGrid(rows, cols);
		//createOpenGLBuffers(shapes);

		// Texture Loading

		
		unsigned int m_texture = LoadTexture("./textures/crate.png");;
		
		FBXFile temp;
		temp.load("./FBX/soulspear/soulspear.fbx", FBXFile::UNITS_CENTIMETER);
		FBXMeshNode * mesh = temp.getMeshByIndex(0);
		MakeGrid(5, 5);

		/*Application temp;
		temp.Init();*/

		
		while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//time = glfwGetTime();

			glUseProgram(programID);
			
			//float timehaspassed = glGetUniformLocation(programID, "time");
			//float height = glGetUniformLocation(programID, "heightScale");

			//glUniform1f(timehaspassed, time);
			//glUniform1f(height, 1);


			//find a way to get these two lines  into init
			unsigned int projectionViewUniform = glGetUniformLocation(programID, "ProjectionView");
			glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(planetView.getProjectionView()));



			//glBindVertexArray(grid.VAO);
			//unsigned int indexCount = (rows - 1) * (cols - 1) * 6;
			//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



			planetView.Move(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, deltaTime);
			planetView.Zoom();



			projectionViewUniform = glGetUniformLocation(programID, "diffuse");
			glUniform1i(projectionViewUniform, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);





			/*for (unsigned int i = 0; i < m_gl_info.size(); ++i)
			{
				glBindVertexArray(m_gl_info[i].m_VAO);
				glDrawElements(GL_TRIANGLES, m_gl_info[i].m_index_count, GL_UNSIGNED_INT, 0);
			}*/






















			/*Gizmos::clear();
			Gizmos::addTransform(glm::mat4(1));
			vec4 white(1);
			vec4 black(0, 0, 0, 1);
			for (int i = 0; i < 21; i++)
			{
				Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
				Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
			}

			mat4 spin = glm::rotate(time /30,vec3(0,1,0));

			Gizmos::addSphere(vec3(),1,24,24,vec4(255,205,0,1),&spin);

			mat4 planet = glm::rotate(time/50, vec3(0, .8f, 0))
						* glm::translate(vec3(5,0,0))
						* glm::rotate(time/40, vec3(0, 1, 0));

			Gizmos::addSphere(vec3(), .5, 24, 24, vec4(0, 56, 30, 1), &planet);


			mat4 moon = glm::translate(vec3(planet[3].xyz))
					  * glm::rotate(time/2, vec3(0, .8f, 0))
					 * glm::translate(vec3(3, 0, 0))
					  * glm::rotate(time, vec3(0, 1, 0));

			Gizmos::addSphere(vec3(), .2, 24, 24, vec4(0, 0, 0, 1), &moon);

			mat4 satellite = glm::translate(vec3(moon[3].xyz))
				* glm::rotate(time/90, vec3(.25f, 0, 0))
				*glm::translate(vec3(2, 1, 0));

			Gizmos::addSphere(vec3(), .01, 10, 10, vec4(0, 200, 50, 1), &satellite);


			Gizmos::draw(planetView.projectionTransform*planetView.getView());*/





			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		Gizmos::destroy();
		glfwTerminate();
		glfwDestroyWindow(window);

		return 0;
	


}
