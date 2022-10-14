#include "GameObject.h"

namespace Engine
{
	GameObject::~GameObject()
	{
		GLib::Release(m_Sprite);
		//Update.m_GameObject.release();
		//Update.m_CollisionCallback = nullptr;
	}

	void GameObject::setRenderOffset(Vector2D i_RO)
	{
		m_RenderOffset = i_RO;
	}

	void GameObject::setMass(float n_Mass)
	{
		m_Mass = n_Mass;
	}

	Vector3D GameObject::GetPositionSyn() const
	{
		return Vector3D(m_Pos.GetX(), m_Pos.GetY(), 0);
	}

	Vector2D GameObject::GetPosition() const
	{
		return m_Pos;
	}

	Vector2D GameObject::GetPositionRender() const
	{
		Vector2D r_Pos;
		switch (m_dir)
		{
		case Dir::UP:
		case Dir::NONE:
			r_Pos = m_Pos;
			break;
		case Dir::DOWN:
			r_Pos = Vector2D(m_Pos.GetX(), m_Pos.GetY() + m_RenderOffset.GetY() * 2);
			break;
		case Dir::LEFT:
			r_Pos = Vector2D(m_Pos.GetX() + m_RenderOffset.GetX(), m_Pos.GetY() + m_RenderOffset.GetY());
			break;
		case Dir::RIGHT:
			r_Pos = Vector2D(m_Pos.GetX() - m_RenderOffset.GetX(), m_Pos.GetY() + m_RenderOffset.GetY());
			break;
		}

		return r_Pos;
	}

	void GameObject::SetPosition(Vector2D i_Pos)
	{
		m_Pos = i_Pos;
	}

	float GameObject::GetZRotation()
	{
		return m_ZRot;
	}

	void GameObject::SetZRotation(float i_ZRot)
	{
		m_ZRot = i_ZRot;
	}

	Vector3D GameObject::GetVelocitySyn() const
	{
		return Vector3D(m_Vel.GetX(), m_Vel.GetY(), 0);
	}

	Vector2D GameObject::GetVelocity() 
	{ 
		return m_Vel; 
	}

	void GameObject::SetVelocity(Vector2D i_Vel)
	{
		m_Vel = i_Vel;
	}

	void GameObject::updateSprite(GLib::Sprite* n_Sprite)
	{
		m_Sprite = n_Sprite;
	}

	void GameObject::setIsApplyGravity(bool n_state)
	{
		isApplyGravity = n_state;
	}

	void GameObject::addDrag(Dir n_dir = Dir::NONE, float i_MoveSpeed = 1)
	{
		switch (n_dir)
		{
		case Dir::UP:
			m_drag = Vector2D(m_drag.GetX(), m_drag.GetY() + 1 * i_MoveSpeed);
			//this->SetPosition(Vector2D(this->GetPosition().GetX(), this->GetPosition().GetY() + 1 * i_dt));
			break;
		case Dir::DOWN:
			m_drag = Vector2D(m_drag.GetX(), m_drag.GetY() - 1 * i_MoveSpeed);
			//this->SetPosition(Vector2D(this->GetPosition().GetX(), this->GetPosition().GetY() - 1 * i_dt));
			break;
		case Dir::LEFT:
			m_drag = Vector2D(m_drag.GetX() - 1 * i_MoveSpeed, m_drag.GetY());
			//this->SetPosition(Vector2D(this->GetPosition().GetX() - 1 * i_dt, this->GetPosition().GetY()));
			break;
		case Dir::RIGHT:
			m_drag = Vector2D(m_drag.GetX() + 1 * i_MoveSpeed, m_drag.GetY());
			//this->SetPosition(Vector2D(this->GetPosition().GetX() + 1 * i_dt, this->GetPosition().GetY()));
			break;
		case Dir::NONE:
			m_drag = Vector2D(0, 0);
			break;
		}
	}

	void GameObject::updatePosition(float i_dt)
	{
		this->SetPosition(Vector2D(this->GetPosition().GetX() + m_drag.GetX() * i_dt, this->GetPosition().GetY() + m_drag.GetY() * i_dt));
	}
}


