#pragma once

#include "Vector4D.h"
#include "Vector3D.h"
#include "Vector2D.h"
namespace Engine {
	class Matrix4x4 {
	private:
		float	m_11, m_12, m_13, m_14, // row 1
				m_21, m_22, m_23, m_24, // row 2
				m_31, m_32, m_33, m_34, // row 3
				m_41, m_42, m_43, m_44; // row 4
	public:
		Matrix4x4();

		Matrix4x4(float i_11, float i_12, float i_13, float i_14,
				  float i_21, float i_22, float i_23, float i_24,
				  float i_31, float i_32, float i_33, float i_34,
				  float i_41, float i_42, float i_43, float i_44);

		Matrix4x4(const Matrix4x4& i_other);

		Matrix4x4(const Vector4D& vec_1, const Vector4D& vec_2, const Vector4D& vec_3, const Vector4D& vec_4);

		static Matrix4x4 CreateZRotationCV(float i_RotationRadians);

		static Matrix4x4 CreateTranslationCV(float i_transX, float i_transY, float i_transZ);
		static Matrix4x4 CreateTranslationCV(const Vector3D& i_trans);
		static Matrix4x4 CreateTranslationCV(const Vector2D& i_trans);
		
		void Multiply(const Matrix4x4& i_other, Matrix4x4& o_out) const;

		// Multiply Vector by Matrix
		// return v * M
		Vector4D MultiplyLeft(const Vector4D& i_Vector) const;
		// return M * v
		Vector4D MultiplyRight(const Vector4D& i_Vector) const;

		Matrix4x4 operator*(const Matrix4x4& i_other) const
		{
			Matrix4x4 tResult;
			Multiply(i_other, tResult);

			return Matrix4x4(tResult);
		}

		Vector3D TransformPointCV(const Vector3D& i_point) const;
	};
}