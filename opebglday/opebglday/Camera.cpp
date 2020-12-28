#include "Camera.h"

Camera::Camera()
{
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}

Camera::Camera(glm::vec3 Pos, glm::vec3 Front,glm::vec3 wordUp=glm::vec3(0.0f,1.0f,0.0f)):cameraPos(Pos),cameraFront(glm::normalize(Front))
{
	cameraRight = glm::normalize(glm::cross(cameraFront, wordUp));
	cameraUp= glm::normalize(glm::cross(cameraFront, cameraRight));
}

Camera::Camera(glm::vec3 Pos, double pitch,double yaw,glm::vec3 wordUp = glm::vec3(0.0f, 1.0f, 0.0f)):cameraPos(Pos),Pitch(pitch),Yaw(yaw)
{
	cameraFront.x = (float)glm::cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	cameraFront.y = (float)glm::sin(glm::radians(pitch));
	cameraFront.z = (float)glm::cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	glm::normalize(cameraFront);
	cameraRight = glm::normalize(glm::cross(cameraFront, wordUp));
	cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));
}

void Camera::processInput(GLFWwindow *window, float cameraSpeed)
{
	/*
	处理IO
	1、属性为当前上下文，2、摄像机对象，3、灵敏度
	**/
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


void Camera::UpdateMous(double pitch, double yaw){
	Pitch = pitch;
	Yaw = yaw;
	cameraFront.x = (float)glm::cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	cameraFront.y = (float)glm::sin(glm::radians(Pitch));
	cameraFront.z = (float)glm::cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	glm::normalize(cameraFront);
	cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));
}