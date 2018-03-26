#include "EIndividualRendererComponent.h"

#include "Shader.h"
#include "EIndividual.h"

short int EIndividualRendererComponent::references = 0;
Shader* EIndividualRendererComponent::shader = nullptr;
GLuint EIndividualRendererComponent::vao = -1;
GLuint EIndividualRendererComponent::vertexBuffer = -1;

EIndividualRendererComponent::EIndividualRendererComponent(Object* owner)
	:RenderingComponent(owner),
	color(1.0, 1.0, 0.0)
{
	// Load shader
	if (references == 0)
	{
		shader = new Shader("IndividualShader");
		shader->loadFromDisk("defaultShader");

		// Set vertices
		const GLfloat vertex_array[] = {
			-10.0f, -10.0f, 0.0f,
			10.0f, -10.0f, 0.0f,
			0.0f,  10.0f, 0.0f,
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


EIndividualRendererComponent::~EIndividualRendererComponent()
{
	references--;
	if (references == 0)
	{
		delete shader;
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteVertexArrays(1, &vao);
	}
}

void EIndividualRendererComponent::render(const Matrix4& projection)
{
	EIndividual* ind = static_cast<EIndividual*>(owner);

	shader->bind();
	shader->uniformMatrix("MVP", projection * ind->getTransform_pointer()->getTransformationMatrix());
	shader->uniform("color", color);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	shader->unBind();
}

void EIndividualRendererComponent::setColor(const Vector3 & newColor)
{
	color = newColor;
}

EIndividualRendererComponent * EIndividualRendererComponent::clone(Object * newOwner) const
{
	return new EIndividualRendererComponent(newOwner);
}
