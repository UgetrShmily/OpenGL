#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Model.h"
#include "Mesh.h"

Camera *camera;
//窗口大小改变回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	camera->Width = width;
	camera->Height = height;
	camera->updata(camera->Yaw, camera->Pitch, camera->Roll);
}

float lastX = 400, lastY = 300;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏鼠标
	if (firstMouse) // 这个bool变量初始时是设定为true的
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	camera->Yaw += xoffset;
	camera->Pitch += yoffset;
	if (camera->Pitch > 89.5f)
		camera->Pitch = 89.5f;
	if (camera->Pitch < -89.5f)
		camera->Pitch = -89.5f;
	camera->updata(camera->Yaw,camera->Pitch, camera->Roll);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (camera->Fov >= 1.0f && camera->Fov <= 45.0f)
		camera->Fov -= yoffset;
	if (camera->Fov <= 1.0f)
		camera->Fov = 1.0f;
	if (camera->Fov >= 45.0f)
		camera->Fov = 45.0f;
	camera->updata(camera->Yaw, camera->Pitch, camera->Roll);
}


int main() {

	cout << "输入文件地址类似：" << "D:/model/model.obj" << endl;
	string path;
	cin >> path;

	//glfw初始化和opengl版本配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	//开窗
	GLFWwindow *window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();//关闭资源
		return -1;
	}
	glfwMakeContextCurrent(window);//set设备上下文
	//初始化GLAD,在调用openGL函数前必须初始化查询工具
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	camera = new Camera();
	
	//映射窗口
	//glViewport(0,0,800,600);
	//回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//窗口大小改变回调函数
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader myshader("modelloading.vs", "modelloading.fs");
	myshader.use();
	//string path = "D:/model/model.obj";
	Model myModel(path);

	//transform
	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	glUniformMatrix4fv(glGetUniformLocation(myshader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		camera->processInput(window);
		glm::mat4 view = camera->view;
		glUniformMatrix4fv(glGetUniformLocation(myshader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 projection = camera->projection;
		glUniformMatrix4fv(glGetUniformLocation(myshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		myModel.Draw(myshader);
		
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//输入监听与双缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

