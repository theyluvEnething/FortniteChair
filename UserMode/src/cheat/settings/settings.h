#pragma once
#include "../../render/render.h"


namespace Settings
{
	inline bool ShowMenu = true;
	inline int Tab = 0;
	inline bool Initialized;
	inline LPCSTR ConfigPath = "";
	namespace Aimbot
	{
		inline bool			Enabled				= true;
		inline bool			ShowFov				= true;
		inline bool			FillFovCircle		= true;
		inline float		Fov = 150;
		inline ImColor		FovColor			= ImColor(255, 20, 20, 255);
		inline float		Smooth				= 10;
		inline const char*	Aimkey[]			= { "Left Mouse Button", "Right Mouse Button" };
		inline int			CurrentAimkey		= 1;
		inline int			CurrentKey			= VK_RBUTTON;
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
		inline ImColor		TracesColor			= ImColor(255, 0, 0, 255);

		inline const char*	TracesOptions[]		= { "Bottom", "Center", "Top"};
		inline int			TracesHeight		= 0;
		inline int			CurrentTracesOption	= 1;
		
		inline ImColor		TeamBoxColor		= ImColor(255, 255, 255, 255);
		inline ImColor		TeamBoxFillColor	= ImColor(0, 0, 0, 120);
		inline ImColor		TeamBoneColor		= ImColor(255, 255, 255, 255);
		inline ImColor		TeamTracesColor		= ImColor(255, 255, 255, 255);
	}
	namespace Misc {
		inline bool			TriggerBot			= false;
	}

	void SaveConfig();
	void LoadConfig();
	void DefaultConfig();
}