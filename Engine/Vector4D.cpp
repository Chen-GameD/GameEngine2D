#include "Vector4D.h"

namespace Engine
{
	void Vector4D::Set(float x, float y, float z, float w)
	{
		this->m_x = x;
		this->m_y = y;
		this->m_z = z;
		this->m_w = w;
	}

	void Vector4D::SetX(float x)
	{
		this->m_x = x;
	}

	void Vector4D::SetY(float y)
	{
		this->m_y = y;
	}

	void Vector4D::SetZ(float z)
	{
		this->m_z = z;
	}

	void Vector4D::SetW(float w)
	{
		this->m_w = w;
	}

	float Vector4D::GetX() const
	{
		return m_x;
	}

	float Vector4D::GetY() const
	{
		return m_y;
	}

	float Vector4D::GetZ() const
	{
		return m_z;
	}

	float Vector4D::GetW() const
	{
		return m_w;
	}

	bool Vector4D::operator == (Vector4D b) const
	{
		if (m_x == b.GetX() && m_y == b.GetY() && m_z == b.GetZ() && m_w == b.GetW())
		{
			return true;
		}
		return false;
	}

	bool Vector4D::operator != (Vector4D b) const
	{
		if (m_x == b.GetX() && m_y == b.GetY() && m_z == b.GetZ() && m_w == b.GetW())
		{
			return false;
		}
		return true;
	}

	Vector4D Vector4D::operator + (Vector4D& b) const
	{
		return Vector4D(m_x + b.GetX(), m_y + b.GetY(), m_z + b.GetZ(), m_w + b.GetW());
	}

	Vector4D Vector4D::operator - (Vector4D& b) const
	{
		return Vector4D(m_x - b.GetX(), m_y - b.GetY(), m_z - b.GetZ(), m_w - b.GetW());
	}

	Vector4D Vector4D::operator * (float multiple) const
	{
		return Vector4D(m_x * multiple, m_y * multiple, m_z * multiple, m_w * multiple);
	}

	Vector4D operator * (float multiple, const Vector4D& b)
	{
		return b * multiple;
	}

	Vector4D Vector4D::operator / (float multiple) const
	{
		return Vector4D((float)(m_x / multiple), (float)(m_y / multiple), (float)(m_z / multiple), (float)(m_w / multiple));
	}

	Vector4D Vector4D::operator - () const
	{
		return Vector4D(-m_x, -m_y, -m_z, -m_w);
	}

	void Vector4D::operator += (Vector4D b)
	{
		this->m_x += b.GetX();
		this->m_y += b.GetY();
		this->m_z += b.GetZ();
		this->m_w += b.GetW();
	}

	void Vector4D::operator -= (Vector4D b)
	{
		this->m_x -= b.GetX();
		this->m_y -= b.GetY();
		this->m_z -= b.GetZ();
		this->m_w -= b.GetW();
	}

	void Vector4D::operator *= (float multiple)
	{
		this->m_x *= multiple;
		this->m_y *= multiple;
		this->m_z *= multiple;
		this->m_w *= multiple;
	}

	void Vector4D::operator /= (float multiple)
	{
		this->m_x = (float)(this->m_x / multiple);
		this->m_y = (float)(this->m_y / multiple);
		this->m_z = (float)(this->m_z / multiple);
		this->m_w = (float)(this->m_w / multiple);
	}
}