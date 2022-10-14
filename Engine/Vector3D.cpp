#include "Vector3D.h"

namespace Engine {
	const Vector3D Vector3D::Zero(0.0f, 0.0f, 0.0f);

	void Vector3D::Normalize()
	{
		float length = Length();

		float maxDiff = 0.000000001f;

		if (!(fabs(length) < maxDiff)) //check length is not zero;
		{
			float inv_length = 1.0f / length;

			m_x *= inv_length;
			m_y *= inv_length;
			m_z *= inv_length;
		}
	}

	Vector3D Vector3D::Normalized() const
	{
		float length = Length();

		float maxDiff = 0.000000001f;

		if (fabs(length) < maxDiff)
			return Zero;
		else
		{
			float inv_length = 1.0f / length;

			return *this * inv_length;
		}
	}

	Vector3D::Vector3D(float i_x, float i_y, float i_z) : m_x(i_x), m_y(i_y), m_z(i_z) {}

	Vector3D::Vector3D(float i_vec[3]) : m_x(i_vec[0]), m_y(i_vec[1]), m_z(i_vec[2]) {}

	Vector3D::Vector3D(const Vector3D& i_rhs) : m_x(i_rhs.m_x), m_y(i_rhs.m_y), m_z(i_rhs.m_z) {}

	// operators
	Vector3D& Vector3D::operator=(const Vector3D& i_rhs)
	{
		m_x = i_rhs.m_x;
		m_y = i_rhs.m_y;
		m_z = i_rhs.m_z;

		return *this;
	}

	// accessors
	float Vector3D::x(void) const
	{
		return m_x;
	}

	float Vector3D::y(void) const
	{
		return m_y;
	}

	float Vector3D::z(void) const
	{
		return m_z;
	}

	void Vector3D::x(float i_x)
	{
		m_x = i_x;
	}

	void Vector3D::y(float i_y)
	{
		m_y = i_y;
	}

	void Vector3D::z(float i_z)
	{
		m_z = i_z;
	}

	Vector3D& Vector3D::operator+=(const Vector3D& i_rhs)
	{
		m_x += i_rhs.m_x;
		m_y += i_rhs.m_y;
		m_z += i_rhs.m_z;

		return *this;
	}

	Vector3D& Vector3D::operator-=(const Vector3D& i_rhs)
	{
		m_x -= i_rhs.m_x;
		m_y -= i_rhs.m_y;
		m_z -= i_rhs.m_z;

		return *this;
	}

	Vector3D& Vector3D::operator*=(float i_rhs)
	{
		m_x *= i_rhs;
		m_y *= i_rhs;
		m_z *= i_rhs;

		return *this;
	}

	Vector3D& Vector3D::operator/=(float i_rhs)
	{
		m_x /= i_rhs;
		m_y /= i_rhs;
		m_z /= i_rhs;

		return *this;
	}

	Vector3D Vector3D::operator-(void)
	{
		return Vector3D(-m_x, -m_y, -m_z);
	}

	float Vector3D::LengthSq() const
	{
		return (m_x * m_x) + (m_y * m_y) + (m_z * m_z);
	}

	float Vector3D::Length() const
	{
		return static_cast<float>(sqrt(LengthSq()));
	}

	// stand alone operators
	Vector3D operator+(const Vector3D& i_lhs, const Vector3D& i_rhs)
	{
		return Vector3D(i_lhs.x() + i_rhs.x(), i_lhs.y() + i_rhs.y(), i_lhs.z() + i_rhs.z());
	}

	Vector3D operator-(const Vector3D& i_lhs, const Vector3D& i_rhs)
	{
		return Vector3D(i_lhs.x() - i_rhs.x(), i_lhs.y() - i_rhs.y(), i_lhs.z() - i_rhs.z());
	}

	Vector3D operator*(const Vector3D& i_lhs, const Vector3D& i_rhs)
	{

		return Vector3D(i_lhs.x() * i_rhs.x(), i_lhs.y() * i_rhs.y(), i_lhs.z() * i_rhs.z());
	}

	Vector3D operator*(const Vector3D& i_lhs, float i_rhs)
	{
		return Vector3D(i_lhs.x() * i_rhs, i_lhs.y() * i_rhs, i_lhs.z() * i_rhs);
	}

	Vector3D operator*(float i_lhs, const Vector3D& i_rhs)
	{
		return Vector3D(i_rhs.x() * i_lhs, i_rhs.y() * i_lhs, i_rhs.z() * i_lhs);
	}

	Vector3D operator/(const Vector3D& i_lhs, float i_rhs)
	{
		return Vector3D(i_lhs.x() / i_rhs, i_lhs.y() / i_rhs, i_lhs.z() / i_rhs);
	}

	bool operator==(const Vector3D& i_lhs, const Vector3D& i_rhs)
	{
		return fabs(i_lhs.x() - i_rhs.x()) && fabs(i_lhs.y() - i_rhs.y()) && fabs(i_lhs.z() - i_rhs.z());
	}

	bool operator!=(const Vector3D& i_lhs, const Vector3D& i_rhs)
	{
		return !fabs(i_lhs.x() - i_rhs.x()) || !fabs(i_lhs.y() - i_rhs.y()) || !fabs(i_lhs.z() - i_rhs.z());
	}

	float dot(const Vector3D& i_lhs, const Vector3D& i_rhs)
	{
		return i_lhs.x() * i_rhs.x() + i_lhs.y() * i_rhs.y() + i_lhs.z() * i_rhs.z();
	}

	Vector3D cross(const Vector3D& i_lhs, const Vector3D& i_rhs)
	{
		return Vector3D(i_lhs.y() * i_rhs.z() - i_lhs.z() * i_rhs.y(),
			i_lhs.z() * i_rhs.x() - i_lhs.x() * i_rhs.z(),
			i_lhs.x() * i_rhs.y() - i_lhs.y() * i_rhs.x());
	}
} // namespace Engine