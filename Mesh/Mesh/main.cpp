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
//���ڴ�С�ı�ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	camera->Width = width;
	camera->Height = height;
	camera->updata(camera->Yaw, camera->Pitch, camera->Roll);
}

float lastX = 400, lastY = 300;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//�������
	if (firstMouse) // ���bool������ʼʱ���趨Ϊtrue��
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
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

	cout << "�����ļ���ַ���ƣ�" << "D:/model/model.obj" << endl;
	string path;
	cin >> path;

	//glfw��ʼ����opengl�汾����
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	//����
	GLFWwindow *window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();//�ر���Դ
		return -1;
	}
	glfwMakeContextCurrent(window);//set�豸������
	//��ʼ��GLAD,�ڵ���openGL����ǰ�����ʼ����ѯ����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	camera = new Camera();
	
	//ӳ�䴰��
	//glViewport(0,0,800,600);
	//�ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//���ڴ�С�ı�ص�����
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

		//���������˫����
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

