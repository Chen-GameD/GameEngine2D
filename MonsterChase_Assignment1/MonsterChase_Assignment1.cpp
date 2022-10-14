#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <assert.h>

#include <Windows.h>

#include <DirectXColors.h>

#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include "GLib.h"
#include "EngineInclude.h"
#include "JsonParse.h"
#include "JobStatus.h"
#include "Vector2D.h"
#include "Collision.h"
#include "Vector3D.h"
#include "StartupShutdown.h"

using namespace Engine;

int num_of_new_monster = 0;

unsigned int keyID = 0;

//SSE_DOT_PRODUCT
float My_SSE_Dot_Product(__m128 x, __m128 y)
{
	__m128 mulRes;
	__m128 shufReg, sumsReg;
	mulRes = _mm_mul_ps(x, y);

	//Calculates
	shufReg = _mm_movehdup_ps(mulRes);
	sumsReg = _mm_add_ps(mulRes, shufReg);
	shufReg = _mm_movehl_ps(shufReg, sumsReg); 
	sumsReg = _mm_add_ss(sumsReg, shufReg);
	return  _mm_cvtss_f32(sumsReg); 
}

//SSE_CROSS_PRODUCT
__m128 My_SSE_Cross_Product(__m128 const& vec0, __m128 const& vec1) 
{
	__m128 tmp0 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 tmp1 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 tmp2 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 tmp3 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 0, 2, 1));
	return _mm_sub_ps(
		_mm_mul_ps(tmp0, tmp1),
		_mm_mul_ps(tmp2, tmp3)
	);
}

void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
	const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	OutputDebugStringA(Buffer);
#endif // __DEBUG

	keyID = bWentDown ? i_VKeyID : 0;
}

auto BulletUpdate = [](WeakPtrs<GameObject>& i_GameObject, float i_dt)
{
	Engine::Vector2D currentPos = i_GameObject.lock()->GetPosition();
	if (currentPos.GetX() >= 800 || currentPos.GetX() <= -800)
	{
		SmartPtrs<GameObject> targetGO = i_GameObject.lock();
		RemoveGameObjectFromTheWorld(targetGO);
	}
	if (currentPos.GetY() >= 400 || currentPos.GetY() <= -400)
	{
		SmartPtrs<GameObject> targetGO = i_GameObject.lock();
		RemoveGameObjectFromTheWorld(targetGO);
	}
};

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	// IMPORTANT: first we need to initialize GLib
	int graphSize_Width = 1600;
	int graphSize_Hight = 800;

	bool bSuccess = Engine::Startup(i_hInstance, i_nCmdShow, "GlibTest", -1, graphSize_Width, graphSize_Hight);

	if (bSuccess)
	{
		Engine::Timing::Init();

		static Engine::SmartPtrs<GameObject> PlayerControl;
		static std::list<Engine::SmartPtrs<GameObject>> Bullets;

		static bool GStart = false;

		//StartMenu
		//Engine::UIRender("data\\UI\\StartMenu.dds");
		// IMPORTANT: Tell GLib that we want to start rendering
		Engine::SmartPtrs<GameObject> startMenu;
		Engine::JobSystem::JobStatus jobStatus;
		Engine::CreateGameObjectWithJobSystem("data\\JsonData\\StartMenu.json", [&startMenu](SmartPtrs<GameObject>& i_TheGameObject)
		{
			startMenu = i_TheGameObject;
		}, &jobStatus);

		jobStatus.WaitForZeroJobsLeft();

		Engine::SetUIObject(startMenu);

		Engine::RegisterOnKeyChange([](unsigned int i_VKey, bool i_bIsDown)
		{
			// press F1 to start
			using namespace Engine;

			if ((i_VKey == VK_F1) && i_bIsDown)
			{
				//If game already start, do nothing.
				GStart = Engine::GetGameStart();

				if (GStart == true)
					return;

				Engine::ResetGame();
				GStart = true;
				Engine::StartAndStopGame(true);
				Engine::SmartPtrs<GameObject> GoodGuy;
				Engine::SmartPtrs<GameObject> BadGuy;
				Engine::SmartPtrs<GameObject> Block1;
				Engine::SmartPtrs<GameObject> Block2;
				Engine::SmartPtrs<GameObject> Block3;

				Engine::JobSystem::JobStatus jobStatus;

				Engine::CreateGameObjectWithJobSystem("data\\JsonData\\Player.json", [&GoodGuy](SmartPtrs<GameObject>& i_TheGameObject)
				{
					i_TheGameObject->SetPosition(Engine::Vector2D(700, 0));
					GoodGuy = i_TheGameObject;
				}, &jobStatus);

				Engine::CreateGameObjectWithJobSystem("data\\JsonData\\Monster.json", [&BadGuy](SmartPtrs<GameObject>& i_TheGameObject)
				{
					i_TheGameObject->SetPosition(Engine::Vector2D(-700, 0));
					BadGuy = i_TheGameObject;
				}, &jobStatus);

				Engine::CreateGameObjectWithJobSystem("data\\JsonData\\BlockWall.json", [&Block1](SmartPtrs<GameObject>& i_TheGameObject)
				{
					i_TheGameObject->SetPosition(Engine::Vector2D(300, 0));
					Block1 = i_TheGameObject;
				}, &jobStatus);
				Engine::CreateGameObjectWithJobSystem("data\\JsonData\\BlockWall.json", [&Block2](SmartPtrs<GameObject>& i_TheGameObject)
				{
					i_TheGameObject->SetPosition(Engine::Vector2D(0, 0));
					Block2 = i_TheGameObject;
				}, &jobStatus);
				Engine::CreateGameObjectWithJobSystem("data\\JsonData\\BlockWall.json", [&Block3](SmartPtrs<GameObject>& i_TheGameObject)
				{
					i_TheGameObject->SetPosition(Engine::Vector2D(-300, 0));
					Block3 = i_TheGameObject;
				}, &jobStatus);

				jobStatus.WaitForZeroJobsLeft();

				//Engine::SetPlayerController(GoodGuy);
				PlayerControl = GoodGuy;

				auto PlayerOnCollision = [](WeakPtrs<GameObject>& i_GameObject)
				{
					//printf("Detect Collision!");
#ifdef _DEBUG
					const size_t	lenBuffer = 65;
					char			Buffer[lenBuffer];

					sprintf_s(Buffer, lenBuffer, "Detect Player Collision\n");
					OutputDebugStringA(Buffer);
#endif // __DEBUG
					//DebugBreak();
				};

				auto MonsterOnCollision = [](WeakPtrs<GameObject>& i_GameObject)
				{
					//printf("Detect Collision!");
#ifdef _DEBUG
					const size_t	lenBuffer = 65;
					char			Buffer[lenBuffer];

					sprintf_s(Buffer, lenBuffer, "Detect Monster Collision\n");
					OutputDebugStringA(Buffer);
#endif // __DEBUG
					//DebugBreak();
				};

				auto BlockOnCollision = [](WeakPtrs<GameObject>& i_GameObject)
				{
					//printf("Detect Collision!");
#ifdef _DEBUG
					const size_t	lenBuffer = 65;
					char			Buffer[lenBuffer];

					sprintf_s(Buffer, lenBuffer, "Detect Block Collision\n");
					OutputDebugStringA(Buffer);
#endif // __DEBUG
					//DebugBreak();

					if (i_GameObject.lock()->m_Tag != "Monster")
					{
						if (i_GameObject.lock()->m_Tag != "Player")
						{
							SmartPtrs<GameObject> targetGO = i_GameObject.lock();
							RemoveGameObjectFromTheWorld(targetGO);
						}
						else
						{
							SmartPtrs<GameObject> targetGO = i_GameObject.lock();
							RemoveGameObjectFromTheWorld(targetGO);

							Engine::ResetGame();
							Engine::StartAndStopGame(false);

							Engine::SmartPtrs<GameObject> LoseMenu;
							Engine::JobSystem::JobStatus jobStatus;
							Engine::CreateGameObjectWithJobSystem("data\\JsonData\\LoseMenu.json", [&LoseMenu](SmartPtrs<GameObject>& i_TheGameObject)
								{
									LoseMenu = i_TheGameObject;
								}, &jobStatus);

							jobStatus.WaitForZeroJobsLeft();

							Engine::SetUIObject(LoseMenu);
						}
					}
				};

				Collision::SetCollisionCollidaable(GoodGuy, PlayerOnCollision);
				Collision::SetCollisionCollidaable(BadGuy, MonsterOnCollision);
				Collision::SetCollisionCollidaable(Block1, BlockOnCollision);
				Collision::SetCollisionCollidaable(Block2, BlockOnCollision);
				Collision::SetCollisionCollidaable(Block3, BlockOnCollision);

				auto MonsterUpdate = [](WeakPtrs<GameObject>& i_GameObject, float i_dt)
				{
					//printf("Detect Collision!");
#ifdef _DEBUG
					const size_t	lenBuffer = 65;
					char			Buffer[lenBuffer];

					sprintf_s(Buffer, lenBuffer, "MonsterUpdate\n");
					OutputDebugStringA(Buffer);
#endif // __DEBUG
					//DebugBreak();

					static float timer_move = 0.0f;
					static float timer_shoot = 0.0f;

					if (timer_move >= 100)
					{
						i_GameObject.lock()->addDrag(Dir::NONE, 1);

						int i = rand() % 4;
						switch (i)
						{
						case 0:
							i_GameObject.lock()->addDrag(Dir::UP, 1);
							i_GameObject.lock()->SetZRotation(0);
							i_GameObject.lock()->m_dir = Dir::UP;
							break;
						case 1:
							i_GameObject.lock()->addDrag(Dir::DOWN, 1);
							i_GameObject.lock()->SetZRotation(3.14);
							i_GameObject.lock()->m_dir = Dir::DOWN;
							break;
						case 2:
							i_GameObject.lock()->addDrag(Dir::LEFT, 1);
							i_GameObject.lock()->SetZRotation(1.57);
							i_GameObject.lock()->m_dir = Dir::LEFT;
							break;
						case 3:
							i_GameObject.lock()->addDrag(Dir::RIGHT, 1);
							i_GameObject.lock()->SetZRotation(4.71);
							i_GameObject.lock()->m_dir = Dir::RIGHT;
							break;
						}

						//i_GameObject.lock()->addDrag(Dir::UP);
						timer_move = 0;
					}

					if (timer_shoot >= 150)
					{
						Engine::SmartPtrs<GameObject> NewBullet;

						Engine::JobSystem::JobStatus jobStatus;

						Engine::Vector2D newPos = i_GameObject.lock()->GetPosition();

						Engine::CreateGameObjectWithJobSystem("data\\JsonData\\BulletMonster.json", [&NewBullet](SmartPtrs<GameObject>& i_TheGameObject)
							{
								i_TheGameObject->SetPosition(Engine::Vector2D(0, 0));
								NewBullet = i_TheGameObject;
							}, &jobStatus);

						jobStatus.WaitForZeroJobsLeft();

						NewBullet->SetPosition(newPos);
						NewBullet->SetZRotation(i_GameObject.lock()->GetZRotation());
						NewBullet->m_dir = i_GameObject.lock()->m_dir;
						NewBullet->addDrag(i_GameObject.lock()->m_dir, 2);

						Bullets.push_back(NewBullet);

						timer_shoot = 0;

						auto BulletMonsterOnCollision = [](WeakPtrs<GameObject>& i_GameObject)
						{
							//printf("Detect Collision!");
#ifdef _DEBUG
							const size_t	lenBuffer = 65;
							char			Buffer[lenBuffer];

							sprintf_s(Buffer, lenBuffer, "Detect Player Bullet Collision\n");
							OutputDebugStringA(Buffer);
#endif // __DEBUG
							//DebugBreak();

							if (i_GameObject.lock()->m_Tag == "Player")
							{
								SmartPtrs<GameObject> targetGO = i_GameObject.lock();
								RemoveGameObjectFromTheWorld(targetGO);

								Engine::ResetGame();
								Engine::StartAndStopGame(false);

								Engine::SmartPtrs<GameObject> LoseMenu;
								Engine::JobSystem::JobStatus jobStatus;
								Engine::CreateGameObjectWithJobSystem("data\\JsonData\\LoseMenu.json", [&LoseMenu](SmartPtrs<GameObject>& i_TheGameObject)
									{
										LoseMenu = i_TheGameObject;
									}, & jobStatus);

								jobStatus.WaitForZeroJobsLeft();

								Engine::SetUIObject(LoseMenu);
							}
						};

						Collision::SetCollisionCollidaable(NewBullet, BulletMonsterOnCollision);

						NewBullet->SetUpdateFunction(NewBullet, BulletUpdate);
					}

					timer_move += i_dt;
					timer_shoot += i_dt;

					Engine::Vector2D currentPos = i_GameObject.lock()->GetPosition();
					if (currentPos.GetX() >= 800 || currentPos.GetX() <= -800)
					{
						i_GameObject.lock()->SetPosition(Engine::Vector2D(-currentPos.GetX(), currentPos.GetY()));
					}
					if (currentPos.GetY() >= 400 || currentPos.GetY() <= -400)
					{
						i_GameObject.lock()->SetPosition(Engine::Vector2D(currentPos.GetX(), -currentPos.GetY()));
					}
				};

				auto PlayerUpdate = [](WeakPtrs<GameObject>& i_GameObject, float i_dt)
				{
					Engine::Vector2D currentPos = i_GameObject.lock()->GetPosition();
					if (currentPos.GetX() >= 800 || currentPos.GetX() <= -800)
					{
						i_GameObject.lock()->SetPosition(Engine::Vector2D(-currentPos.GetX(), currentPos.GetY()));
					}
					if (currentPos.GetY() >= 400 || currentPos.GetY() <= -400)
					{
						i_GameObject.lock()->SetPosition(Engine::Vector2D(currentPos.GetX(), -currentPos.GetY()));
					}
				};

				auto BlockUpdate = [](WeakPtrs<GameObject>& i_GameObject, float i_dt)
				{
					Engine::Vector2D currentPos = i_GameObject.lock()->GetPosition();
					if (currentPos.GetX() >= 800 || currentPos.GetX() <= -800)
					{
						i_GameObject.lock()->SetPosition(Engine::Vector2D(-currentPos.GetX(), currentPos.GetY()));
					}
					if (currentPos.GetY() >= 400 || currentPos.GetY() <= -400)
					{
						i_GameObject.lock()->SetPosition(Engine::Vector2D(currentPos.GetX(), -currentPos.GetY()));
					}
				};
				Block1->addDrag(Dir::UP, 1);
				Block2->addDrag(Dir::DOWN, 1);
				Block3->addDrag(Dir::UP, 1);

				BadGuy->SetUpdateFunction(BadGuy, MonsterUpdate);
				GoodGuy->SetUpdateFunction(GoodGuy, PlayerUpdate);
				Block1->SetUpdateFunction(Block1, BlockUpdate);
				Block2->SetUpdateFunction(Block2, BlockUpdate);
				Block3->SetUpdateFunction(Block3, BlockUpdate);
			}

			//PlayerInput
			//W
			if (i_VKey == 87 && i_bIsDown)
			{
				PlayerControl->addDrag(Dir::UP, 1);
				PlayerControl->SetZRotation(0);
				PlayerControl->m_dir = Dir::UP;
			}
			if (i_VKey == 87 && !i_bIsDown)
			{
				PlayerControl->addDrag(Dir::DOWN, 1);
			}

			//S
			if (i_VKey == 83 && i_bIsDown)
			{
				PlayerControl->addDrag(Dir::DOWN, 1);
				PlayerControl->SetZRotation(3.14);
				PlayerControl->m_dir = Dir::DOWN;
			}
			if (i_VKey == 83 && !i_bIsDown)
			{
				PlayerControl->addDrag(Dir::UP, 1);
			}

			//A
			if (i_VKey == 65 && i_bIsDown)
			{
				PlayerControl->addDrag(Dir::LEFT, 1);
				PlayerControl->SetZRotation(1.57);
				PlayerControl->m_dir = Dir::LEFT;
			}
			if (i_VKey == 65 && !i_bIsDown)
			{
				PlayerControl->addDrag(Dir::RIGHT, 1);
			}

			//D
			if (i_VKey == 68 && i_bIsDown)
			{
				PlayerControl->addDrag(Dir::RIGHT, 1);
				PlayerControl->SetZRotation(4.71);
				PlayerControl->m_dir = Dir::RIGHT;
			}
			if (i_VKey == 68 && !i_bIsDown)
			{
				PlayerControl->addDrag(Dir::LEFT, 1);
			}

			//space
			if (i_VKey == VK_SPACE && i_bIsDown)
			{
				Engine::SmartPtrs<GameObject> NewBullet;

				Engine::JobSystem::JobStatus jobStatus;

				Engine::Vector2D newPos = PlayerControl->GetPosition();

				Engine::CreateGameObjectWithJobSystem("data\\JsonData\\BulletPlayer.json", [&NewBullet](SmartPtrs<GameObject>& i_TheGameObject)
				{
					i_TheGameObject->SetPosition(Engine::Vector2D(0, 0));
					NewBullet = i_TheGameObject;
				}, & jobStatus);

				jobStatus.WaitForZeroJobsLeft();

				NewBullet->SetPosition(newPos);
				NewBullet->SetZRotation(PlayerControl->GetZRotation());
				NewBullet->m_dir = PlayerControl->m_dir;
				NewBullet->addDrag(PlayerControl->m_dir, 2);

				Bullets.push_back(NewBullet);

				auto BulletPlayerOnCollision = [](WeakPtrs<GameObject>& i_GameObject)
				{
					//printf("Detect Collision!");
#ifdef _DEBUG
					const size_t	lenBuffer = 65;
					char			Buffer[lenBuffer];

					sprintf_s(Buffer, lenBuffer, "Detect Player Bullet Collision\n");
					OutputDebugStringA(Buffer);
#endif // __DEBUG
					//DebugBreak();

					if (i_GameObject.lock()->m_Tag == "Monster")
					{
						SmartPtrs<GameObject> targetGO = i_GameObject.lock();
						RemoveGameObjectFromTheWorld(targetGO);

						Engine::ResetGame();
						Engine::StartAndStopGame(false);

						Engine::SmartPtrs<GameObject> WinMenu;
						Engine::JobSystem::JobStatus jobStatus;
						Engine::CreateGameObjectWithJobSystem("data\\JsonData\\WinMenu.json", [&WinMenu](SmartPtrs<GameObject>& i_TheGameObject)
							{
								WinMenu = i_TheGameObject;
							}, & jobStatus);

						jobStatus.WaitForZeroJobsLeft();

						Engine::SetUIObject(WinMenu);
					}
				};

				Collision::SetCollisionCollidaable(NewBullet, BulletPlayerOnCollision);

				NewBullet->SetUpdateFunction(NewBullet, BulletUpdate);
			}

			//Quit
			if (i_VKey == VK_F2 && i_bIsDown)
			{
				//Quit the game
				Engine::Quit();
			}
		});

		bool bQuit = false;

		Engine::Run();

		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		Engine::Shutdown();
		PlayerControl.release();
		Bullets.clear();
		startMenu.release();
	}
}