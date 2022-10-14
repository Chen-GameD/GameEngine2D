#pragma once

namespace Engine
{
	class Vector2D
	{
	public:
		Vector2D(float x = 0, float y = 0) : m_x(x), m_y(y) {};
		~Vector2D() {};

		static const Vector2D Zero;

		void Set(float x, float y);

		void SetX(float x);
		void SetY(float y);

		float GetX() const;
		float GetY() const;

		//Operator overloading
		// "A==B"
		bool operator == (Vector2D b) const;

		// "A!=B"
		bool operator != (Vector2D b) const;

		// "A+B"
		Vector2D operator + (Vector2D& b) const;

		// "A-B"
		Vector2D operator - (Vector2D& b) const;

		// "A*int"
		Vector2D operator * (float multiple) const;

		// "int*A"
		friend Vector2D operator * (float multiple, const Vector2D& b);

		// "A/int"
		Vector2D operator / (float multiple) const;

		// "-A"
		Vector2D operator - () const;

		// "A+=B"
		void operator += (Vector2D b);

		// "A-=B"
		void operator -= (Vector2D b);

		// "A*=int"
		void operator *= (float multiple);

		// "A/=int"
		void operator /= (float multiple);

	private:
		float m_x;
		float m_y;
	};
}

