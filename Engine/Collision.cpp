#include "Collision.h"

#include "Point2D.h"
#include "GameObject.h"
#include "StartupShutdown.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "ScopeLock.h"
#include "vector"

#include "json.hpp"
#include "ConsolePrint.h"

namespace Engine
{
	namespace Collision
	{
		struct AABB
		{
			Vector3D m_Center;
			Vector3D m_Extents;
		};

		struct CollisionCheckData
		{
			SmartPtrs<GameObject>	m_GameObject;
			Matrix4x4				m_ObjectToWorld;
			Matrix3x3				m_OrientationInWorld;
			Vector3D				m_BBCenterInWorld;
		};

		struct Collideable
		{
			WeakPtrs<GameObject>	m_GameObject;
			AABB					m_BoundingBox;
			CollisionCallback_t		m_CollisionCallback;

			CollisionCheckData		m_CachedCheckData;
		};

		struct CollisionPair
		{
			Collideable* m_pCollideables[2];
			float		 m_CollisionTime;
		};

		std::vector<Collideable> NewCollideables;
		Mutex NewCollideablesMutex;

		std::vector<Collideable> AllCollideables;

		bool bFoundCollisionThisTick = false;

		bool CheckForCollisions(float i_dt);
		void CheckForNewCollideables();

		void Shutdown()
		{
			NewCollideables.clear();
			AllCollideables.clear();
		}

		static Vector3D Vec3(const nlohmann::json& i_JSON)
		{
			assert(i_JSON.is_array() && (i_JSON.size() >= 2));

			return Vector3D(i_JSON[0], i_JSON[1], i_JSON.size() > 2 ? float(i_JSON[2]) : 0.0f);
		}

		void CreateCollidableFromJSON(SmartPtrs<GameObject>& i_GameObject, nlohmann::json& i_JSON)
		{
			AABB NewAABB;

			NewAABB.m_Center = Vec3(i_JSON["offset"]);
			NewAABB.m_Extents = Vec3(i_JSON["extents"]);

			{
				ScopeLock Lock(NewCollideablesMutex);
				NewCollideables.push_back({ i_GameObject, NewAABB });
			}
		}

		void SetCollisionCollidaable(SmartPtrs<GameObject>& i_GameObject, const CollisionCallback_t& i_Callback)
		{
			CheckForNewCollideables();

			for (auto& c : AllCollideables)
			{
				if (c.m_GameObject == i_GameObject)
				{
					c.m_CollisionCallback = i_Callback;
					break;
				}
			}
		}

		void RemoveCollisionCollidaable(SmartPtrs<GameObject>& i_GameObject)
		{
			size_t count = AllCollideables.size();

			for (size_t i = 0; i < count; i++)
			{
				if (AllCollideables[i].m_GameObject.lock() == i_GameObject)
				{
					auto iter = AllCollideables.erase(AllCollideables.begin() + i);
					DEBUG_PRINT("Remove gameobject collideables from the world: index = " + i);
					break;
				}
			}
		}

		void RemoveAllCollisionCollidaable()
		{
			int count = AllCollideables.size();

			for (int i = count - 1; i >= 0 ; i--)
			{
				auto iter = AllCollideables.erase(AllCollideables.begin() + i);
				DEBUG_PRINT("Remove gameobject collideables from the world: index = " + i);
			}
		}

		void CheckForNewCollideables()
		{
			ScopeLock Lock(NewCollideablesMutex);
			for (auto& c : NewCollideables)
			{
				AllCollideables.push_back(c);
			}

			NewCollideables.clear();
		}

		void CacheCollisionCheckData()
		{
			size_t count = AllCollideables.size();

			for (size_t i = 0; i < count; )
			{
				Collideable& c = AllCollideables[i];

				c.m_CachedCheckData.m_GameObject = c.m_GameObject.lock();
				if (c.m_CachedCheckData.m_GameObject)
				{
					c.m_CachedCheckData.m_OrientationInWorld = Matrix3x3::CreateZRotationCV(c.m_CachedCheckData.m_GameObject->GetZRotation());
					c.m_CachedCheckData.m_ObjectToWorld = Matrix4x4::CreateTranslationCV(c.m_CachedCheckData.m_GameObject->GetPosition()) * Matrix4x4::CreateZRotationCV(c.m_CachedCheckData.m_GameObject->GetZRotation());
					c.m_CachedCheckData.m_BBCenterInWorld = c.m_CachedCheckData.m_ObjectToWorld.TransformPointCV(c.m_BoundingBox.m_Center);

					++i;
				}
				else
				{
					if (i < (count - 1))
						AllCollideables[i] = std::move(AllCollideables.back());

					AllCollideables.pop_back();
					--count;
				}
			}

			assert(count == AllCollideables.size());
		}

		void ReleaseCachedCollisionCheckData()
		{
			for (auto& c : AllCollideables)
			{
				c.m_CachedCheckData.m_GameObject = nullptr;
			}
		}

		void Tick(float i_dt)
		{
			CheckForNewCollideables();

			bFoundCollisionThisTick = CheckForCollisions(i_dt);
		}

		bool FoundCollisionLastTick()
		{
			return bFoundCollisionThisTick;
		}

		bool DetectCrossTimes(float i_Center, float i_Extent, float i_Point, float i_Travel, float& o_tEnter, float& o_tExit)
		{
			assert(i_Extent > 0.0f);

			float i_Start = i_Center - i_Extent;
			float i_End = i_Center + i_Extent;

			if (i_Travel == 0)
			{
				if ((i_Point < i_Start) || (i_Point > i_End))
					return false;
				else
				{
					o_tEnter = 0.0f;
					o_tExit = 1.0f;
					return true;
				}
			}
			o_tEnter = (i_Start - i_Point) / i_Travel;
			o_tExit = (i_End - i_Point) / i_Travel;

			if (o_tEnter > o_tExit)
			{
				//swap
				float temp = o_tEnter;
				o_tEnter = o_tExit;
				o_tExit = temp;
			}

			return !((o_tEnter >= 1.0f) || (o_tExit <= 0.0f));
		}

		bool CheckCollision2D(const Collideable& i_Collideable1, const Collideable& i_Collideable2, float i_dt, float& o_tCollision)
		{
			const SmartPtrs<GameObject>& i_GameObject1 = i_Collideable1.m_CachedCheckData.m_GameObject;
			const SmartPtrs<GameObject>& i_GameObject2 = i_Collideable2.m_CachedCheckData.m_GameObject;

			const CollisionCheckData& i_CachedData1 = i_Collideable1.m_CachedCheckData;
			const CollisionCheckData& i_CachedData2 = i_Collideable2.m_CachedCheckData;

			float tEnter = 0.0f;
			float tExit = 1.0f;

			// Obj2 vs Obj1
			Vector3D Obj2RelVel = i_GameObject2->GetVelocitySyn() - i_GameObject1->GetVelocitySyn();
			Vector3D Obj2Travel = Obj2RelVel * i_dt;

			// Obj1 X Axis
			{
				Vector3D Obj1XAxisInWorld = i_CachedData1.m_OrientationInWorld.Col(0).Normalized();

				float Obj1BBCenterOnAxis = dot(i_CachedData1.m_BBCenterInWorld, Obj1XAxisInWorld);
				float Obj2BBCenterOnAxis = dot(i_CachedData2.m_BBCenterInWorld, Obj1XAxisInWorld);

				float Obj2ProjectedExtents =
					fabs(dot(Obj1XAxisInWorld, i_CachedData2.m_OrientationInWorld.Col(0).Normalized() * i_Collideable2.m_BoundingBox.m_Extents.x())) +
					fabs(dot(Obj1XAxisInWorld, i_CachedData2.m_OrientationInWorld.Col(1).Normalized() * i_Collideable2.m_BoundingBox.m_Extents.y())) +
					fabs(dot(Obj1XAxisInWorld, i_CachedData2.m_OrientationInWorld.Col(2).Normalized() * i_Collideable2.m_BoundingBox.m_Extents.z()));

				float Obj1ExpandedExtents = i_Collideable1.m_BoundingBox.m_Extents.x() + Obj2ProjectedExtents;

				float Obj2TravelAlongAxis = dot(Obj2Travel, Obj1XAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
					return false;

 				if (axisEnter > tEnter)
					tEnter = axisEnter;
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj1 Y Axis
			{
				Vector3D Obj1YAxisInWorld = i_CachedData1.m_OrientationInWorld.Col(1).Normalized();

				float Obj1BBCenterOnAxis = dot(i_CachedData1.m_BBCenterInWorld, Obj1YAxisInWorld);
				float Obj2BBCenterOnAxis = dot(i_CachedData2.m_BBCenterInWorld, Obj1YAxisInWorld);

				float Obj2ProjectedExtents =
					fabs(dot(Obj1YAxisInWorld, i_CachedData2.m_OrientationInWorld.Col(0).Normalized() * i_Collideable2.m_BoundingBox.m_Extents.x())) +
					fabs(dot(Obj1YAxisInWorld, i_CachedData2.m_OrientationInWorld.Col(1).Normalized() * i_Collideable2.m_BoundingBox.m_Extents.y())) +
					fabs(dot(Obj1YAxisInWorld, i_CachedData2.m_OrientationInWorld.Col(2).Normalized() * i_Collideable2.m_BoundingBox.m_Extents.z()));

				float Obj1ExpandedExtents = i_Collideable1.m_BoundingBox.m_Extents.y() + Obj2ProjectedExtents;

				float Obj2TravelAlongAxis = dot(Obj2Travel, Obj1YAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
					return false;

				if (axisEnter > tEnter)
					tEnter = axisEnter;
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj1 vs Obj2
			Vector3D Obj1RelVel = i_GameObject1->GetVelocitySyn() - i_GameObject2->GetVelocitySyn();
			Vector3D Obj1Travel = Obj1RelVel * i_dt;

			// Obj2 X Axis
			{
				Vector3D Obj2XAxisInWorld = i_CachedData2.m_OrientationInWorld.Col(0).Normalized();

				float Obj2BBCenterOnAxis = dot(i_CachedData2.m_BBCenterInWorld, Obj2XAxisInWorld);
				float Obj1BBCenterOnAxis = dot(i_CachedData1.m_BBCenterInWorld, Obj2XAxisInWorld);

				float Obj1ProjectedExtents =
					fabs(dot(Obj2XAxisInWorld, i_CachedData1.m_OrientationInWorld.Col(0).Normalized() * i_Collideable1.m_BoundingBox.m_Extents.x())) +
					fabs(dot(Obj2XAxisInWorld, i_CachedData1.m_OrientationInWorld.Col(1).Normalized() * i_Collideable1.m_BoundingBox.m_Extents.y())) +
					fabs(dot(Obj2XAxisInWorld, i_CachedData1.m_OrientationInWorld.Col(2).Normalized() * i_Collideable1.m_BoundingBox.m_Extents.z()));

				float Obj2ExpandedExtents = i_Collideable2.m_BoundingBox.m_Extents.x() + Obj1ProjectedExtents;

				float Obj1TravelAlongAxis = dot(Obj1Travel, Obj2XAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
					return false;

				if (axisEnter > tEnter)
					tEnter = axisEnter;
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj2 Y Axis
			{
				Vector3D Obj2YAxisInWorld = i_CachedData2.m_OrientationInWorld.Col(1).Normalized();

				float Obj2BBCenterOnAxis = dot(i_CachedData2.m_BBCenterInWorld, Obj2YAxisInWorld);
				float Obj1BBCenterOnAxis = dot(i_CachedData1.m_BBCenterInWorld, Obj2YAxisInWorld);

				float Obj1ProjectedExtents =
					fabs(dot(Obj2YAxisInWorld, i_CachedData1.m_OrientationInWorld.Col(0).Normalized() * i_Collideable1.m_BoundingBox.m_Extents.x())) +
					fabs(dot(Obj2YAxisInWorld, i_CachedData1.m_OrientationInWorld.Col(1).Normalized() * i_Collideable1.m_BoundingBox.m_Extents.y())) +
					fabs(dot(Obj2YAxisInWorld, i_CachedData1.m_OrientationInWorld.Col(2).Normalized() * i_Collideable1.m_BoundingBox.m_Extents.z()));

				float Obj2ExpandedExtents = i_Collideable2.m_BoundingBox.m_Extents.y() + Obj1ProjectedExtents;

				float Obj1TravelAlongAxis = dot(Obj1Travel, Obj2YAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
					return false;

				if (axisEnter > tEnter)
					tEnter = axisEnter;
				if (axisExit < tExit)
					tExit = axisExit;
			}

			o_tCollision = tEnter;
			return tEnter < tExit;
		}

		bool FindCollision(float i_dt, CollisionPair& o_CollisionPair)
		{
			bool bFoundCollision = false;
			const size_t count = AllCollideables.size();

			if (count > 1)
			{
				for (size_t i = 0; i < (count - 1); i++)
				{
					for (size_t j = i + 1; j < count; j++)
					{
						float tCollision = 0.0f;

						if (CheckCollision2D(AllCollideables[i], AllCollideables[j], i_dt, tCollision))
						{
							if (!bFoundCollision || (bFoundCollision && (tCollision < o_CollisionPair.m_CollisionTime)))
							{
								o_CollisionPair.m_pCollideables[0] = &AllCollideables[i];
								o_CollisionPair.m_pCollideables[1] = &AllCollideables[j];
								o_CollisionPair.m_CollisionTime = tCollision;

								bFoundCollision = true;
							}
						}
					}
				}
			}

			return bFoundCollision;
		}

		bool CheckForCollisions(float i_dt)
		{
			bool bFoundCollision = false;

			CacheCollisionCheckData();

			CollisionPair FoundCollision;

			if (FindCollision(i_dt, FoundCollision))
			{
				assert(FoundCollision.m_pCollideables[0]);
				assert(FoundCollision.m_pCollideables[1]);

				if (FoundCollision.m_pCollideables[0]->m_CollisionCallback)
					FoundCollision.m_pCollideables[0]->m_CollisionCallback(FoundCollision.m_pCollideables[1]->m_GameObject);

				if (FoundCollision.m_pCollideables[1]->m_CollisionCallback)
					FoundCollision.m_pCollideables[1]->m_CollisionCallback(FoundCollision.m_pCollideables[0]->m_GameObject);

				bFoundCollision = true;
			}

			ReleaseCachedCollisionCheckData();

			return bFoundCollision;
		}

	} // namespace Collision
} // namespace Engine