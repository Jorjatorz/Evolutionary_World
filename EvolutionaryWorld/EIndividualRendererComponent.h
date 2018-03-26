#pragma once

#include <GL\glew.h>

#include "RenderingComponent.h"
#include "Matrix4.h"
#include "Vector3.h"

class Shader;

class EIndividualRendererComponent : public RenderingComponent
{
public:
	EIndividualRendererComponent(Object* owner);
	~EIndividualRendererComponent();

	void render(const Matrix4& projection) override;

	void setColor(const Vector3& newColor);

private:
	Vector3 color;


	static short int references; // Counter of instances

	static GLuint vao, vertexBuffer;

	static Shader* shader;

	// Heredado vía RenderingComponent
	virtual EIndividualRendererComponent* clone(Object* newOwner) const override;
};

