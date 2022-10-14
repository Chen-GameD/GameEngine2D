#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "GLib.h"
#include "SmartPtrs.h"
#include <iostream>

namespace Engine
{
	enum class Dir { UP, DOWN, LEFT, RIGHT, NONE };
	class GameObject;

	typedef std::function<void(WeakPtrs<GameObject>&, float i_dt)> CustomFunctionCallback;

	struct FunctionCreator
	{
		WeakPtrs<GameObject>	m_GameObject;
		CustomFunctionCallback	m_CollisionCallback;
	};

	class GameObject
	{
	public:
		std::string m_Tag;
		float m_Mass;
		Vector2D m_Pos;
		Vector2D m_Vel;
		Vector2D m_RenderOffset;
		float m_ZRot;
		GLib::Sprite* m_Sprite;
		bool isApplyGravity;
		Dir m_dir = Dir::NONE;
		Vector2D m_drag = Vector2D::Zero;

		static SmartPtrs<GameObject> Create(std::string i_Tag = "", float i_Mass = 0, Vector2D i_Pos = Vector2D::Zero, Vector2D i_Vel = Vector2D::Zero, Vector2D i_RO = Vector2D(0, 0), GLib::Sprite * i_Sprite = nullptr, bool i_isApplyGravity = false)
		{
			return SmartPtrs<GameObject>(new GameObject(i_Tag, i_Mass, i_Pos, i_Vel, i_RO, i_Sprite, i_isApplyGravity));
		}

		GameObject(std::string i_Tag = "", float i_Mass = 0, Vector2D i_Pos = Vector2D(0, 0), Vector2D i_Vel = Vector2D(0, 0), Vector2D i_RO = Vector2D(0, 0), GLib::Sprite* i_Sprite = nullptr, bool i_isApplyGravity = false) :
			m_Tag(i_Tag), m_Mass(i_Mass), m_Pos(i_Pos), m_Vel(i_Vel), m_RenderOffset(i_RO), m_Sprite(i_Sprite), isApplyGravity(i_isApplyGravity) {};

		Vector3D GetPositionSyn() const;
		Vector2D GetPosition() const;
		Vector2D GetPositionRender() const;
		void SetPosition(Vector2D i_Pos);

		float GetZRotation();
		void SetZRotation(float i_ZRot);

		Vector3D GetVelocitySyn() const;
		Vector2D GetVelocity();
		void SetVelocity(Vector2D i_Vel);

		void setRenderOffset(Vector2D i_RO);
		void setMass(float n_Mass);
		void updateSprite(GLib::Sprite* n_Sprite);
		void setIsApplyGravity(bool n_state);
		void addDrag(Dir n_dir, float i_MoveSpeed);

		//virtual function;
		//using in AI auto move;
		void updatePosition(float i_dt);

		FunctionCreator Update;
		void SetUpdateFunction(SmartPtrs<GameObject>& i_GameObject, const CustomFunctionCallback& i_Update)
		{
			Update.m_GameObject = i_GameObject;
			Update.m_CollisionCallback = i_Update;
		}

		~GameObject();
	};
}