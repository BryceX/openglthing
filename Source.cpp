#include <iostream>
#include "gl_core_4_4.h"
#include <glfw3.h>
#include "Gizmos.h"
#include <math.h>
#define GLM_SWIZZLE
#include "glm/glm/glm.hpp"
#include "glm/glm/ext.hpp"
#include "Vertex.h"
#include "Camera.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
float deltaTime = 0;
float currentTime = glfwGetTime();
Vertex testVert;

unsigned int VAO;
unsigned int VBO;
unsigned int IBO;
unsigned int programID;

void GenerateGrid(unsigned int rows, unsigned int columns)
{
	Vertex* vertices = new Vertex[rows*columns];
	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < columns; c++)
		{
			vertices[r*columns + c].position = glm::vec4((float)c, 0, (float)r, 1);
			glm::vec3 colour = glm::vec3(sinf((c / (float)(columns - 1))*(r / (float)(rows - 1))));
			vertices[r * columns + c].colour = glm::vec4(colour, 1);
		}
	}
	unsigned int* auiIndices = new unsigned int[(rows - 1)*(columns - 1) * 6];
	unsigned int index = 0;

	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (columns - 1); ++c)
		{
			auiIndices[index++] = r*columns + c;
			auiIndices[index++] = (r + 1) * columns + c;
			auiIndices[index++] = (r + 1) * columns + (c + 1);
			auiIndices[index++] = r * columns + c;
			auiIndices[index++] = (r + 1) * columns + (c + 1);
			auiIndices[index++] = r * columns + (c + 1);

		}
	}

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows*columns)*sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (columns - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	
	glBindVertexArray(0);	// unbind the VAO by setting to zero, which is NULL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the VBO



	
	
	//
	

	//



	//// populate VBO
	
	
	

	//// populate IBO
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	
	
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the IBO



	delete[] auiIndices;
	delete[] vertices;
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


	
		const char* vsSource = "#version 410\n  in vec4 Position;  in vec4 Colour; out vec4 vColour; uniform mat4 ProjectionView; uniform float time; uniform float heightScale; void main(){vColour = Colour; vec4 P = Position; P.y += sin(time + Position.x)*heightScale; gl_Position = ProjectionView*P; }";
		
		
		
		char* fsSource = "#version 410\n \in vec4 vColour; \ out vec4 FragColor; \  void main() {FragColor = vColour;}";
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

		GenerateGrid(rows, cols);

		while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			time = glfwGetTime();
			
			glUseProgram(programID);
			unsigned int projectionViewUniform = glGetUniformLocation(programID, "ProjectionView"); //hotdog
			float deltatime = glGetUniformLocation(programID, "time");
			float height = glGetUniformLocation(programID, "heightScale");
			deltatime += deltaTime;
			height = 4;
			
			glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(planetView.getProjectionView()));
			glBindVertexArray(VAO);
			unsigned int indexCount = (rows - 1)*(cols - 1) * 6;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		


			planetView.Move(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, deltaTime);
			planetView.Zoom();
			
			/*Gizmos::clear();
			Gizmos::addTransform(glm::mat4(1));
			vec4 white(1);
			vec4 black(0, 0, 0, 1);
			for (int i = 0; i < 21; i++)
			{
				Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
				Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
			}*/
			/*
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
			
			
			Gizmos::draw(planetView.projectionTransform*planetView.getView());

			*/



			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		Gizmos::destroy();
		glfwTerminate();
		glfwDestroyWindow(window);
		
	return 0;
	


}
