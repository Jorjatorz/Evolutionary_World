#pragma once

#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>

#include "Matrix4.h"

//Forward declaration
class Vector3;
class FRotator;

class Quaternion
{
public:
	//Unit quaternion constructor
	Quaternion();
	//Constructor with the components of the quaternion as arguments
	Quaternion(float w, float x, float y, float z);
	//Builds a quaternion from an angle and a NORMALIZED axis
	Quaternion(float degrees, const Vector3& normalizedAxis);
	//Builds a quaternion from euler angles
	Quaternion(const Vector3& eulerAngle);
	~Quaternion();


	//Operator overloading
	friend std::ostream& operator<<(std::ostream& out, Quaternion& quat);
	friend std::ostream& operator<<(std::ostream& out, const Quaternion& quat);

	Quaternion& operator=(const Quaternion& other);
	bool operator==(const Quaternion& other) const;
	bool operator!=(const Quaternion& other) const;

	Quaternion operator+(const Quaternion& other);
	Quaternion operator*(const Quaternion& other);
	Quaternion operator*(const Quaternion& other) const;

	Vector3 operator*(const Vector3& other);
	Vector3 operator*(const Vector3& other) const;

	Quaternion operator-();

	//Normalize the quaternion
	void normalize();
	//Return a quaternion that is this one normalized
	Quaternion getNormalizedQuaternion() const;

	//Return the inverse of this quaternion
	Quaternion inverse() const;
	//Return a quaternion that is the conjugate of the parameter
	Quaternion conjugate() const;

	//Return an euler angle vector from the quaternion
	Vector3 toEuler() const;

	// Returns a matrix encoding the rotation
	Matrix4 toMatrix() const;

	float getX() const
	{
		return _GLMQuat.x;
	}
	float getY() const
	{
		return _GLMQuat.y;
	}
	float getZ() const
	{
		return _GLMQuat.z;
	}
	float getW() const
	{
		return _GLMQuat.w;
	}

	//Dot product between quaternions
	static float dot(const Quaternion& quat1, const Quaternion& quat2);
	//Do a Spherical linear interpolation between quaternions
	static Quaternion SLERP(const Quaternion& quat1, const Quaternion& quat2, float amount);
	//Linear interpolation
	static Quaternion LERP(const Quaternion& quat1, const Quaternion& quat2, float amount);

private:
	//GLM quaternion, incharge of all the operations.
	glm::quat _GLMQuat;

	friend class Math;
};

