#include "Physic.h"

namespace Engine
{
	namespace Physic
	{
		void Physic::UpdatePhysic(GameObject& i_Object, Vector2D i_Force, float i_dt)
		{
			Vector2D Acceleration = i_Force / i_Object.m_Mass;

			if (i_Object.isApplyGravity)
			{
				Acceleration += g_Gravity / i_Object.m_Mass;
			}

			Vector2D dt_Vel = Acceleration * i_dt;
			Vector2D pre_Vel = i_Object.m_Vel;
			Vector2D next_Vel = pre_Vel + dt_Vel;
			i_Object.SetVelocity(next_Vel);
			Vector2D dt_Pos = (pre_Vel + next_Vel) / 2 * i_dt;
			i_Object.SetPosition(i_Object.m_Pos + dt_Pos);
		
		}

		void Physic::UpdateGravity(Vector2D n_Gravity)
		{
			g_Gravity = n_Gravity;
		}
	}
	
}
