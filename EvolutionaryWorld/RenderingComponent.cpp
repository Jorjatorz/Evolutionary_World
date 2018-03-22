#include "RenderingComponent.h"

#include "Renderer.h"

RenderingComponent::RenderingComponent(Object* owner)
	:Component(owner)
{
	// Register to the renderer when its created
	Renderer::getInstance()->attachRenderingComponent(this);
}

RenderingComponent::~RenderingComponent()
{
	// Erase from the component when its destroyed
	Renderer::getInstance()->detachRenderingComponent(this);
}
