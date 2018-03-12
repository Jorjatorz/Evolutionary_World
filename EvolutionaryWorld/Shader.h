#pragma once

#include <string>

#include <gl/glew.h>
#include <gl/gl.h>

class Vector3;
class Matrix4;

class Shader
{
public:
	Shader(std::string name);
	~Shader(void);

	//shader locations
	enum shaderVertexLocations
	{
		VERTEX_POSITION,
		VERTEX_NORMALS,
		VERTEX_TEXCOORDS,
		VERTEX_TANGENTS
	};

	//Load the shader, false if an error ocurred
	///Could make tis static
	bool loadFromDisk(std::string filePath);

	//Return the openGL shader program
	GLuint getShaderProgram()
	{
		return _program;
	}
	//bind the shader (gl_UsePorgram)
	void bind();
	//unbind the shader
	static void unBind();

	// Communication with shader
	void uniformTexture(const std::string& uniformName, GLint activeTextureSlot);
	void uniform(const std::string& uniformName, GLint value);
	void uniform(const std::string& uniformName, GLuint value);
	void uniform(const std::string& uniformName, GLfloat value);
	void uniform(const std::string& uniformName, const Vector3& value);
	void uniformMatrix(const std::string & uniformName, const Matrix4& value);

private:
	std::string _name;
	GLuint _program;

};
