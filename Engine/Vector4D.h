#pragma once

#include "Vector3D.h"

namespace Engine
{
	class Vector4D
	{
	public:
		Vector4D(float x = 0, float y = 0, float z = 0, float w = 0) : m_x(x), m_y(y), m_z(z), m_w(w) {};
		Vector4D(const Vector3D& i_other, float i_w) : m_x(i_other.x()), m_y(i_other.y()), m_z(i_other.z()), m_w(i_w) {};
		~Vector4D() {};

		void Set(float x, float y, float z, float w);

		void SetX(float x); 
		void SetY(float y);
		void SetZ(float z);
		void SetW(float w);

		float GetX() const;
		float GetY() const;
		float GetZ() const;
		float GetW() const;

		//Operator overloading
		// "A==B"
		bool operator == (Vector4D b) const;

		// "A!=B"
		bool operator != (Vector4D b) const;

		// "A+B"
		Vector4D operator + (Vector4D& b) const;

		// "A-B"
		Vector4D operator - (Vector4D& b) const;

		// "A*int"
		Vector4D operator * (float multiple) const;

		// "int*A"
		friend Vector4D operator * (float multiple, const Vector4D& b);

		// "A/int"
		Vector4D operator / (float multiple) const;

		// "-A"
		Vector4D operator - () const;

		// "A+=B"
		void operator += (Vector4D b);

		// "A-=B"
		void operator -= (Vector4D b);

		// "A*=int"
		void operator *= (float multiple);

		// "A/=int"
		void operator /= (float multiple);

	private:
		float m_x;
		float m_y;
		float m_z;
		float m_w;
	};
}

