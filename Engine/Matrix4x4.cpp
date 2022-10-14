#include "Matrix4x4.h"

#include <assert.h>
#include <math.h>
#include <stdint.h>

namespace Engine
{
	Matrix4x4::Matrix4x4(void)
	{
#ifdef _DEBUG
		// In a _DEBUG build we use this to signal that we're not initialized in case we
		// call a using method (Transpose(), Invert(), operator*(), etc.) before setting the
		// matrix via one of the Create<xxxxx>() methods.
		m_11 = NAN;
#endif
	}

	Matrix4x4::Matrix4x4(float i_11, float i_12, float i_13, float i_14,
						 float i_21, float i_22, float i_23, float i_24,
						 float i_31, float i_32, float i_33, float i_34,
						 float i_41, float i_42, float i_43, float i_44):
						 m_11(i_11), m_12(i_12), m_13(i_13), m_14(i_14),
						 m_21(i_21), m_22(i_22), m_23(i_23), m_24(i_24),
						 m_31(i_31), m_32(i_32), m_33(i_33), m_34(i_34),
						 m_41(i_41), m_42(i_42), m_43(i_43), m_44(i_44) {}

	Matrix4x4::Matrix4x4(const Matrix4x4& i_other):
		m_11(i_other.m_11), m_12(i_other.m_12), m_13(i_other.m_13), m_14(i_other.m_14),
		m_21(i_other.m_21), m_22(i_other.m_22), m_23(i_other.m_23), m_24(i_other.m_24),
		m_31(i_other.m_31), m_32(i_other.m_32), m_33(i_other.m_33), m_34(i_other.m_34),
		m_41(i_other.m_41), m_42(i_other.m_42), m_43(i_other.m_43), m_44(i_other.m_44) {}

	Matrix4x4::Matrix4x4(const Vector4D& vec_1, const Vector4D& vec_2, const Vector4D& vec_3, const Vector4D& vec_4):
		m_11(vec_1.GetX()), m_12(vec_1.GetY()), m_13(vec_1.GetZ()), m_14(vec_1.GetW()),
		m_21(vec_2.GetX()), m_22(vec_2.GetY()), m_23(vec_2.GetZ()), m_24(vec_2.GetW()), 
		m_31(vec_3.GetX()), m_32(vec_3.GetY()), m_33(vec_3.GetZ()), m_34(vec_3.GetW()), 
		m_41(vec_4.GetX()), m_42(vec_4.GetY()), m_43(vec_4.GetZ()), m_44(vec_4.GetW()) {}

	Matrix4x4 Matrix4x4::CreateZRotationCV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix4x4(
			cos, -sin, 0.0f, 0.0f,
			sin, cos, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4x4 Matrix4x4::CreateTranslationCV(float i_transX, float i_transY, float i_transZ)
	{
		return Matrix4x4(
			1.0f, 0.0f, 0.0f, i_transX,
			0.0f, 1.0f, 0.0f, i_transY,
			0.0f, 0.0f, 1.0f, i_transZ,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4x4 Matrix4x4::CreateTranslationCV(const Vector3D& i_trans)
	{
		return CreateTranslationCV(i_trans.x(), i_trans.y(), i_trans.z());
	}

	Matrix4x4 Matrix4x4::CreateTranslationCV(const Vector2D& i_trans)
	{
		return CreateTranslationCV(i_trans.GetX(), i_trans.GetY(), 0);
	}

	void Matrix4x4::Multiply(const Matrix4x4& i_other, Matrix4x4& o_out) const
	{
		o_out.m_11 = m_11 * i_other.m_11 + m_12 * i_other.m_21 + m_13 * i_other.m_31 + m_14 * i_other.m_41;
		o_out.m_12 = m_11 * i_other.m_12 + m_12 * i_other.m_22 + m_13 * i_other.m_32 + m_14 * i_other.m_42;
		o_out.m_13 = m_11 * i_other.m_13 + m_12 * i_other.m_23 + m_13 * i_other.m_33 + m_14 * i_other.m_43;
		o_out.m_14 = m_11 * i_other.m_14 + m_12 * i_other.m_24 + m_13 * i_other.m_34 + m_14 * i_other.m_44;

		o_out.m_21 = m_21 * i_other.m_11 + m_22 * i_other.m_21 + m_23 * i_other.m_31 + m_24 * i_other.m_41;
		o_out.m_22 = m_21 * i_other.m_12 + m_22 * i_other.m_22 + m_23 * i_other.m_32 + m_24 * i_other.m_42;
		o_out.m_23 = m_21 * i_other.m_13 + m_22 * i_other.m_23 + m_23 * i_other.m_33 + m_24 * i_other.m_43;
		o_out.m_24 = m_21 * i_other.m_14 + m_22 * i_other.m_24 + m_23 * i_other.m_34 + m_24 * i_other.m_44;

		o_out.m_31 = m_31 * i_other.m_11 + m_32 * i_other.m_21 + m_33 * i_other.m_31 + m_34 * i_other.m_41;
		o_out.m_32 = m_31 * i_other.m_12 + m_32 * i_other.m_22 + m_33 * i_other.m_32 + m_34 * i_other.m_42;
		o_out.m_33 = m_31 * i_other.m_13 + m_32 * i_other.m_23 + m_33 * i_other.m_33 + m_34 * i_other.m_43;
		o_out.m_34 = m_31 * i_other.m_14 + m_32 * i_other.m_24 + m_33 * i_other.m_34 + m_34 * i_other.m_44;

		o_out.m_41 = m_41 * i_other.m_11 + m_42 * i_other.m_21 + m_43 * i_other.m_31 + m_44 * i_other.m_41;
		o_out.m_42 = m_41 * i_other.m_12 + m_42 * i_other.m_22 + m_43 * i_other.m_32 + m_44 * i_other.m_42;
		o_out.m_43 = m_41 * i_other.m_13 + m_42 * i_other.m_23 + m_43 * i_other.m_33 + m_44 * i_other.m_43;
		o_out.m_44 = m_41 * i_other.m_14 + m_42 * i_other.m_24 + m_43 * i_other.m_34 + m_44 * i_other.m_44;
	}

	Vector4D Matrix4x4::MultiplyLeft(const Vector4D& i_Vector) const
	{
		return Vector4D(
			i_Vector.GetX() * m_11 + i_Vector.GetY() * m_21 + i_Vector.GetZ() * m_31 + i_Vector.GetW() * m_41,
			i_Vector.GetX() * m_12 + i_Vector.GetY() * m_22 + i_Vector.GetZ() * m_32 + i_Vector.GetW() * m_42,
			i_Vector.GetX() * m_13 + i_Vector.GetY() * m_23 + i_Vector.GetZ() * m_33 + i_Vector.GetW() * m_43,
			i_Vector.GetX() * m_14 + i_Vector.GetY() * m_24 + i_Vector.GetZ() * m_34 + i_Vector.GetW() * m_44);
	}

	Vector4D Matrix4x4::MultiplyRight(const Vector4D& i_Vector) const
	{
		return Vector4D(
			i_Vector.GetX() * m_11 + i_Vector.GetY() * m_12 + i_Vector.GetZ() * m_13 + i_Vector.GetW() * m_14,
			i_Vector.GetX() * m_21 + i_Vector.GetY() * m_22 + i_Vector.GetZ() * m_23 + i_Vector.GetW() * m_24,
			i_Vector.GetX() * m_31 + i_Vector.GetY() * m_32 + i_Vector.GetZ() * m_33 + i_Vector.GetW() * m_34,
			i_Vector.GetX() * m_41 + i_Vector.GetY() * m_42 + i_Vector.GetZ() * m_43 + i_Vector.GetW() * m_44);
	}

	Vector3D Matrix4x4::TransformPointCV(const Vector3D& i_point) const
	{
		Vector4D Point = MultiplyRight(Vector4D(i_point, 1.0f));

		float inv_w = 1.0f / Point.GetW();
		return Vector3D(Point.GetX() * inv_w, Point.GetY() * inv_w, Point.GetZ() * inv_w);
	}
}