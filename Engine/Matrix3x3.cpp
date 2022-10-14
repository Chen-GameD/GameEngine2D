#include "Matrix3x3.h"

#include "Matrix4x4.h"

#include <assert.h>
#include <math.h>
#include <stdint.h>

namespace Engine
{
	const Matrix3x3 Matrix3x3::Identity(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f);

	Matrix3x3::Matrix3x3(void)
	{
#ifdef _DEBUG
		// In a _DEBUG build we use this to signal that we're not initialized in case we
		// call a using method (Transpose(), Invert(), operator*(), etc.) before setting the
		// matrix via one of the Create<xxxxx>() methods.
		m_11 = NAN;
#endif
	}

	Matrix3x3::Matrix3x3(float i_11, float i_12, float i_13,
						 float i_21, float i_22, float i_23,
						 float i_31, float i_32, float i_33) :
						 m_11(i_11), m_12(i_12), m_13(i_13),
						 m_21(i_21), m_22(i_22), m_23(i_23),
						 m_31(i_31), m_32(i_32), m_33(i_33) {}

	Matrix3x3::Matrix3x3(const Matrix3x3& i_other)
		:
		m_11(i_other.m_11), m_12(i_other.m_12), m_13(i_other.m_13),
		m_21(i_other.m_21), m_22(i_other.m_22), m_23(i_other.m_23),
		m_31(i_other.m_31), m_32(i_other.m_32), m_33(i_other.m_33)
	{

	}

	Matrix3x3& Matrix3x3::operator=(const Matrix3x3& i_other)
	{
		m_11 = i_other.m_11; m_12 = i_other.m_12; m_13 = i_other.m_13;
		m_21 = i_other.m_21; m_22 = i_other.m_22; m_23 = i_other.m_23;
		m_31 = i_other.m_31; m_32 = i_other.m_32; m_33 = i_other.m_33;

		return *this;
	}

	Matrix4x4 Matrix3x3::ToTransform() const
	{
		return Matrix4x4(
			m_11, m_12, m_13, 0.0f,
			m_21, m_22, m_23, 0.0f,
			m_31, m_32, m_33, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Matrix3x3::Transpose(void)
	{
		float temp = 0.0f;

		// swap row 1 and column 1 except m_11
		temp = m_21; 
		m_21 = m_12; 
		m_12 = temp;

		temp = m_31; 
		m_31 = m_13; 
		m_13 = temp;

		temp = m_32; 
		m_32 = m_23; 
		m_23 = temp;
	}

	Matrix3x3 Matrix3x3::GetTranspose(void) const
	{
		return Matrix3x3(
			m_11, m_21, m_31,
			m_12, m_22, m_32,
			m_13, m_23, m_33);
	}

	void Matrix3x3::GetTranspose(Matrix3x3& o_out) const
	{
		o_out.m_11 = m_11; o_out.m_12 = m_21; o_out.m_13 = m_31;
		o_out.m_21 = m_12; o_out.m_22 = m_22; o_out.m_23 = m_32;
		o_out.m_31 = m_13; o_out.m_32 = m_23; o_out.m_33 = m_33;
	}

	Vector3D Matrix3x3::MultiplyLeft(const Vector3D& i_Vector) const
	{
		return Vector3D(
			i_Vector.x() * m_11 + i_Vector.y() * m_21 + i_Vector.z() * m_31,
			i_Vector.x() * m_12 + i_Vector.y() * m_22 + i_Vector.z() * m_32,
			i_Vector.x() * m_13 + i_Vector.y() * m_23 + i_Vector.z() * m_33);
	}

	Vector3D Matrix3x3::MultiplyRight(const Vector3D& i_Vector) const
	{
		return Vector3D(
			i_Vector.x() * m_11 + i_Vector.y() * m_12 + i_Vector.z() * m_13,
			i_Vector.x() * m_21 + i_Vector.y() * m_22 + i_Vector.z() * m_23,
			i_Vector.x() * m_31 + i_Vector.y() * m_32 + i_Vector.z() * m_33);
	}

	void Matrix3x3::MultiplyInt(const Matrix3x3& i_other, Matrix3x3& o_out) const
	{
		o_out.m_11 = m_11 * i_other.m_11 + m_12 * i_other.m_21 + m_13 * i_other.m_31;
		o_out.m_12 = m_11 * i_other.m_12 + m_12 * i_other.m_22 + m_13 * i_other.m_32;
		o_out.m_13 = m_11 * i_other.m_13 + m_12 * i_other.m_23 + m_13 * i_other.m_33;

		o_out.m_21 = m_21 * i_other.m_11 + m_22 * i_other.m_21 + m_23 * i_other.m_31;
		o_out.m_22 = m_21 * i_other.m_12 + m_22 * i_other.m_22 + m_23 * i_other.m_32;
		o_out.m_23 = m_21 * i_other.m_13 + m_22 * i_other.m_23 + m_23 * i_other.m_33;

		o_out.m_31 = m_31 * i_other.m_11 + m_32 * i_other.m_21 + m_33 * i_other.m_31;
		o_out.m_32 = m_31 * i_other.m_12 + m_32 * i_other.m_22 + m_33 * i_other.m_32;
		o_out.m_33 = m_31 * i_other.m_13 + m_32 * i_other.m_23 + m_33 * i_other.m_33;
	}

	void Matrix3x3::Multiply(const Matrix3x3& i_other, Matrix3x3& o_out) const
	{
		MultiplyInt(i_other, o_out);
	}


	Matrix3x3 Matrix3x3::CreateIdentity(void)
	{
		return Matrix3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	Matrix3x3 Matrix3x3::CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ)
	{
		return Matrix3x3(
			i_scaleX, 0.0f, 0.0f,
			0.0f, i_scaleY, 0.0f,
			0.0f, 0.0f, i_scaleZ);
	}

	Matrix3x3 Matrix3x3::CreateXRotationCV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, cos, -sin,
			0.0f, sin, cos);
	}

	Matrix3x3 Matrix3x3::CreateYRotationCV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3x3(
			cos, 0.0f, sin,
			0.0f, 1.0f, 0.0f,
			-sin, 0.0f, cos);
	}
	Matrix3x3 Matrix3x3::CreateZRotationCV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3x3(
			cos, -sin, 0.0f,
			sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	Matrix3x3 Matrix3x3::CreateXRotationRV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, cos, sin,
			0.0f, -sin, cos);
	}

	Matrix3x3 Matrix3x3::CreateYRotationRV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3x3(
			cos, 0.0f, -sin,
			0.0f, 1.0f, 0.0f,
			sin, 0.0f, cos);
	}
	Matrix3x3 Matrix3x3::CreateZRotationRV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3x3(
			cos, sin, 0.0f,
			-sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	Vector3D Matrix3x3::Row(size_t i_row) const
	{
		switch (i_row)
		{
		case 0:
			return Vector3D(m_11, m_12, m_13);
		case 1:
			return Vector3D(m_21, m_22, m_23);
		case 2:
			return Vector3D(m_31, m_32, m_33);
		default:
			assert(false);
			return Vector3D(NAN, NAN, NAN);
		}
	}

	Vector3D Matrix3x3::Col(size_t i_col) const
	{
		switch (i_col)
		{
		case 0:
			return Vector3D(m_11, m_21, m_31);
		case 1:
			return Vector3D(m_12, m_22, m_32);
		case 2:
			return Vector3D(m_13, m_23, m_33);
		default:
			assert(false);
			return Vector3D(NAN, NAN, NAN);
		}
	}
} // namespace Engine