#include "StartupShutdown.h"

#include "Timing.h"
#include "Physic.h"
#include "Collision.h"
#include "ScopeLock.h"
#include "WeakPtrs.h"
#include "SmartPtrs.h"

#include "GLib.h"

#include <Windows.h>
#include <DirectXColors.h>
#include <vector>
#include "JobSystem.h"
#include "ConsolePrint.h"

namespace Engine
{
	bool GameStart = false;

	bool QuitGame = false;

	float GetFrameTime();

	static bool bGLibInitialized = false;

	void SetPlayerController(SmartPtrs<GameObject>& i_PlayerController)
	{
		PlayerControllers.push_back(i_PlayerController);
	}

	std::vector<SmartPtrs<GameObject>> AllGameObject;
	SmartPtrs<GameObject> UIObject;

	void AddRenderable(SmartPtrs<GameObject>& i_GameObject)
	{
		AllGameObject.push_back(i_GameObject);
	}

	void SetUIObject(SmartPtrs<GameObject>& i_GameObject)
	{
		UIObject = i_GameObject;
	}

	std::vector<std::function<void(unsigned int, bool)>> KeyChangeCallbacks;

	void RegisterOnKeyChange(std::function<void(unsigned int, bool)> i_OnKeyChange)
	{
		KeyChangeCallbacks.push_back(i_OnKeyChange);
	}

	void InputInit()
	{
		GLib::SetKeyStateChangeCallback([](unsigned int i_VKeyID, bool i_bDown)
			{
				for (auto k : KeyChangeCallbacks)
				{
					if (k)
						k(i_VKeyID, i_bDown);
				}
			}
		);
	}

	bool Startup(HINSTANCE i_hInstance, int i_nCmdShow, const char* i_pWindowName, WORD i_IconID, int graphSize_Width, int graphSize_Hight)
	{
		//Init for Glib and graphic
		bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, i_pWindowName, i_IconID, graphSize_Width, graphSize_Hight, true);
		InputInit();
		JobSystem::Init();
		return bSuccess;
	}

	void RenderAllGameObject()
	{
		// IMPORTANT: Tell GLib that we want to start rendering
		GLib::BeginRendering(DirectX::Colors::White);
		// Tell GLib that we want to render some sprites
		GLib::Sprites::BeginRendering();

		static float timer = 0;

		for (int i = 0; i < AllGameObject.size(); i++)
		{
			//Render All Object
			GLib::Point2D Position = { AllGameObject[i]->GetPositionRender().GetX(), AllGameObject[i]->GetPositionRender().GetY() };
			GLib::Render(*(AllGameObject[i]->m_Sprite), Position, 0.0f, AllGameObject[i]->GetZRotation());
		}

		timer += 0.01f;

		// Tell GLib we're done rendering sprites
		GLib::Sprites::EndRendering();
		// IMPORTANT: Tell GLib we're done rendering
		GLib::EndRendering();
	}

	void AllAIMove(float i_dt)
	{
		for (int i = 0; i < AllGameObject.size(); i++)
		{
			//Auto move for All Object
			AllGameObject[i]->updatePosition(i_dt);
			if (AllGameObject[i]->Update.m_CollisionCallback)
			{
				AllGameObject[i]->Update.m_CollisionCallback(AllGameObject[i]->Update.m_GameObject, i_dt);
			}
		}
	}

	void StartAndStopGame(bool i_GameStart)
	{
		GameStart = i_GameStart;
	}

	bool GetGameStart()
	{
		return GameStart;
	}

	void ResetGame()
	{
		//StartAndStopGame(false);

		AllGameObject.clear();
		Collision::RemoveAllCollisionCollidaable();
	}

	void RenderUIObject()
	{
		// IMPORTANT: Tell GLib that we want to start rendering
		GLib::BeginRendering(DirectX::Colors::White);
		// Tell GLib that we want to render some sprites
		GLib::Sprites::BeginRendering();

		//Render All Object
		GLib::Point2D Position = { 0.0f, -400 };
		GLib::Render(*(UIObject->m_Sprite), Position, 0.0f, 0.0f);

		// Tell GLib we're done rendering sprites
		GLib::Sprites::EndRendering();
		// IMPORTANT: Tell GLib we're done rendering
		GLib::EndRendering();
	}

	void Run()
	{
		bool bDone = false;
		bool bQuit = false;

		do
		{
			bQuit = QuitGame;

			GLib::Service(bDone);
			if (GameStart)
			{
				float dt = GetFrameTime();
				//Physics::Tick(dt);
				Collision::Tick(dt);

				RenderAllGameObject();

				AllAIMove(dt);

				//Renderer::SetClearColor(Collision::FoundCollisionLastTick() ? DirectX::Colors::Red : DirectX::Colors::Blue);
				//Renderer::Tick(dt);
			}
			else
			{
				RenderUIObject();
			}
		} while (bDone == false && bQuit == false);
	}

	void RemoveGameObjectFromAllGameObject(SmartPtrs<GameObject>& i_GameObject)
	{
		size_t count = AllGameObject.size();

		for (size_t i = 0; i < count; i++)
		{
			if (AllGameObject[i] == i_GameObject)
			{
				auto iter = AllGameObject.erase(AllGameObject.begin() + i);
				DEBUG_PRINT("Remove gameobject  from AllGameObject: index = " + i);
				break;
			}
		}
	}

	void RemoveGameObjectFromTheWorld(SmartPtrs<GameObject>& i_GameObject)
	{
		Collision::RemoveCollisionCollidaable(i_GameObject);
		RemoveGameObjectFromAllGameObject(i_GameObject);
	}

	void Shutdown()
	{
		Collision::Shutdown();

		AllGameObject.clear();


		GLib::Shutdown();
	}

	float GetFrameTime()
	{
		static Timing::tick_t lastFrameStartTick = 0;

		float FrameTime;

		Timing::tick_t currentFrameStartTick = Timing::GetCurrentTickCounter();

		FrameTime = (lastFrameStartTick == 0) ? (1.0f / 60.0f) : (Timing::GetTimeDiff_ms(currentFrameStartTick, lastFrameStartTick) / 1000.0f);

		lastFrameStartTick = currentFrameStartTick;

		return IsDebuggerPresent() ? (1.0f / 60.0f) : FrameTime;
	}

	void Quit()
	{
		QuitGame = true;
	}
}