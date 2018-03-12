#include "EIndividualRenderer.h"

#include "Shader.h"

short int EIndividualRenderer::references = 0;
Shader* EIndividualRenderer::shader = nullptr;
GLuint EIndividualRenderer::vao = -1;
GLuint EIndividualRenderer::vertexBuffer = -1;

EIndividualRenderer::EIndividualRenderer()
{
	// Load shader
	if (references == 0)
	{
		shader = new Shader("IndividualShader");
		shader->loadFromDisk("defaultShader");

		// Set vertices
		const GLfloat vertex_array[] = {
			0.0f, 0.0f, 0.0f,
			20.0f, 0.0f, 0.0f,
			10.0f,  20.0f, 0.0f,
		};


		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array), vertex_array, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	references++;
}


EIndividualRenderer::~EIndividualRenderer()
{
	references--;
	if (references == 0)
	{
		delete shader;
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteVertexArrays(1, &vao);
	}
}

void EIndividualRenderer::render(const Matrix4& transform, const Vector3& color)
{
	shader->bind();
	shader->uniformMatrix("MVP", transform);
	shader->uniform("color", color);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	shader->unBind();
}
