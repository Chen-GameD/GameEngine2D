#pragma once

#include "Point2D.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "WeakPtrs.h"
#include "SmartPtrs.h"

#include <functional>

#include "json.hpp"

namespace Engine
{
	namespace Collision
	{
		typedef std::function<void(WeakPtrs<GameObject>&)> CollisionCallback_t;

		void Shutdown();

		void CreateCollidableFromJSON(SmartPtrs<GameObject>& i_GameObject, nlohmann::json& i_JSON);
		void SetCollisionCollidaable(SmartPtrs<GameObject>& i_GameObject, const CollisionCallback_t& i_Callback);

		void RemoveCollisionCollidaable(SmartPtrs<GameObject>& i_GameObject);
		void RemoveAllCollisionCollidaable();

		void Tick(float i_dt);
		bool FoundCollisionLastTick();
	} // namespace Collision
} // namespace Engine
