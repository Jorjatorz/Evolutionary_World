#include "Quaternion.h"

#include "Vector3.h"

#include <glm/gtx/fast_trigonometry.hpp>

Quaternion::Quaternion()
	:_GLMQuat()
{
}

Quaternion::Quaternion(float w, float x, float y, float z)
	: _GLMQuat(w, x, y, z)
{
}

Quaternion::Quaternion(float degrees, const Vector3& normalizedAxis)
{
	glm::vec3 axis(normalizedAxis.x, normalizedAxis.y, normalizedAxis.z);

	_GLMQuat = glm::angleAxis(glm::radians(degrees), axis);
}

Quaternion::Quaternion(const Vector3& eulerAngle)
	:_GLMQuat(glm::radians(glm::vec3(eulerAngle.x, eulerAngle.y, eulerAngle.z)))
{
}

Quaternion::~Quaternion()
{
}

std::ostream& operator<<(std::ostream& out, Quaternion& quat)
{
	out << "W: " << quat.getW() << " X: " << quat.getX() << " Y: " << quat.getY() << " Z: " << quat.getZ();

	return out;
}

std::ostream& operator<<(std::ostream& out, const Quaternion& quat)
{
	out << "W: " << quat.getW() << " X: " << quat.getX() << " Y: " << quat.getY() << " Z: " << quat.getZ();

	return out;
}

Quaternion& Quaternion::operator=(const Quaternion& other)
{
	_GLMQuat = other._GLMQuat;

	return *this;
}

bool Quaternion::operator==(const Quaternion& other) const
{
	return _GLMQuat == other._GLMQuat;
}

bool Quaternion::operator!=(const Quaternion& other) const
{
	return _GLMQuat != other._GLMQuat;
}

Quaternion Quaternion::operator+(const Quaternion& other)
{
	Quaternion quat;
	quat._GLMQuat = _GLMQuat + other._GLMQuat;
	return quat;
}

Quaternion Quaternion::operator*(const Quaternion& other)
{
	Quaternion quat;
	quat._GLMQuat = _GLMQuat * other._GLMQuat;
	return quat;
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	Quaternion quat;
	quat._GLMQuat = _GLMQuat * other._GLMQuat;
	return quat;
}

Vector3 Quaternion::operator*(const Vector3& other)
{
	glm::vec3 aux(other.x, other.y, other.z);

	glm::vec3 ret = _GLMQuat * aux;

	return Vector3(ret.x, ret.y, ret.z);
}

Vector3 Quaternion::operator*(const Vector3& other) const
{
	glm::vec3 aux(other.x, other.y, other.z);

	glm::vec3 ret = _GLMQuat * aux;

	return Vector3(ret.x, ret.y, ret.z);
}

Quaternion Quaternion::operator-()
{
	Quaternion newQuat;

	newQuat._GLMQuat = -_GLMQuat;

	return newQuat;
}

void Quaternion::normalize()
{
	_GLMQuat = glm::normalize(_GLMQuat);
}

Quaternion Quaternion::getNormalizedQuaternion() const
{
	Quaternion toRet;
	toRet._GLMQuat = glm::normalize(_GLMQuat);

	return toRet;
}

Quaternion Quaternion::inverse() const
{
	///In here we should normalize the quaternion or check if its normalized
	Quaternion toRet;
	toRet._GLMQuat = glm::inverse(_GLMQuat);

	return toRet;
}

Quaternion Quaternion::conjugate() const
{
	Quaternion toRet;
	toRet._GLMQuat = glm::conjugate(_GLMQuat);
	return toRet;
}

Vector3 Quaternion::toEuler() const
{
	//CODE FROM UNREAL ENGINE 4 toEuler Code
	/*
	const float SingularityTest = _GLMQuat.z * _GLMQuat.x - _GLMQuat.w * _GLMQuat.y;
	const float YawY = 2.f*(_GLMQuat.w * _GLMQuat.z + _GLMQuat.x*_GLMQuat.y);
	const float YawX = (1.f - 2.f*(glm::pow(_GLMQuat.y, 2) + glm::pow(_GLMQuat.z, 2)));

	// reference 
	// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

	// this value was found from experience, the above websites recommend different values
	// but that isn't the case for us, so I went through different testing, and finally found the case 
	// where both of world lives happily. 
	const float SINGULARITY_THRESHOLD = 0.4999995f;

	FRotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.setYaw(270.f);
		RotatorFromQuat.setRoll(glm::degrees(glm::atan(YawY, YawX)));
		RotatorFromQuat.setPitch(glm::degrees(-RotatorFromQuat.getYaw() - (2.f * glm::atan(_GLMQuat.x, _GLMQuat.w))));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.setYaw(90.f);
		RotatorFromQuat.setRoll(glm::degrees(glm::atan(YawY, YawX)));
		RotatorFromQuat.setPitch(glm::degrees(RotatorFromQuat.getYaw() - (2.f * glm::atan(_GLMQuat.x, _GLMQuat.w))));
	}
	else
	{
		RotatorFromQuat.setYaw(glm::degrees(glm::fastAsin(2.f*(SingularityTest))));
		RotatorFromQuat.setRoll(glm::degrees(glm::atan(YawY, YawX)));
		RotatorFromQuat.setPitch(glm::degrees(glm::atan(-2.f*(_GLMQuat.w*_GLMQuat.x + _GLMQuat.y*_GLMQuat.z), (1.f - 2.f*(glm::pow(_GLMQuat.x, 2) + glm::pow(_GLMQuat.y, 2))))));
	}

	RotatorFromQuat.normalize();*/

	glm::vec3 eulerA = glm::degrees(glm::eulerAngles(_GLMQuat));

	//Translate roll, yaw, pitch into a range of 0 - 360
	if (eulerA.x < 0.0f)
	{
		eulerA.x += 360.0f;
	}
	if (eulerA.z < 0.0f)
	{
		eulerA.z += 360.0f;
	}
	//For yaw we have three different possibilities
	if (eulerA.y > 0.0f && (eulerA.x >= 180.0f)) //Second quadrant
	{
		eulerA.y = 90 - eulerA.y + 90;
	}
	else if (eulerA.y < 0.0f && (eulerA.x >= 180.0f)) //Third quadrant
	{
		eulerA.y = -eulerA.y + 180.0f;
	}
	else if (eulerA.y < 0.0f && (eulerA.x < 180.0f)) //Forth quadrant
	{
		eulerA.y = 90 + eulerA.y + 270;
	}
	return Vector3(eulerA.x, eulerA.y, eulerA.z);
}

Matrix4 Quaternion::toMatrix() const
{
	return Matrix4(glm::toMat4(_GLMQuat));
}

Quaternion Quaternion::SLERP(const Quaternion& quat1, const Quaternion& quat2, float amount)
{
	Quaternion toRet;

	toRet._GLMQuat = glm::mix(quat1._GLMQuat, quat2._GLMQuat, amount);

	return toRet;
}

Quaternion Quaternion::LERP(const Quaternion& quat1, const Quaternion& quat2, float amount)
{
	Quaternion toRet;

	toRet._GLMQuat = glm::lerp(quat1._GLMQuat, quat2._GLMQuat, amount);

	return toRet;
}

float Quaternion::dot(const Quaternion& quat1, const Quaternion& quat2)
{
	return glm::dot(quat1._GLMQuat, quat2._GLMQuat);
}