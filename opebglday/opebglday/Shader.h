#pragma once
#include <string>
class Shader
{
public:
	void test();
	Shader(const char* vertexPath, const char* fragmentPath);
	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;
	unsigned int ID;//shader program ID
	void use();
private:
	void checkCompileErrors(unsigned int ID, std::string type);
};

