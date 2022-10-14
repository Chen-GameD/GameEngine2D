#include "Vector2D.h"

namespace Engine
{
	const Vector2D Vector2D::Zero(0.0f, 0.0f);

	void Vector2D::Set(float x, float y)
	{
		this->m_x = x;
		this->m_y = y;
	}

	void Vector2D::SetX(float x)
	{
		this->m_x = x;
	}

	void Vector2D::SetY(float y)
	{
		this->m_y = y;
	}

	float Vector2D::GetX() const
	{
		return m_x;
	}

	float Vector2D::GetY() const
	{
		return m_y;
	}

	bool Vector2D::operator == (Vector2D b) const
	{
		if (m_x == b.GetX() && m_y == b.GetY())
		{
			return true;
		}
		return false;
	}

	bool Vector2D::operator != (Vector2D b) const
	{
		if (m_x == b.GetX() && m_y == b.GetY())
		{
			return false;
		}
		return true;
	}

	Vector2D Vector2D::operator + (Vector2D& b) const
	{
		return Vector2D(m_x + b.GetX(), m_y + b.GetY());
	}

	Vector2D Vector2D::operator - (Vector2D& b) const
	{
		return Vector2D(m_x - b.GetX(), m_y - b.GetY());
	}

	Vector2D Vector2D::operator * (float multiple) const
	{
		return Vector2D(m_x * multiple, m_y * multiple);
	}

	Vector2D operator * (float multiple, const Vector2D& b)
	{
		return b * multiple;
	}

	Vector2D Vector2D::operator / (float multiple) const
	{
		return Vector2D((float)(m_x / multiple), (float)(m_y / multiple));
	}

	Vector2D Vector2D::operator - () const
	{
		return Vector2D(-m_x, -m_y);
	}

	void Vector2D::operator += (Vector2D b)
	{
		this->m_x += b.GetX();
		this->m_y += b.GetY();
	}

	void Vector2D::operator -= (Vector2D b)
	{
		this->m_x -= b.GetX();
		this->m_y -= b.GetY();
	}

	void Vector2D::operator *= (float multiple)
	{
		this->m_x *= multiple;
		this->m_y *= multiple;
	}

	void Vector2D::operator /= (float multiple)
	{
		this->m_x = (float)(this->m_x / multiple);
		this->m_y = (float)(this->m_y / multiple);
	}
}