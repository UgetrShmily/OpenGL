#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Camera::processInput(GLFWwindow *window) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 5.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Pos += cameraSpeed * Front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Pos -= cameraSpeed * Front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Pos -= Right* cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Pos += Right * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		Roll += cameraSpeed*100;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		Roll -=cameraSpeed*100;
	updata(Yaw, Pitch,Roll);
}
Camera::Camera():Yaw(-90.0f), Pitch(0.0f),deltaTime(0.0f),lastFrame(0.0f)
{
	Pos = glm::vec3(0.0f,0.0f,3.0f);
	Front = glm::vec3(0.0f,0.0f,-1.0f);
	Up = glm::vec3(0.0f,1.0f,0.0f);
	Right = glm::vec3(1.0f,0.0f,0.0f);
	setProjection(0.1f, 100.0f, 45.0f, Width, Height);
	lookAt(0.0f);
}
Camera::Camera(glm::vec3 cameraPos, float cameraYaw, float cameraPitch)
	:Pos(cameraPos), Yaw(cameraYaw), Pitch(cameraPitch),deltaTime(0.0f), lastFrame(0.0f)
{
	Front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	Front.y = sin(glm::radians(Pitch));
	Front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Front = glm::normalize(Front);
	glm::vec3 wolrd(0.0f, 1.0f, 0.0f);
	Right = glm::normalize(glm::cross(Front, wolrd));
	Up = glm::cross(Right, Front);
	setProjection(0.1f, 100.0f, 45.0f, Width, Height);
	lookAt(0.0f);
}
void Camera::updata(float yaw,float pitch,float roll)
{
	Yaw = yaw; Pitch = pitch; 
	Front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)); 
	Front.y = sin(glm::radians(Pitch));
	Front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Front = glm::normalize(Front);
	Right =glm::normalize(glm::cross(Front, glm::vec3(0.0f,1.0f,0.0f)));
	Up = glm::normalize(glm::cross(Front,Right));
	//std::cout << "Front:\t" << Front.x << "\t" << Front.y << "\t" << Front.z << std::endl;
	//std::cout << "Right:\t" << Right.x << "\t" << Right.y << "\t" << Right.z << std::endl;
	//std::cout << "Up:\t" << Up.x << "\t" << Up.y << "\t" << Up.z << std::endl;
	//std::cout << "Yaw:\t" << Yaw << "\t\n" << "Pitch:\t" << Pitch << "\t\n" << "Roll:\t" << Roll << std::endl;
	setProjection(0.1f, 100.0f, Fov, Width, Height);
	lookAt(roll);
}
void Camera::lookAt(float rool) {
	Roll = rool;
	glm::vec4 _Up(0.0f, 1.0f, 0.0f, 1.0f);
	Up = _Up * glm::rotate(glm::mat4(1.0), glm::radians(rool), Front);
	view=glm::lookAt(Pos, Front + Pos, Up);//Ê¼ÖÕ¿´×ÅFront
}
void Camera::setProjection(float near, float far,float fov, float width, float height) {
	Fov = fov;
	projection = glm::perspective(glm::radians(fov), width / height, near, far);
}