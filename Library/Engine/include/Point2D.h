#ifndef _POINT2D_H_
#define _POINT2D_H_

namespace Engine
{
	class Point2D
	{
	public:
		Point2D(int x = 0, int y = 0) : m_x(x), m_y(y) {};
		~Point2D() {};

		void Set(int x, int y);

		void SetX(int x);
		void SetY(int y);

		int GetX() const;
		int GetY() const;

		//Operator overloading
		// "A==B"
		bool operator == (Point2D b) const;

		// "A!=B"
		bool operator != (Point2D b) const;

		// "A+B"
		Point2D operator + (Point2D& b) const;

		// "A-B"
		Point2D operator - (Point2D& b) const;

		// "A*int"
		Point2D operator * (int multiple) const;

		// "int*A"
		friend Point2D operator * (int multiple, const Point2D& b);

		// "A/int"
		Point2D operator / (int multiple) const;

		// "-A"
		Point2D operator - () const;

		// "A+=B"
		void operator += (Point2D b);

		// "A-=B"
		void operator -= (Point2D b);

		// "A*=int"
		void operator *= (int multiple);

		// "A/=int"
		void operator /= (int multiple);

	private:
		int m_x;
		int m_y;
	};
}

#endif // !_POINT2D_H_

