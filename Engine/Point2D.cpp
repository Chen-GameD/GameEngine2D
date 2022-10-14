#include "Point2D.h"

namespace Engine
{
	void Point2D::Set(int x, int y)
	{
		this->m_x = x;
		this->m_y = y;
	}

	void Point2D::SetX(int x)
	{
		this->m_x = x;
	}

	void Point2D::SetY(int y)
	{
		this->m_y = y;
	}

	int Point2D::GetX() const
	{
		return m_x;
	}

	int Point2D::GetY() const
	{
		return m_y;
	}

	bool Point2D::operator == (Point2D b) const
	{
		if (m_x == b.GetX() && m_y == b.GetY())
		{
			return true;
		}
		return false;
	}

	bool Point2D::operator != (Point2D b) const
	{
		if (m_x == b.GetX() && m_y == b.GetY())
		{
			return false;
		}
		return true;
	}

	Point2D Point2D::operator + (Point2D& b) const
	{
		return Point2D(m_x + b.GetX(), m_y + b.GetY());
	}

	Point2D Point2D::operator - (Point2D& b) const
	{
		return Point2D(m_x - b.GetX(), m_y - b.GetY());
	}

	Point2D Point2D::operator * (int multiple) const
	{
		return Point2D(m_x * multiple, m_y * multiple);
	}

	Point2D operator * (int multiple, const Point2D& b)
	{
		return b * multiple;
	}

	Point2D Point2D::operator / (int multiple) const
	{
		return Point2D((int)(m_x / multiple), (int)(m_y / multiple));
	}

	Point2D Point2D::operator - () const
	{
		return Point2D(-m_x, -m_y);
	}

	void Point2D::operator += (Point2D b)
	{
		this->m_x += b.GetX();
		this->m_y += b.GetY();
	}

	void Point2D::operator -= (Point2D b)
	{
		this->m_x -= b.GetX();
		this->m_y -= b.GetY();
	}

	void Point2D::operator *= (int multiple)
	{
		this->m_x *= multiple;
		this->m_y *= multiple;
	}

	void Point2D::operator /= (int multiple)
	{
		this->m_x = (int)(this->m_x / multiple);
		this->m_y = (int)(this->m_y / multiple);
	}
}