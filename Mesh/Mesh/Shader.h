#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Shader
{
public:
	// ����ID
	unsigned int ID;

	// ��������ȡ��������ɫ��
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// ʹ��/�������
	void use();
	// uniform���ߺ���
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

