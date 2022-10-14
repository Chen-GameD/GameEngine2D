#pragma once

#include "Vector3D.h"

namespace Engine
{
	class Matrix4x4;

	class Matrix3x3
	{
	public:
		Matrix3x3();
		Matrix3x3(
			float i_11, float i_12, float i_13,
			float i_21, float i_22, float i_23,
			float i_31, float i_32, float i_33);

		// Copy constructor & assignment operator
		Matrix3x3(const Matrix3x3& i_other);
		Matrix3x3& operator=(const Matrix3x3& i_other);

		Matrix3x3 operator*(const Matrix3x3& i_other) const;

		Vector3D Row(size_t i_row) const;
		Vector3D Col(size_t i_col) const;

		Matrix4x4 ToTransform() const;

		// Useful matrix creators
		static Matrix3x3 CreateIdentity(void);
		static Matrix3x3 CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ);

		// Layout for row vector multiplies ( V = V * M )
		static Matrix3x3 CreateXRotationRV(float i_RotationRadians);
		static Matrix3x3 CreateYRotationRV(float i_RotationRadians);
		static Matrix3x3 CreateZRotationRV(float i_RotationRadians);

		// Layout for column vector multiplies ( V = M * V )
		static Matrix3x3 CreateXRotationCV(float i_RotationRadians);
		static Matrix3x3 CreateYRotationCV(float i_RotationRadians);
		static Matrix3x3 CreateZRotationCV(float i_RotationRadians);

		// Transpose this instance
		void Transpose(void);
		// return a matrix that is a transpose of this instance but don't modify this instance
		Matrix3x3 GetTranspose(void) const;
		void GetTranspose(Matrix3x3& o_out) const;

		// Multiply Matrix * Matrix
		// return *this * i_other
		void Multiply(const Matrix3x3& i_other, Matrix3x3& o_out) const;

		// Multiply Vector by Matrix
		// return v * M
		Vector3D MultiplyLeft(const Vector3D& i_Vector) const;
		// return M * v
		Vector3D MultiplyRight(const Vector3D& i_Vector) const;

		static const Matrix3x3 Identity;

	private:
		float
			// m_<row><column>
			m_11, m_12, m_13, // row 1
			m_21, m_22, m_23, // row 2
			m_31, m_32, m_33; // row 3

		void MultiplyInt(const Matrix3x3& i_other, Matrix3x3& o_out) const;

	};

	// Allows us to use V = M * V; (i.e. column vector)
	inline Vector3D operator*(const Matrix3x3& i_mtx, const Vector3D& i_vec);
	// Allows us to use V = V * M; (i.e. row vector)
	inline Vector3D operator*(const Vector3D& i_vec, const Matrix3x3& i_mtx);
} // namespace Engine

//#include "Matrix3x3.inl"