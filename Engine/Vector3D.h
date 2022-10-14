#pragma once

#include <math.h>
#include <stdint.h>
#include <assert.h>

namespace Engine {
	class Vector3D
	{
	public:
		Vector3D() { }
		// standard constructor
		Vector3D(float i_x, float i_y, float i_z);
		Vector3D(float i_vec[3]);

		// copy constructor - for completeness
		Vector3D(const Vector3D& i_rhs);

		// assignment operator
		Vector3D& operator=(const Vector3D& i_rhs);

		// element accessors
		// - get individual element
		float x(void) const;
		float y(void) const;
		float z(void) const;
		// - set individual element
		void x(float i_x);
		void y(float i_y);
		void z(float i_z);

		Vector3D& operator+=(const Vector3D& i_rhs);
		Vector3D& operator-=(const Vector3D& i_rhs);

		Vector3D& operator*=(float i_val);
		Vector3D& operator/=(float i_val);

		// negate
		Vector3D operator-(void);

		void Normalize();
		Vector3D Normalized() const;

		float Length() const;
		float LengthSq() const;

		// - ( 0.0f, 0.0f, 0.0f )
		static const Vector3D Zero;
	private:
		float	m_x, m_y, m_z;
	};

	Vector3D operator+(const Vector3D& i_lhs, const Vector3D& i_rhs);
	Vector3D operator-(const Vector3D& i_lhs, const Vector3D& i_rhs);

	Vector3D operator*(const Vector3D& i_lhs, const Vector3D& i_rhs);
	Vector3D operator*(const Vector3D& i_lhs, float i_rhs);
	Vector3D operator*(float i_lhs, const Vector3D& i_rhs);
	Vector3D operator/(const Vector3D& i_lhs, float i_rhs);

	bool operator==(const Vector3D& i_lhs, const Vector3D& i_rhs);
	bool operator!=(const Vector3D& i_lhs, const Vector3D& i_rhs);

	float dot(const Vector3D& i_lhs, const Vector3D& i_rhs);
	Vector3D cross(const Vector3D& i_lhs, const Vector3D& i_rhs);

} // namespace Engine