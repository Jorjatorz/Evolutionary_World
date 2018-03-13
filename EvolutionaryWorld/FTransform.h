#pragma once

#include "Vector3.h"
#include "Quaternion.h"

class Matrix4;

//Holds a position, orientation and scale information
class FTransform
{
public:
	FTransform();
	FTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
	~FTransform();

	void translate(const Vector3& delta);
	void rotate(const Quaternion& delta);
	void scale(const Vector3& delta);
	
	void setPosition(const Vector3& pos);
	void setRotation(const Quaternion& quat);
	void setScale(const Vector3& scale);

	Vector3 getPosition() const
	{
		return _postion;
	}
	const Vector3* getPositionPtr() const
	{
		return &_postion;
	}
	Quaternion getRotationQuaternion() const
	{
		return _rotation;
	}
	Vector3 getScale() const
	{
		return _scale;
	}
	//Computes the transformationMatrix and returns it
	Matrix4 getTransformationMatrix();

private:
	Vector3 _postion;
	Quaternion _rotation;
	Vector3 _scale;
};

