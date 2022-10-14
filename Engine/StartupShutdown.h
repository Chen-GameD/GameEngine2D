#pragma once

#include <Windows.h>

#include "GameObject.h"

#include <functional>
#include <vector>
namespace Engine
{
	static SmartPtrs<GameObject> PlayerController;
	static std::vector<SmartPtrs<GameObject>> PlayerControllers;

	void SetPlayerController(SmartPtrs<GameObject>& i_PlayerController);

	void SetUIObject(SmartPtrs<GameObject>& i_GameObject);

	void RegisterOnKeyChange(std::function<void(unsigned int, bool)> i_OnKeyChange);

	bool Startup(HINSTANCE i_hInstance, int i_nCmdShow, const char* i_pWindowName, WORD i_IconID, int graphSize_Width, int graphSize_Hight);

	void StartAndStopGame(bool i_GameStart);

	bool GetGameStart();

	void ResetGame();

	void Run();

	void AddRenderable(SmartPtrs<GameObject>& i_GameObject);

	void RemoveGameObjectFromTheWorld(SmartPtrs<GameObject>& i_GameObject);

	void Quit();

	void Shutdown();

} // namespace Engine