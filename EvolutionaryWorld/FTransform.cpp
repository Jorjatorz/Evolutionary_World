#include "FTransform.h"

#include "Matrix4.h"
#include "Math.h"

FTransform::FTransform()
	:_postion(0.0),
	_scale(1.0),
	_rotation(1.0, 0.0, 0.0, 0.0)
{
}

FTransform::FTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	:_postion(position),
	_scale(scale),
	_rotation(rotation)
{
}


FTransform::~FTransform()
{
}

void FTransform::setPosition(const Vector3& pos)
{
	_postion = pos;
}

void FTransform::setRotation(const Quaternion& quat)
{
	_rotation = quat;
}

void FTransform::setScale(const Vector3& scale)
{
	_scale = scale;
}

void FTransform::translate(const Vector3& delta)
{
	_postion += delta;
}

void FTransform::rotate(const Quaternion& delta)
{
	_rotation = delta * _rotation;
	_rotation.normalize();
}

void FTransform::scale(const Vector3& delta)
{
	_scale *= delta;
}

Matrix4 FTransform::getTransformationMatrix()
{
	Matrix4 toRet(1.0);

	toRet.translate(_postion);
	toRet = toRet * _rotation.toMatrix();
	toRet.scale(_scale);

	return toRet;
}
