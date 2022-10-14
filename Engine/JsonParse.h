#pragma once
#include "json.hpp"
#include "LoadFileToBuffer.h"
#include "SmartPtrs.h"
#include "ProcessFileJob.h"
#include "GameObject.h"
#include <assert.h>
#include <fstream>
#include <functional>
#include <map>
#include <iostream>
#include "Collision.h"
#include "StartupShutdown.h"

namespace Engine
{
	nlohmann::json ParseJSON(const char* i_pFilename)
	{
		std::vector<uint8_t> PlayerData = LoadFileToBuffer(i_pFilename);

		if (!PlayerData.empty())
		{
			nlohmann::json PlayerJSON = nlohmann::json::parse(PlayerData);
			return PlayerJSON;
		}
	}

	void operator<<(Engine::Vector2D& vec, nlohmann::json& json_obj)
	{
		assert(json_obj.is_array() && json_obj.size() == 2);

		vec.SetX(json_obj[0]);
		vec.SetY(json_obj[1]);
	}

	void* LoadFile(const char* i_pFilename, size_t& o_sizeFile)
	{
		assert(i_pFilename != NULL);

		FILE* pFile = NULL;

		errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
		if (fopenError != 0)
			return NULL;

		assert(pFile != NULL);

		int FileIOError = fseek(pFile, 0, SEEK_END);
		assert(FileIOError == 0);

		long FileSize = ftell(pFile);
		assert(FileSize >= 0);

		FileIOError = fseek(pFile, 0, SEEK_SET);
		assert(FileIOError == 0);

		uint8_t* pBuffer = new uint8_t[FileSize];
		assert(pBuffer);

		size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
		assert(FileRead == FileSize);

		fclose(pFile);

		o_sizeFile = FileSize;

		return pBuffer;
	}

	GLib::Sprite* CreateSprite(const char* i_pFilename)
	{
		assert(i_pFilename);

		size_t sizeTextureFile = 0;

		// Load the source file (texture data)
		void* pTextureFile = LoadFile(i_pFilename, sizeTextureFile);

		// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
		GLib::Texture* pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

		// exit if something didn't work
		// probably need some debug logging in here!!!!
		if (pTextureFile)
			delete[] pTextureFile;

		if (pTexture == nullptr)
			return nullptr;

		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int depth = 0;

		// Get the dimensions of the texture. We'll use this to determine how big it is on screen
		bool result = GLib::GetDimensions(*pTexture, width, height, depth);
		assert(result == true);
		assert((width > 0) && (height > 0));

		// Define the sprite edges
		GLib::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
		GLib::SpriteUVs	UVs = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };
		GLib::RGBA							Color = { 255, 255, 255, 255 };

		// Create the sprite
		GLib::Sprite* pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);

		// release our reference on the Texture
		GLib::Release(pTexture);

		return pSprite;
	}

	SmartPtrs<GameObject> CreateObjectFromJSON(nlohmann::json& i_data)
	{
		//name
		std::string Name = i_data["name"];

		//mass
		float mass = i_data["mass"];

		//pos
		Engine::Vector2D InitialPosition;
		InitialPosition << i_data["initial_position"];

		//vel
		Engine::Vector2D InitialVelocity;
		InitialVelocity << i_data["initial_velocity"];

		GLib::Sprite* sprite = nullptr;
		//sprite
		if (i_data.contains("render_data"))
		{
			assert(i_data["render_data"]["sprite"].is_string());

			std::string SpriteAddress = i_data["render_data"]["sprite"];

			sprite = CreateSprite(SpriteAddress.data());
		}

		//render offset
		Engine::Vector2D RenderOffset = Engine::Vector2D(0, 0);
		if (i_data.contains("render_offset"))
		{
			RenderOffset << i_data["render_offset"];
		}

		//isApplyGravity
		bool isApplyGravity = i_data["isApplyGravity"] == 0 ? false : true;

		// SmartPtr<GameObject> Player = GameObject::Create(Name, InitialPosition);
		SmartPtrs<GameObject> Object = GameObject::Create(Name, mass, InitialPosition, InitialVelocity, RenderOffset, sprite, isApplyGravity);

		if (i_data.contains("Collision_data"))
		{
			//assert(i_data["Collision_data"]["offset"] && i_data["Collision_data"]["extents"]);

			nlohmann::json it = i_data["Collision_data"];
			Collision::CreateCollidableFromJSON(Object, it);
		}

		if (i_data.contains("render_data"))
		{
			AddRenderable(Object);
		}

		return Object;
	}

	static void CreateGameObjectFromJSONData(std::vector<uint8_t> i_JSONData, bool i_bIsCBOR, std::function<void(SmartPtrs<GameObject>&)> i_OnCreated)
	{

		if (!i_JSONData.empty())
		{
			//nlohmann::json JSON = i_bIsCBOR ? nlohmann::json::from_cbor(i_JSONData) : nlohmann::json::parse(i_JSONData);
			nlohmann::json JSON = nlohmann::json::parse(i_JSONData);

			SmartPtrs<GameObject> NewObject = CreateObjectFromJSON(JSON);

			if (i_OnCreated)
				i_OnCreated(NewObject);
		}
	}

	void CreateGameObjectWithJobSystem(const std::string& i_JSONFilename, std::function<void(SmartPtrs<GameObject>&)> i_OnCreated, JobSystem::JobStatus* i_pStatus)
	{
		using namespace std::placeholders;
		ProcessFileAsync(i_JSONFilename, std::bind(CreateGameObjectFromJSONData, _1, false, i_OnCreated), { JobSystem::GetDefaultQueue(), i_pStatus });
	}
}