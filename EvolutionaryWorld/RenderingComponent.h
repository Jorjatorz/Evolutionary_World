#pragma once

#include "Component.h"

class Object;
class Matrix4;

// Virtual class for rendering components
class RenderingComponent : public Component
{
public:
	RenderingComponent(Object* owner);
	virtual ~RenderingComponent();

	virtual void render(const Matrix4& projectionView) = 0;
	virtual RenderingComponent* clone(Object* newOwner) const = 0;
};

