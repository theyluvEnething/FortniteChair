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

		inline bool showFov = false;

		inline float distance = 30;

		inline bool SmartFov = false;
		inline float minFov = 150;
		inline float maxFov = 150;
		inline int scaleFov = 5;

		inline bool SmartSmoothness = false;
		inline float minSmooth = 10;
		inline float maxSmooth = 10;
		inline int scaleSmooth = 5;

		inline bool			TriggerBot = false;
		inline bool			Bone = true;
		inline bool			Box = true;
		inline bool			Traces = true;
		inline bool			Distance = true;

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
		inline float		Fov = 150;
		inline ImColor		FovColor			= ImColor(255, 20, 20, 255);
		inline float		SmoothX				= 10;
		inline float		SmoothY             = 10;
		inline bool			LockSmooth			= true;
		inline const char*	Aimkey[]			= { "Left Mouse Button", "Right Mouse Button" };
		inline int			CurrentAimkey		= 1;
		inline int			CurrentKey			= VK_RBUTTON;
	
		inline const char*  TargetPart[]		= { "Head", "Neck", "Hip"};
		inline int			CurrentTargetPart = 0;
	}
	namespace Visuals
	{
		inline bool			Enabled				= true;
		inline bool			Box					= true;
		inline bool			FillBox				= true;
		inline bool			Traces				= false;
		inline bool			Distance			= true;
		inline bool			Bone				= true;
		inline float		TraceLineThickness	= 1.3f;
		inline float		BoxLineThickness	= 1;
		inline float		BoneLineThickness	= 2.1;
		inline ImColor		BoxColor			= ImColor(255, 0, 0, 255);
		inline ImColor		BoxFillColor		= ImColor(0, 0, 0, 120);
		inline ImColor		BoneColor			= ImColor(255, 0, 0, 255);
		inline float		BoneDisplayRange	= 25.0f;
		inline bool			BoneOnTeam			= false;

		
		inline ImColor		TracesColor			= ImColor(255, 0, 0, 255);
		inline const char*	TracesOptions[]		= { "Bottom", "Center", "Top"};
		inline int			TracesHeight		= 0;
		inline int			CurrentTracesOption	= 1;
		
		inline ImColor		TeamBoxColor		= ImColor(255, 255, 255, 255);
		inline ImColor		TeamBoxFillColor	= ImColor(0, 0, 0, 120);
		inline ImColor		TeamBoneColor		= ImColor(255, 255, 255, 255);
		inline ImColor		TeamTracesColor		= ImColor(255, 255, 255, 255);

		inline ImColor		BotBoxColor			= ImColor(255, 255, 255, 255);
		inline ImColor		BotBoxFillColor		= ImColor(0, 0, 0, 120);
		inline ImColor		BotBoneColor		= ImColor(255, 255, 255, 255);
		inline ImColor		BotTracesColor		= ImColor(255, 255, 255, 255);
	}
	namespace Misc {
		inline bool			TriggerBot			= false;
		inline bool			OnlyWhenAimbot		= false;
		inline ImColor		MenuColor = ImColor(255, 255, 0, 255);

		inline const char* tabs[] = {"Misc", "Close-Range"};
		inline int selectedTabIndex = 0;

		inline bool onlyShowActiveFOV = false; // so whenever close range is active close range fov is shown, otherwise standard fov 8===D O:

	}

	void SaveConfig();
	void LoadConfig();
	void DefaultConfig();
}