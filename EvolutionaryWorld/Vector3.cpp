#include "Vector3.h"

const Vector3 Vector3::UNIT_FORWARD = Vector3(0.0, 0.0, 1.0);

Vector3::Vector3()
	:x(0.0),
	y(0.0),
	z(0.0)
{
}

Vector3::Vector3(const float& xyz)
	:x(xyz),
	y(xyz),
	z(xyz)
{

}
Vector3::Vector3(const float& x, const float& y, const float& z)
	:x(x),
	y(y),
	z(z)
{
}


Vector3::~Vector3()
{
}

std::ostream& operator<<(std::ostream& out, const Vector3& vec)
{
	out << "X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z;

	return out;
}

Vector3& Vector3::operator=(const Vector3& other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

bool Vector3::operator == (const Vector3& other) const
{
	return ((x == other.x) && (y == other.y) && (z == other.z));
}

bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}

bool Vector3::operator>(const Vector3& other) const
{
	return ((x > other.x) && (y > other.y) && (z > other.z));
}

bool Vector3::operator>(float value) const
{
	return ((x > value) && (y > value) && (z > value));
}

bool Vector3::operator>=(const Vector3& other) const
{
	return ((x >= other.x) && (y >= other.y) && (z >= other.z));
}

bool Vector3::operator>=(float value) const
{
	return ((x >= value) && (y >= value) && (z >= value));
}

bool Vector3::operator<(const Vector3& other) const
{
	return ((x < other.x) && (y < other.y) && (z < other.z));
}

bool Vector3::operator<(float value) const
{
	return ((x < value) && (y < value) && (z < value));
}

bool Vector3::operator<=(const Vector3& other) const
{
	return ((x <= other.x) && (y <= other.y) && (z <= other.z));
}

bool Vector3::operator<=(float value) const
{
	return ((x <= value) && (y <= value) && (z <= value));
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	x = x + other.x;
	y = y + other.y;
	z = z + other.z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	x = x - other.x;
	y = y - other.y;
	z = z - other.z;

	return *this;
}


Vector3& Vector3::operator*=(const Vector3& other)
{
	x = x * other.x;
	y = y * other.y;
	z = z * other.z;

	return *this;
}

Vector3 Vector3::operator+(const Vector3& other)
{
	Vector3 result;
	result.x = x + other.x;
	result.y = y + other.y;
	result.z = z + other.z;
	return result;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
	Vector3 result;
	result.x = x + other.x;
	result.y = y + other.y;
	result.z = z + other.z;
	return result;
}

Vector3 Vector3::operator-(const Vector3& other)
{
	Vector3 result;
	result.x = x - other.x;
	result.y = y - other.y;
	result.z = z - other.z;
	return result;
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	Vector3 result;
	result.x = x - other.x;
	result.y = y - other.y;
	result.z = z - other.z;
	return result;
}

Vector3 Vector3::operator*(const Vector3& other)
{
	Vector3 result;
	result.x = x * other.x;
	result.y = y * other.y;
	result.z = z * other.z;
	return result;
}

const Vector3 Vector3::operator*(const Vector3& other) const
{
	Vector3 result;
	result.x = x * other.x;
	result.y = y * other.y;
	result.z = z * other.z;
	return result;
}

Vector3 Vector3::operator/(const Vector3& other)
{
	Vector3 result;
	result.x = x / other.x;
	result.y = y / other.y;
	result.z = z / other.z;
	return result;
}

Vector3 Vector3::operator+(const float& n)
{
	Vector3 result;
	result.x = x + n;
	result.y = y + n;
	result.z = z + n;
	return result;
}

Vector3 Vector3::operator-(const float& n)
{
	Vector3 result;
	result.x = x - n;
	result.y = y - n;
	result.z = z - n;
	return result;
}


Vector3 Vector3::operator*(const float& n)
{
	Vector3 result;
	result.x = x * n;
	result.y = y * n;
	result.z = z * n;
	return result;
}
Vector3 Vector3::operator/(const float& n)
{
	Vector3 result;
	result.x = x / n;
	result.y = y / n;
	result.z = z / n;
	return result;
}

const float* Vector3::getValuePtr() const
{
	return &x;
}

Vector3 Vector3::operator-()
{
	Vector3 newVec(-x, -y, -z);
	return newVec;
}

Vector3 Vector3::cross(const Vector3& a, const Vector3& b)
{
	glm::vec3 a1(a.x, a.y, a.z);
	glm::vec3 b1(b.x, b.y, b.z);

	glm::vec3 c = glm::cross(a1, b1);

	return Vector3(c.x, c.y, c.z);
}

float Vector3::dot(const Vector3& a, const Vector3& b)
{
	glm::vec3 a1(a.x, a.y, a.z);
	glm::vec3 b1(b.x, b.y, b.z);

	float d = glm::dot(a1, b1);

	return d;
}

void Vector3::normalize()
{
	glm::vec3 aux(x, y, z);
	aux = glm::normalize(aux);

	x = aux.x;
	y = aux.y;
	z = aux.z;
}

Vector3 Vector3::getNormalizedVector() const
{
	glm::vec3 aux(x, y, z);
	aux = glm::normalize(aux);

	return Vector3(aux.x, aux.y, aux.z);
}

float Vector3::length(const Vector3& a)
{
	return glm::length(glm::vec3(a.x, a.y, a.z));
}
