#pragma once
#include "GameObject.h"

namespace Engine
{
	namespace Physic
	{
		static Vector2D g_Gravity = Vector2D(0, 9.8f);

		void UpdatePhysic(GameObject& i_Object, Vector2D i_Force, float i_dt);

		void UpdateGravity(Vector2D n_Gravity);
	}
}