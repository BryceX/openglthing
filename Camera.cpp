#include "Camera.h"
#include <glfw3.h>
#include "glm\glm\ext.hpp"

#include <iostream>

// initialize statics
double FlyCam::X_SCROLL = 0.0;
double FlyCam::Y_SCROLL = 0.0;

void FlyCam::scrollCallback(GLFWwindow * window, double xScroll, double yScroll)
{
	std::cout << yScroll << "\n";
	X_SCROLL = xScroll;
	Y_SCROLL = yScroll;
}

void Camera::Update(float deltaTime)
{

}
void Camera::setPerspective(float FOV, float aspectRatio, float near, float far)
{
	projectionTransform = glm::perspective(FOV, aspectRatio, near, far);
	
}
mat4 Camera::getView()
{
	return glm::inverse(worldTransform);
}

void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
	worldTransform = inverse(glm::lookAt(from, to, up));
}
void Camera::setPosition(vec3 position)
{
	worldTransform[3].xyz = position.xyz;
}
mat4 Camera::getProjectionView() 
{
	// return P * V * M
	return projectionTransform * getView();
}

void FlyCam::Init()
{
	// acquire the game windoqw
	auto window = glfwGetCurrentContext();

	// assign the scroll callback
	auto cb = glfwSetScrollCallback(window, scrollCallback);

	//Camera::Update(Camera * this, float deltaTime)
	//Camera::scrollCallback(GLFWwindow * window, double xScroll, double yScroll);
}

void FlyCam::SetSpeed(float a_speed)
{
	speed = a_speed;
}
void FlyCam::Move(int up, int down, int left, int right, double deltaTime)
{
	// acquire the game windoqw
	auto window = glfwGetCurrentContext();
	speed = 1;
	// check if W is pressed
	if (glfwGetKey(window, up) == GLFW_PRESS)
	{
		worldTransform[3].xyz += worldTransform[1].xyz * speed * deltaTime;
	}
	if (glfwGetKey(window, down) == GLFW_PRESS)
	{
		worldTransform[3].xyz -= worldTransform[1].xyz * speed * deltaTime;
	}
	if (glfwGetKey(window, left) == GLFW_PRESS)
	{ 
		worldTransform[3].xyz -= worldTransform[0].xyz * speed * deltaTime;
	}
	if (glfwGetKey(window, right) == GLFW_PRESS)
	{
		worldTransform[3].xyz += worldTransform[0].xyz * speed *deltaTime;
	}
}
void FlyCam::Zoom()
{
	if (Y_SCROLL >0)
	{
		worldTransform[3].xyz -= worldTransform[2].xyz*speed;
		Y_SCROLL = 0;
	}
	if (Y_SCROLL <0)
	{
		worldTransform[3].xyz += worldTransform[2].xyz*speed;
		Y_SCROLL = 0;
	}
	
}
void FlyCam::ChangeFOV() 
{

}






/*
Transformation Matrices:


	transform:
	     Affine transformation matrix

		[rx ux fx px]
		[ry	uy fy py]
		[rz	uz fz pz]
		[ 0  0	0  1]

		[rx ry rz 0]
		[ux uy uz 0]
		[fx fy fz 0]
		[px py pz 1]

		right    = transform[0]
		up       = transform[1]
		forward  = transform[2]
		position = transform[3]


		
		
		
		position += velocity * dt;
	*/