#pragma once
#include <iostream>
#define GLM_SWIZZLE
#include <math.h>
#include <glfw3.h>
#include "glm\glm\glm.hpp"
#include "glm\glm\ext.hpp"


using glm::mat4;
using glm::vec3;

class Camera
{
protected:
	
	
	//mat4 viewTransform; // this can be derived by getting the inverse of the worldTransform
	// the lens of the camera, or how the camera views the world (e.g. perspective / ortho)
	//void updateProjectionViewTransform();//mat4 projectionViewTransform; // this can be calculated by multiplying the projection and view matricies

public:
	virtual void Update(float deltaTime);	// = 0 because this is an abstract function to be filled by classes that inherit from this
	virtual void setPerspective(float FOV, float aspectRatio, float near, float far);
	virtual void setLookAt(vec3 from, vec3 to, vec3 up);
	virtual void setPosition(vec3 position);
	//mat4 getWorldTransform();
	mat4 getView();
	//mat4 getProjection();
	mat4 getProjectionView();
	mat4 projectionTransform;
	mat4 worldTransform;
	



	/*int upKey = GLFW_KEY_W;
	int downKey = GLFW_KEY_S;
	int leftKey = GLFW_KEY_A;
	int rightKey = GLFW_KEY_D;*/

};

class GLFWwindow;

class FlyCam : public Camera
{
protected:
	float speed = 1;
	vec3 up = vec3(0,1,0);

public:
	//virtual void Update(float deltaTime);
	
	virtual void SetSpeed(float speed);
	void Init();
	void Move(int up, int down, int left, int right, double deltaTime);
	void Zoom();
	void ChangeFOV();
	static void scrollCallback(GLFWwindow * window, double xScroll, double yScroll);

	static double X_SCROLL;
	static double Y_SCROLL;
	
};