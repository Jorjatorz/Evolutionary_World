#pragma once

#include <GL\glew.h>

#include "Matrix4.h"

class Shader;

class EIndividualRenderer
{
public:
	EIndividualRenderer();
	~EIndividualRenderer();

	void render(const Matrix4& transform, const Vector3& color);

private:
	static short int references; // Counter of instances

	static GLuint vao, vertexBuffer;

	static Shader* shader;
};

