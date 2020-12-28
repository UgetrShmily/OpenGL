#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Camera
{
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	float Yaw;
	float Pitch;
	float Roll;
	float Fov;
	glm::mat4 projection;
	glm::mat4 view;
	float Width;
	float Height;
private:
	float deltaTime ; // 当前帧与上一帧的时间差
	float lastFrame ; // 上一帧的时间
public:
	Camera();
	Camera(glm::vec3 cameraPos, float cameraYaw, float cameraPitch);
	void processInput(GLFWwindow *window);//键盘事件监听
	void updata(float yaw, float pitch,float roll);
private:
	void setProjection(float near, float far, float fov, float width, float height);
	void lookAt(float roll);
};

