#include "settings.h"
#include <iostream>


BOOL WritePrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, int nInteger, LPCSTR lpFileName) {
	char lpString[1024];
	sprintf_s(lpString, "%d", nInteger);
	return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
}

BOOL WritePrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, float nInteger, LPCSTR lpFileName) {
	char lpString[1024];
	sprintf_s(lpString, "%f", nInteger);
	return WritePrivateProfileStringA(lpAppName, lpKeyName, lpString, lpFileName);
}

BOOL WritePrivateProfileImColor(LPCSTR lpAppName, LPCSTR lpKeyName, ImColor color, LPCSTR lpFileName) {
	return WritePrivateProfileFloat(lpAppName, StringAdd(lpKeyName, ".X"), color.Value.x, lpFileName) &
		   WritePrivateProfileFloat(lpAppName, StringAdd(lpKeyName, ".Y"), color.Value.y, lpFileName) &
		   WritePrivateProfileFloat(lpAppName, StringAdd(lpKeyName, ".Z"), color.Value.z, lpFileName) &
		   WritePrivateProfileFloat(lpAppName, StringAdd(lpKeyName, ".W"), color.Value.w, lpFileName);
}

float GetPrivateProfileFloat(LPCSTR lpAppName, LPCSTR lpKeyName, FLOAT flDefault, LPCSTR lpFileName)
{
	char szData[32];
	GetPrivateProfileStringA(lpAppName, lpKeyName, std::to_string(flDefault).c_str(), szData, 32, lpFileName);
	return (float)atof(szData);
}

void Settings::DefaultConfig() {
	Aimbot::Enabled = true;
	Aimbot::ShowFov = true;
	Aimbot::FillFovCircle = true;
	Aimbot::Fov = 150;
	Aimbot::FovColor = ImColor(255, 20, 20, 255);
	Aimbot::SmoothX = 10;
	Aimbot::SmoothY = 10;
	Aimbot::CurrentAimkey = 1;

	Visuals::Enabled = true;
	Visuals::Box = true;
	Visuals::FillBox = true;
	Visuals::Traces = false;
	Visuals::Distance = true;
	Visuals::Bone = true;
	Visuals::TraceLineThickness = 1.3f;
	Visuals::BoxLineThickness = 1;
	Visuals::BoneLineThickness = 2.1;
	Visuals::BoxColor = ImColor(255, 0, 0, 255);
	Visuals::BoxFillColor = ImColor(0, 0, 0, 120);
	Visuals::BoneColor = ImColor(255, 0, 0, 255);
	Visuals::TracesColor = ImColor(255, 0, 0, 255);
	
	Visuals::TracesHeight = 0;
	Visuals::CurrentTracesOption = 1;

	Visuals::TeamBoxColor = ImColor(255, 255, 255, 255);
	Visuals::TeamBoxFillColor = ImColor(0, 0, 0, 120);
	Visuals::TeamBoneColor = ImColor(255, 255, 255, 255);
	Visuals::TeamTracesColor = ImColor(255, 255, 255, 255);

	Misc::TriggerBot = false;
	Misc::OnlyWhenAimbot = true;
}

void Settings::SaveConfig()
{
	std::ofstream file(ConfigPath, std::ofstream::out | std::ofstream::trunc);
	WritePrivateProfileInt		(("chair"),	"Initialized",						Initialized,						ConfigPath);
	WritePrivateProfileInt		(("chair"), "Enabled",							Aimbot::Enabled,					ConfigPath);
	WritePrivateProfileInt		(("chair"), "Show Fov",						Aimbot::ShowFov,					    ConfigPath);
	WritePrivateProfileInt		(("chair"), "Fill Fov Circle",					Aimbot::ShowFov,					ConfigPath);
	WritePrivateProfileFloat	(("chair"), "Fov",								Aimbot::Fov,						ConfigPath);
	WritePrivateProfileFloat	(("chair"), "SmoothX",							Aimbot::SmoothX,						ConfigPath);
	WritePrivateProfileFloat    (("chair"), "SmoothY",                          Aimbot::SmoothY,                     ConfigPath);
	WritePrivateProfileImColor	(("chair"), "FovColor",						Aimbot::FovColor,						ConfigPath);
	WritePrivateProfileInt		(("chair"), "Aimkey",							Aimbot::CurrentAimkey,				ConfigPath);
								   
	WritePrivateProfileInt	    (("chair"), "Enabled",							Visuals::Enabled,					ConfigPath);
	WritePrivateProfileInt	    (("chair"), "Box",								Visuals::Box,						ConfigPath);
	WritePrivateProfileInt	    (("chair"), "FillBox",							Visuals::FillBox,					ConfigPath);
	WritePrivateProfileInt		(("chair"), "Traces",							Visuals::Traces,					ConfigPath);
	WritePrivateProfileInt		(("chair"), "Distance",						Visuals::Distance,						ConfigPath);
	WritePrivateProfileInt		(("chair"), "Bone",							Visuals::Bone,							ConfigPath);
	WritePrivateProfileInt		(("chair"), "CurrentTracesOption",				Visuals::CurrentTracesOption,		ConfigPath);
	WritePrivateProfileFloat	(("chair"), "BoxLineThickness",				Visuals::BoxLineThickness,				ConfigPath);
	WritePrivateProfileFloat	(("chair"), "TraceLineThickness",				Visuals::TraceLineThickness,		ConfigPath);
	WritePrivateProfileFloat	(("chair"), "BoneLineThickness",				Visuals::BoneLineThickness,			ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BoxColor",						Visuals::BoxColor,						ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BoxFillColor",					Visuals::BoxFillColor,					ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BoneColor",						Visuals::BoneColor,					ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TracesColor",						Visuals::TracesColor,				ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TeamBoxColor",					Visuals::TeamBoxColor,					ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TeamBoxFillColor",				Visuals::TeamBoxFillColor,				ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TeamBoneColor",					Visuals::TeamBoneColor,				ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TeamTracesColor",					Visuals::TeamTracesColor,			ConfigPath);

	WritePrivateProfileInt	    (("chair"),	 "TriggerBot",						Misc::TriggerBot,					ConfigPath);
	WritePrivateProfileInt	    (("chair"),	 "OnlyWhenAimbot",					Misc::OnlyWhenAimbot,				ConfigPath);
}

void Settings::LoadConfig()
{
	if (!GetPrivateProfileIntA(("chair"), "Initialized", Initialized, ConfigPath))
		return;

	Aimbot::Enabled =					GetPrivateProfileIntA		(("chair"), "Enabled",							Aimbot::Enabled,					ConfigPath);
	Aimbot::ShowFov =					GetPrivateProfileIntA		(("chair"), "Show Fov",							Aimbot::ShowFov,					ConfigPath);
	Aimbot::FillFovCircle =				GetPrivateProfileIntA		(("chair"), "Fill Fov Circle",					Aimbot::ShowFov,					ConfigPath);
	Aimbot::Fov		=					GetPrivateProfileFloat		(("chair"), "Fov",								Aimbot::Fov,						ConfigPath);
	Aimbot::SmoothX =					GetPrivateProfileFloat		(("chair"), "SmoothX",							Aimbot::SmoothX,					ConfigPath);
	Aimbot::SmoothY =                   GetPrivateProfileFloat      (("chair"), "SmoothY",                          Aimbot::SmoothY,	                ConfigPath);
	Aimbot::CurrentAimkey =				GetPrivateProfileIntA		(("chair"), "Aimkey",							Aimbot::CurrentAimkey,				ConfigPath);

	Aimbot::FovColor.Value.x = GetPrivateProfileFloat(("chair"), "FovColor.X", Aimbot::FovColor.Value.x, ConfigPath);
	Aimbot::FovColor.Value.y = GetPrivateProfileFloat(("chair"), "FovColor.Y", Aimbot::FovColor.Value.y, ConfigPath);
	Aimbot::FovColor.Value.z = GetPrivateProfileFloat(("chair"), "FovColor.Z", Aimbot::FovColor.Value.z, ConfigPath);
	Aimbot::FovColor.Value.w = GetPrivateProfileFloat(("chair"), "FovColor.W", Aimbot::FovColor.Value.w, ConfigPath);



	Visuals::Enabled =					GetPrivateProfileIntA	    (("chair"), "Enabled",							Visuals::Enabled,					ConfigPath);
	Visuals::Box =						GetPrivateProfileIntA	    (("chair"), "Box",								Visuals::Box,						ConfigPath);
	Visuals::FillBox =					GetPrivateProfileIntA	    (("chair"), "FillBox",							Visuals::FillBox,					ConfigPath);
	Visuals::Traces =					GetPrivateProfileIntA		(("chair"), "Traces",							Visuals::Traces,					ConfigPath);
	Visuals::Distance =					GetPrivateProfileIntA		(("chair"), "Distance",							Visuals::Distance,					ConfigPath);
	Visuals::Bone =						GetPrivateProfileIntA		(("chair"), "Bone",								Visuals::Bone,						ConfigPath);
	Visuals::CurrentTracesOption =		GetPrivateProfileIntA		(("chair"), "CurrentTracesOption",				Visuals::CurrentTracesOption,		ConfigPath);
	Visuals::BoxLineThickness =			GetPrivateProfileFloat		(("chair"), "BoxLineThickness",					Visuals::BoxLineThickness,			ConfigPath);
	Visuals::TraceLineThickness =		GetPrivateProfileFloat		(("chair"), "TraceLineThickness",				Visuals::TraceLineThickness,		ConfigPath);
	Visuals::BoneLineThickness =		GetPrivateProfileFloat		(("chair"), "BoneLineThickness",				Visuals::BoneLineThickness,			ConfigPath);

	Visuals::BoxColor.Value.x = GetPrivateProfileFloat(("chair"), "BoxColor.X", Visuals::BoxColor.Value.x, ConfigPath);
	Visuals::BoxColor.Value.y = GetPrivateProfileFloat(("chair"), "BoxColor.Y", Visuals::BoxColor.Value.y, ConfigPath);
	Visuals::BoxColor.Value.z = GetPrivateProfileFloat(("chair"), "BoxColor.Z", Visuals::BoxColor.Value.z, ConfigPath);
	Visuals::BoxColor.Value.w = GetPrivateProfileFloat(("chair"), "BoxColor.W", Visuals::BoxColor.Value.w, ConfigPath);

	Visuals::BoxFillColor.Value.x = GetPrivateProfileFloat(("chair"), "BoxFillColor.X", Visuals::BoxFillColor.Value.x, ConfigPath);
	Visuals::BoxFillColor.Value.y = GetPrivateProfileFloat(("chair"), "BoxFillColor.Y", Visuals::BoxFillColor.Value.y, ConfigPath);
	Visuals::BoxFillColor.Value.z = GetPrivateProfileFloat(("chair"), "BoxFillColor.Z", Visuals::BoxFillColor.Value.z, ConfigPath);
	Visuals::BoxFillColor.Value.w = GetPrivateProfileFloat(("chair"), "BoxFillColor.W", Visuals::BoxFillColor.Value.w, ConfigPath);

	Visuals::BoneColor.Value.x = GetPrivateProfileFloat(("chair"), "BoneColor.X", Visuals::BoneColor.Value.x, ConfigPath);
	Visuals::BoneColor.Value.y = GetPrivateProfileFloat(("chair"), "BoneColor.Y", Visuals::BoneColor.Value.y, ConfigPath);
	Visuals::BoneColor.Value.z = GetPrivateProfileFloat(("chair"), "BoneColor.Z", Visuals::BoneColor.Value.z, ConfigPath);
	Visuals::BoneColor.Value.w = GetPrivateProfileFloat(("chair"), "BoneColor.W", Visuals::BoneColor.Value.w, ConfigPath);

	Visuals::TracesColor.Value.x = GetPrivateProfileFloat(("chair"), "TracesColor.X", Visuals::TracesColor.Value.x, ConfigPath);
	Visuals::TracesColor.Value.y = GetPrivateProfileFloat(("chair"), "TracesColor.Y", Visuals::TracesColor.Value.y, ConfigPath);
	Visuals::TracesColor.Value.z = GetPrivateProfileFloat(("chair"), "TracesColor.Z", Visuals::TracesColor.Value.z, ConfigPath);
	Visuals::TracesColor.Value.w = GetPrivateProfileFloat(("chair"), "TracesColor.W", Visuals::TracesColor.Value.w, ConfigPath);
	
	Visuals::TeamBoxColor.Value.x = GetPrivateProfileFloat(("chair"), "TeamBoxColor.X", Visuals::TeamBoxColor.Value.x, ConfigPath);
	Visuals::TeamBoxColor.Value.y = GetPrivateProfileFloat(("chair"), "TeamBoxColor.Y", Visuals::TeamBoxColor.Value.y, ConfigPath);
	Visuals::TeamBoxColor.Value.z = GetPrivateProfileFloat(("chair"), "TeamBoxColor.Z", Visuals::TeamBoxColor.Value.z, ConfigPath);
	Visuals::TeamBoxColor.Value.w = GetPrivateProfileFloat(("chair"), "TeamBoxColor.W", Visuals::TeamBoxColor.Value.w, ConfigPath);

	Visuals::TeamBoxFillColor.Value.x = GetPrivateProfileFloat(("chair"), "TeamBoxFillColor.X", Visuals::TeamBoxFillColor.Value.x, ConfigPath);
	Visuals::TeamBoxFillColor.Value.y = GetPrivateProfileFloat(("chair"), "TeamBoxFillColor.Y", Visuals::TeamBoxFillColor.Value.y, ConfigPath);
	Visuals::TeamBoxFillColor.Value.z = GetPrivateProfileFloat(("chair"), "TeamBoxFillColor.Z", Visuals::TeamBoxFillColor.Value.z, ConfigPath);
	Visuals::TeamBoxFillColor.Value.w = GetPrivateProfileFloat(("chair"), "TeamBoxFillColor.W", Visuals::TeamBoxFillColor.Value.w, ConfigPath);

	Visuals::TeamBoneColor.Value.x = GetPrivateProfileFloat(("chair"), "TeamBoneColor.X", Visuals::TeamBoneColor.Value.x, ConfigPath);
	Visuals::TeamBoneColor.Value.y = GetPrivateProfileFloat(("chair"), "TeamBoneColor.Y", Visuals::TeamBoneColor.Value.y, ConfigPath);
	Visuals::TeamBoneColor.Value.z = GetPrivateProfileFloat(("chair"), "TeamBoneColor.Z", Visuals::TeamBoneColor.Value.z, ConfigPath);
	Visuals::TeamBoneColor.Value.w = GetPrivateProfileFloat(("chair"), "TeamBoneColor.W", Visuals::TeamBoneColor.Value.w, ConfigPath);

	Visuals::TeamTracesColor.Value.x = GetPrivateProfileFloat(("chair"), "TeamTracesColor.X", Visuals::TeamTracesColor.Value.x, ConfigPath);
	Visuals::TeamTracesColor.Value.y = GetPrivateProfileFloat(("chair"), "TeamTracesColor.Y", Visuals::TeamTracesColor.Value.y, ConfigPath);
	Visuals::TeamTracesColor.Value.z = GetPrivateProfileFloat(("chair"), "TeamTracesColor.Z", Visuals::TeamTracesColor.Value.z, ConfigPath);
	Visuals::TeamTracesColor.Value.w = GetPrivateProfileFloat(("chair"), "TeamTracesColor.W", Visuals::TeamTracesColor.Value.w, ConfigPath);



	Misc::TriggerBot =					GetPrivateProfileIntA	    (("chair"),	 "TriggerBot",						Misc::TriggerBot,					ConfigPath);
	Misc::OnlyWhenAimbot =				GetPrivateProfileIntA		(("chair"),	 "OnlyWhenAimbot",					Misc::OnlyWhenAimbot,				ConfigPath);

}

