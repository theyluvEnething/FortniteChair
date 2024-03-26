#pragma once
#include "../../render/render.h"


namespace Settings
{
	inline bool ShowMenu = true;
	inline int Tab = 0;
	inline bool Initialized;
	inline LPCSTR ConfigPath = "";
	namespace CloseRange
	{
		inline bool Enabled = false;
		inline float distance = 30;

		inline bool DynamicFov = true;
		inline float MaxFov = 280;
		inline float CurrentFov = 140;

		inline float		SmoothX = 2.5f;
		inline float		SmoothY = 2.5f;
		inline bool			LockSmooth = true;
		inline bool			SmartSmooth = false;
		inline bool			InstantInterpolation = false;
		inline bool			InterpolateColor = true;

		inline bool			TriggerBot = false;

		inline bool			LockColors = true;
		inline ImColor		BoxColor = ImColor(255, 255, 255, 255);
		inline ImColor		BoneColor = ImColor(255, 255, 255, 255);
		inline ImColor		TracesColor = ImColor(255, 255, 255, 255);

		inline float lineThickness = 1.8f;

		inline bool isActive = false;

	}
	namespace Aimbot
	{
		inline bool			Enabled				= true;
		inline bool			ShowFov				= true;
		inline bool			FillFovCircle		= true;
		inline bool			Predict				= true;
		inline float		Fov					= 150;
		inline ImColor		FovColor			= ImColor(255, 20, 20, 255);
		inline float		SmoothX				= 10;
		inline float		SmoothY             = 10;
		inline bool			LockSmooth			= true;
		inline const char* Aimkey[] = { "left mouse button", "right mouse button", "mouse button 4", "mouse button 5", "left ctrl", "left shift", "left alt" };
		inline int			CurrentAimkey		= 1;
		inline int			CurrentKey			= VK_RBUTTON;
	
		inline const char*  TargetPart[]		= { "head", "neck", "hip"};
		inline int			CurrentTargetPart = 0;
	}
	namespace Visuals
	{
		inline bool			Enabled				= true;
		inline bool			TeamEnabled			= true;
		inline bool			BotEnabled			= true;
		inline bool			Box					= true;
		inline bool			FillBox				= true;
		inline bool			Traces				= false;
		inline bool			Distance			= true;
		inline bool			Bone				= true;
		inline bool			BoneOnSelf			= false;

		inline bool			BoxOnTeam			= true;
		inline bool			FillBoxOnTeam		= true;
		inline bool			TracesOnTeam		= false;
		inline bool			DistanceOnTeam		= true;
		inline bool			BoneOnTeam			= false;

		inline bool			BoxOnBot			= true;
		inline bool			FillBoxOnBot		= true;
		inline bool			TracesOnBot			= false;
		inline bool			DistanceOnBot		= true;
		inline bool			BoneOnBot			= false;
	

		inline bool			LockColors			= true;
		inline bool			LockColorsTeam		= true;
		inline bool			LockColorsBot		= true;
		inline float		TraceLineThickness	= 1.3f;
		inline float		BoxLineThickness	= 1;
		inline float		BoneLineThickness	= 2.1;
		inline ImColor		BoxColor			= ImColor(255, 0, 0, 255);
		inline ImColor		BoxFillColor		= ImColor(0, 0, 0, 120);
		inline ImColor		BoneColor			= ImColor(255, 0, 0, 255);
		inline ImColor		DistanceColor		= ImColor(255, 0, 0, 255);
		inline ImColor		TracesColor			= ImColor(255, 0, 0, 255);

		inline float		TextSize			= 1.0f;

		inline float		BoneDisplayRange	= 25.0f;

		
		inline const char*	TracesOptions[]		= { "bottom", "center", "top"};
		inline int			TracesHeight		= 0;
		inline int			CurrentTracesOption	= 1;
		
		inline ImColor		TeamBoxColor		= ImColor(42, 255, 0, 255);
		inline ImColor		TeamBoxFillColor	= ImColor(0, 0, 0, 120);
		inline ImColor		TeamBoneColor		= ImColor(42, 255, 0, 255);
		inline ImColor		TeamTracesColor		= ImColor(42, 255, 0, 255);
		inline ImColor		TeamDistanceColor	= ImColor(42, 255, 0, 255);


		inline ImColor		BotBoxColor			= ImColor(0, 182, 255, 255);
		inline ImColor		BotBoxFillColor		= ImColor(0, 0, 0, 120);
		inline ImColor		BotBoneColor		= ImColor(0, 182, 255, 255);
		inline ImColor		BotTracesColor		= ImColor(0, 182, 255, 255);
		inline ImColor		BotDistanceColor	= ImColor(0, 182, 255, 255);

	}
	namespace Misc {
		inline bool			TriggerBot			= true;
		inline bool			OnlyWhenAimbot		= true;
		inline ImColor		MenuColor			= ImColor(249, 255, 20, 255);

		inline int FPSLimit = 240;
		inline bool showFPS = true;

		inline const char* tabs[] = {"misc", "close-range"};
		inline int selectedTabIndex = 0;
	}

	void SaveConfig();
	void LoadConfig();
	void DefaultConfig();
}