#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	glm::vec3 cameraPos ;
	glm::vec3 cameraFront ;
	glm::vec3 cameraUp ;
	glm::vec3 cameraRight;
	double Pitch;
	double Yaw;
public:
	Camera();
	Camera(glm::vec3 Pos, glm::vec3 Front, glm::vec3 wordUp);
	Camera(glm::vec3 Pos, double pitch, double yaw, glm::vec3 wordUp);
	void processInput(GLFWwindow *window,float cameraSpeed);
	void UpdateMous(double pitch, double yaw);
};

