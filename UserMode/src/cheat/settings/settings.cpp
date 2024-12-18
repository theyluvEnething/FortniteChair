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
	CloseRange::MaxFov = 280;
	CloseRange::CurrentFov = 15;

	Aimbot::Enabled = true;
	Aimbot::ShowFov = true;
	Aimbot::FillFovCircle = false;
	Aimbot::Fov = 140;
	Aimbot::FovColor = ImColor(249, 255, 20, 255);
	Aimbot::LockSmooth = true;
	Aimbot::SmoothX = 5;
	Aimbot::SmoothY = 5;
	Aimbot::CurrentAimkey = 1;
	Aimbot::CurrentTargetPart = 0;
	Aimbot::Predict = false;

	Visuals::Enabled = true;
	Visuals::TeamEnabled = false;
	Visuals::BotEnabled = true;
	Visuals::Box = true;
	Visuals::FillBox = true;
	Visuals::Traces = false;
	Visuals::Distance = true;
	Visuals::Bone = true;
	Visuals::BoneOnSelf = false;
	
	Visuals::BoxOnTeam = true;
	Visuals::FillBoxOnTeam = true;
	Visuals::TracesOnTeam = false;
	Visuals::DistanceOnTeam = true;
	Visuals::BoneOnTeam = false;

	Visuals::BoxOnBot = true;
	Visuals::FillBoxOnBot = true;
	Visuals::TracesOnBot = false;
	Visuals::DistanceOnBot = true;
	Visuals::BoneOnBot = false;

	Visuals::LockColors = true;
	Visuals::LockColorsTeam = true;
	Visuals::LockColorsBot = true;

	Visuals::TraceLineThickness = 1.3f;
	Visuals::BoxLineThickness = 1;
	Visuals::BoneLineThickness = 2.1;
	Visuals::BoxColor = ImColor(255, 0, 0, 255);
	Visuals::BoneColor = ImColor(255, 0, 0, 255);
	Visuals::TracesColor = ImColor(255, 0, 0, 255);
	Visuals::BoxFillColor = ImColor(255, 255, 255, 120);
	Visuals::DistanceColor = ImColor(255, 255, 255, 120);
	Visuals::TextSize = 1.0f;
	Visuals::BoneDisplayRange = 25.0f;

	Visuals::TracesHeight = 0;
	Visuals::CurrentTracesOption = 1;

	Visuals::TeamBoxColor = ImColor(42, 255, 0, 255);
	Visuals::TeamBoxFillColor = ImColor(255, 255, 255, 120);
	Visuals::TeamBoneColor = ImColor(42, 255, 0, 255);
	Visuals::TeamTracesColor = ImColor(42, 255, 0, 255);
	Visuals::TeamDistanceColor = ImColor(42, 255, 0, 255);

	Visuals::BotBoxColor = ImColor(0, 182, 255, 255);
	Visuals::BotBoxFillColor = ImColor(255, 255, 255, 120);
	Visuals::BotBoneColor = ImColor(0, 182, 255, 255);
	Visuals::BotTracesColor = ImColor(0, 182, 255, 255);
	Visuals::BotDistanceColor = ImColor(0, 182, 255, 255);

	Misc::TriggerBot = true;
	Misc::OnlyWhenAimbot = true;
	Misc::MenuColor = ImColor(249, 255, 20, 255);;

	CloseRange::Enabled = true;
	CloseRange::distance = 15;
	CloseRange::DynamicFov = true;
	CloseRange::MaxFov = 265;
	CloseRange::CurrentFov = 145;
	CloseRange::TriggerBot = true;

	CloseRange::LockColors = true;
	CloseRange::BoxColor = ImColor(249, 255, 20, 255);
	CloseRange::BoneColor = ImColor(249, 255, 20, 255);
	CloseRange::TracesColor = ImColor(255, 255, 255, 120);

	CloseRange::SmoothX = 2.5f;
	CloseRange::SmoothY = 2.5f;
	CloseRange::LockSmooth = true;
	CloseRange::SmartSmooth = false;
	CloseRange::InstantInterpolation = false;
	CloseRange::InterpolateColor = true;
}

void Settings::SaveConfig()
{
	std::ofstream file(ConfigPath, std::ofstream::out | std::ofstream::trunc);
	WritePrivateProfileInt(("chair"), "showWatermark", Visuals::showWatermark, ConfigPath);



	WritePrivateProfileInt(("chair"), "lootEsp", Settings::Visuals::lootEsp, ConfigPath);
	WritePrivateProfileInt(("chair"), "vehicleEsp", Settings::Visuals::vehicleEsp, ConfigPath);
	WritePrivateProfileInt(("chair"), "legendary", Settings::Visuals::legendary, ConfigPath);
	WritePrivateProfileInt(("chair"), "epic", Settings::Visuals::epic, ConfigPath);
	WritePrivateProfileInt(("chair"), "rare", Settings::Visuals::rare, ConfigPath);
	WritePrivateProfileInt(("chair"), "uncommon", Settings::Visuals::uncommon, ConfigPath);
	WritePrivateProfileInt(("chair"), "common", Settings::Visuals::common, ConfigPath);


	WritePrivateProfileInt		(("chair"),	"Initialized",						Initialized,						ConfigPath);
	WritePrivateProfileInt		(("chair"), "Enabled",							Aimbot::Enabled,					ConfigPath);
	WritePrivateProfileInt		(("chair"), "Show Fov",						Aimbot::ShowFov,					    ConfigPath);
	WritePrivateProfileInt		(("chair"), "Fill Fov Circle",					Aimbot::FillFovCircle, ConfigPath);
	WritePrivateProfileFloat	(("chair"), "Fov",								Aimbot::Fov,						ConfigPath);
	WritePrivateProfileInt		(("chair"), "Predict",						Aimbot::Predict,					ConfigPath);
	WritePrivateProfileInt		(("chair"), "LockSmooth",						Aimbot::LockSmooth,					ConfigPath);
	WritePrivateProfileFloat	(("chair"), "SmoothX",							Aimbot::SmoothX,						ConfigPath);
	WritePrivateProfileFloat    (("chair"), "SmoothY",                          Aimbot::SmoothY,                     ConfigPath);
	WritePrivateProfileImColor	(("chair"), "FovColor",						Aimbot::FovColor,						ConfigPath);
	WritePrivateProfileInt		(("chair"), "Aimkey",							Aimbot::CurrentAimkey,				ConfigPath);
	WritePrivateProfileInt		(("chair"), "TargetPart",						Aimbot::CurrentTargetPart,			ConfigPath);
								   
	WritePrivateProfileInt	    (("chair"), "Enabled",							Visuals::Enabled,					ConfigPath);
	WritePrivateProfileInt	    (("chair"), "TeamEnabled",							Visuals::TeamEnabled,					ConfigPath);
	WritePrivateProfileInt	    (("chair"), "BotEnabled",							Visuals::BotEnabled,					ConfigPath);
	WritePrivateProfileInt	    (("chair"), "LockVisualColorsTeam",					Visuals::LockColorsTeam,		ConfigPath);
	WritePrivateProfileInt	    (("chair"), "LockVisualColorsBot",					Visuals::LockColorsBot,			ConfigPath);
	WritePrivateProfileInt	    (("chair"), "LockVisualColors",					Visuals::LockColors,				ConfigPath);

	WritePrivateProfileInt	    (("chair"), "Box",								Visuals::Box,						ConfigPath);
	WritePrivateProfileInt	    (("chair"), "FillBox",							Visuals::FillBox,					ConfigPath);
	WritePrivateProfileInt		(("chair"), "Traces",							Visuals::Traces,					ConfigPath);
	WritePrivateProfileInt		(("chair"), "Distance",						Visuals::Distance,						ConfigPath);
	WritePrivateProfileInt		(("chair"), "Bone",							Visuals::Bone,							ConfigPath);
	WritePrivateProfileInt		(("chair"), "BoneOnSelf",							Visuals::BoneOnSelf,							ConfigPath);

	WritePrivateProfileInt	    (("chair"), "TeamBox",							Visuals::BoxOnTeam,					ConfigPath);
	WritePrivateProfileInt	    (("chair"), "TeamFillBox",						Visuals::FillBoxOnTeam,				ConfigPath);
	WritePrivateProfileInt		(("chair"), "TeamTraces",						Visuals::TracesOnTeam,				ConfigPath);
	WritePrivateProfileInt		(("chair"), "TeamDistance",						Visuals::DistanceOnTeam,			ConfigPath);
	WritePrivateProfileInt		(("chair"), "TeamBone",							Visuals::BoneOnTeam,				ConfigPath);

	WritePrivateProfileInt	    (("chair"), "BotBox",							Visuals::BoxOnBot,					ConfigPath);
	WritePrivateProfileInt	    (("chair"), "BotFillBox",						Visuals::FillBoxOnBot,				ConfigPath);
	WritePrivateProfileInt		(("chair"), "BotTraces",						Visuals::TracesOnBot,				ConfigPath);
	WritePrivateProfileInt		(("chair"), "BotDistance",						Visuals::DistanceOnBot,			ConfigPath);
	WritePrivateProfileInt		(("chair"), "BotBone",							Visuals::BoneOnBot,				ConfigPath);


	WritePrivateProfileInt		(("chair"), "CurrentTracesOption",				Visuals::CurrentTracesOption,		ConfigPath);
	WritePrivateProfileFloat	(("chair"), "BoxLineThickness",				Visuals::BoxLineThickness,				ConfigPath);
	WritePrivateProfileFloat	(("chair"), "TraceLineThickness",				Visuals::TraceLineThickness,		ConfigPath);
	WritePrivateProfileFloat	(("chair"), "BoneLineThickness",				Visuals::BoneLineThickness,			ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BoxColor",						Visuals::BoxColor,						ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BoxFillColor",					Visuals::BoxFillColor,					ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BoneColor",						Visuals::BoneColor,					ConfigPath);
	WritePrivateProfileFloat	(("chair"), "BoneDisplayRange",					Visuals::BoneDisplayRange,			ConfigPath);
	WritePrivateProfileImColor  (("chair"), "TracesColor",					 Visuals::TracesColor, ConfigPath);
	WritePrivateProfileImColor	(("chair"), "DistanceColor",					Visuals::DistanceColor,				ConfigPath);

	WritePrivateProfileImColor	(("chair"), "TeamBoxColor",					Visuals::TeamBoxColor,					ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TeamBoxFillColor",				Visuals::TeamBoxFillColor,				ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TeamBoneColor",					Visuals::TeamBoneColor,				ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TeamTracesColor",					Visuals::TeamTracesColor,			ConfigPath);
	WritePrivateProfileImColor	(("chair"), "TeamDistanceColor",					Visuals::TeamDistanceColor,			ConfigPath);

	WritePrivateProfileImColor	(("chair"), "BotBoxColor",						Visuals::BotBoxColor,				ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BotBoxFillColor",					Visuals::BotBoxFillColor,			ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BotBoneColor",						Visuals::BotBoneColor,				ConfigPath);
	WritePrivateProfileImColor	(("chair"), "BotDistanceColor",					Visuals::BotDistanceColor,			ConfigPath);

	WritePrivateProfileInt	    (("chair"),	 "TriggerBot",						Misc::TriggerBot,					ConfigPath);
	WritePrivateProfileInt	    (("chair"),	 "OnlyWhenAimbot",					Misc::OnlyWhenAimbot,				ConfigPath);
	WritePrivateProfileImColor  (("chair"),	"MenuColor",						Misc::MenuColor,					ConfigPath);

	WritePrivateProfileInt(("chair"), "CloseRangeEnabled", CloseRange::Enabled, ConfigPath);
	WritePrivateProfileInt(("chair"), "LockColors", CloseRange::LockColors, ConfigPath);

	WritePrivateProfileInt(("chair"), "CloseRangeTriggerbotBool", CloseRange::TriggerBot, ConfigPath);
	WritePrivateProfileInt(("chair"), "DynamicFov", CloseRange::DynamicFov, ConfigPath);

	WritePrivateProfileFloat(("chair"), "CloseRangeDistance", CloseRange::distance, ConfigPath);
	WritePrivateProfileFloat(("chair"), "CloseRangeFov", CloseRange::MaxFov, ConfigPath);

	WritePrivateProfileFloat(("chair"), "CloseRangeSmoothX", CloseRange::SmoothX, ConfigPath);
	WritePrivateProfileFloat(("chair"), "CloseRangeSmoothY", CloseRange::SmoothY, ConfigPath);

	WritePrivateProfileFloat(("chair"), "CloseRangeCurrentFov", CloseRange::CurrentFov, ConfigPath);
	

	WritePrivateProfileInt(("chair"), "CloseRangeLockSmooth", CloseRange::LockSmooth, ConfigPath);
	WritePrivateProfileInt(("chair"), "SmartSmooth", CloseRange::SmartSmooth, ConfigPath);
	WritePrivateProfileInt(("chair"), "InstantInterpolation", CloseRange::InstantInterpolation, ConfigPath);
	WritePrivateProfileInt(("chair"), "InterpolateColor", CloseRange::InterpolateColor, ConfigPath);


	WritePrivateProfileImColor(("chair"), "CloseRangeBoxColor", CloseRange::BoxColor, ConfigPath);
	WritePrivateProfileImColor(("chair"), "CloseRangeTracesColor", CloseRange::TracesColor, ConfigPath);
	WritePrivateProfileImColor(("chair"), "CloseRangeBoneColor", CloseRange::BoneColor, ConfigPath);

	WritePrivateProfileFloat(("chair"), "CloseRangeLineThickness", CloseRange::lineThickness, ConfigPath);


	WritePrivateProfileInt(("chair"), "showFPS", Misc::showFPS, ConfigPath);
	WritePrivateProfileInt(("chair"), "FPSLimit", Misc::FPSLimit, ConfigPath);


}

void Settings::LoadConfig()
{
	if (!GetPrivateProfileIntA(("chair"), "Initialized", Initialized, ConfigPath))
		return;

	Visuals::showWatermark = GetPrivateProfileIntA(("chair"), "showWatermark", Visuals::showWatermark, ConfigPath);
	Aimbot::Enabled =					GetPrivateProfileIntA		(("chair"), "Enabled",							Aimbot::Enabled,					ConfigPath);
	Aimbot::ShowFov =					GetPrivateProfileIntA		(("chair"), "Show Fov",							Aimbot::ShowFov,					ConfigPath);
	Aimbot::Predict =					GetPrivateProfileIntA		(("chair"), "Predict",							Aimbot::Predict,					ConfigPath);

	Aimbot::FillFovCircle =				GetPrivateProfileIntA		(("chair"), "Fill Fov Circle",					Aimbot::FillFovCircle,				ConfigPath);
	Aimbot::Fov		=					GetPrivateProfileFloat		(("chair"), "Fov",								Aimbot::Fov,						ConfigPath);
	Aimbot::SmoothX =					GetPrivateProfileFloat		(("chair"), "SmoothX",							Aimbot::SmoothX,					ConfigPath);
	Aimbot::SmoothY =                   GetPrivateProfileFloat      (("chair"), "SmoothY",                          Aimbot::SmoothY,	                ConfigPath);
	Aimbot::LockSmooth =				GetPrivateProfileIntA		(("chair"), "LockSmooth",						Aimbot::LockSmooth,					ConfigPath);

	Aimbot::CurrentAimkey =				GetPrivateProfileIntA		(("chair"), "Aimkey",							Aimbot::CurrentAimkey,				ConfigPath);
	Aimbot::CurrentTargetPart =			GetPrivateProfileIntA		(("chair"), "TargetPart",					Aimbot::CurrentTargetPart,			ConfigPath);

	Aimbot::FovColor.Value.x = GetPrivateProfileFloat(("chair"), "FovColor.X", Aimbot::FovColor.Value.x, ConfigPath);
	Aimbot::FovColor.Value.y = GetPrivateProfileFloat(("chair"), "FovColor.Y", Aimbot::FovColor.Value.y, ConfigPath);
	Aimbot::FovColor.Value.z = GetPrivateProfileFloat(("chair"), "FovColor.Z", Aimbot::FovColor.Value.z, ConfigPath);
	Aimbot::FovColor.Value.w = GetPrivateProfileFloat(("chair"), "FovColor.W", Aimbot::FovColor.Value.w, ConfigPath);



	Visuals::Enabled =					GetPrivateProfileIntA	    (("chair"), "Enabled",							Visuals::Enabled,					ConfigPath);
	Visuals::TeamEnabled =					GetPrivateProfileIntA	    (("chair"), "TeamEnabled",							Visuals::TeamEnabled,					ConfigPath);
	Visuals::BotEnabled =					GetPrivateProfileIntA	    (("chair"), "BotEnabled",							Visuals::BotEnabled,					ConfigPath);
	Visuals::LockColors	=				GetPrivateProfileIntA		(("chair"), "LockVisualColors",					Visuals::LockColors,				ConfigPath);
	Visuals::LockColorsTeam =			GetPrivateProfileIntA		(("chair"), "LockVisualColorsTeam",				Visuals::LockColorsTeam,			ConfigPath);
	Visuals::LockColorsBot =			GetPrivateProfileIntA		(("chair"), "LockVisualColorsBot",				Visuals::LockColorsBot,				ConfigPath);

	Visuals::Box =						GetPrivateProfileIntA	    (("chair"), "Box",								Visuals::Box,						ConfigPath);
	Visuals::FillBox =					GetPrivateProfileIntA	    (("chair"), "FillBox",							Visuals::FillBox,					ConfigPath);
	Visuals::Traces =					GetPrivateProfileIntA		(("chair"), "Traces",							Visuals::Traces,					ConfigPath);
	Visuals::Distance =					GetPrivateProfileIntA		(("chair"), "Distance",							Visuals::Distance,					ConfigPath);
	Visuals::Bone =						GetPrivateProfileIntA		(("chair"), "BoneOnSelf",								Visuals::BoneOnSelf,						ConfigPath);
	Visuals::Bone =						GetPrivateProfileIntA		(("chair"), "Bone",								Visuals::Bone,						ConfigPath);
	
	Visuals::BoxOnTeam =				GetPrivateProfileIntA(("chair"), "TeamBox",									Visuals::BoxOnTeam,				ConfigPath);
	Visuals::FillBoxOnTeam =			GetPrivateProfileIntA(("chair"), "TeamFillBox",								Visuals::FillBoxOnTeam,			ConfigPath);
	Visuals::TracesOnTeam =				GetPrivateProfileIntA(("chair"), "TeamTraces",								Visuals::TracesOnTeam,			ConfigPath);
	Visuals::DistanceOnTeam =			GetPrivateProfileIntA(("chair"), "TeamDistance",							Visuals::DistanceOnTeam,			ConfigPath);
	Visuals::BoneOnTeam =				GetPrivateProfileIntA(("chair"), "TeamBone",								Visuals::BoneOnTeam,				ConfigPath);

	Visuals::BoxOnBot =				GetPrivateProfileIntA(("chair"), "BotBox",									Visuals::BoxOnBot,				ConfigPath);
	Visuals::FillBoxOnBot =			GetPrivateProfileIntA(("chair"), "BotFillBox",								Visuals::FillBoxOnBot,			ConfigPath);
	Visuals::TracesOnBot =				GetPrivateProfileIntA(("chair"), "BotTraces",								Visuals::TracesOnBot,			ConfigPath);
	Visuals::DistanceOnBot =			GetPrivateProfileIntA(("chair"), "BotDistance",							Visuals::DistanceOnBot,			ConfigPath);
	Visuals::BoneOnBot =				GetPrivateProfileIntA(("chair"), "BotBone",								Visuals::BoneOnBot,				ConfigPath);
	
	
	Visuals::BoneDisplayRange =			GetPrivateProfileFloat		(("chair"), "BoneDisplayRange",					Visuals::BoneDisplayRange,			ConfigPath);


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

	Visuals::DistanceColor.Value.x = GetPrivateProfileFloat(("chair"), "DistanceColor.X", Visuals::DistanceColor.Value.x, ConfigPath);
	Visuals::DistanceColor.Value.y = GetPrivateProfileFloat(("chair"), "DistanceColor.Y", Visuals::DistanceColor.Value.y, ConfigPath);
	Visuals::DistanceColor.Value.z = GetPrivateProfileFloat(("chair"), "DistanceColor.Z", Visuals::DistanceColor.Value.z, ConfigPath);
	Visuals::DistanceColor.Value.w = GetPrivateProfileFloat(("chair"), "DistanceColor.W", Visuals::DistanceColor.Value.w, ConfigPath);

	
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

	Visuals::TeamDistanceColor.Value.x = GetPrivateProfileFloat(("chair"), "TeamDistanceColor.X", Visuals::TeamDistanceColor.Value.x, ConfigPath);
	Visuals::TeamDistanceColor.Value.y = GetPrivateProfileFloat(("chair"), "TeamDistanceColor.Y", Visuals::TeamDistanceColor.Value.y, ConfigPath);
	Visuals::TeamDistanceColor.Value.z = GetPrivateProfileFloat(("chair"), "TeamDistanceColor.Z", Visuals::TeamDistanceColor.Value.z, ConfigPath);
	Visuals::TeamDistanceColor.Value.w = GetPrivateProfileFloat(("chair"), "TeamDistanceColor.W", Visuals::TeamDistanceColor.Value.w, ConfigPath);


	Visuals::BotBoxColor.Value.x = GetPrivateProfileFloat(("chair"), "BotBoxColor.X", Visuals::BotBoxColor.Value.x, ConfigPath);
	Visuals::BotBoxColor.Value.y = GetPrivateProfileFloat(("chair"), "BotBoxColor.Y", Visuals::BotBoxColor.Value.y, ConfigPath);
	Visuals::BotBoxColor.Value.z = GetPrivateProfileFloat(("chair"), "BotBoxColor.Z", Visuals::BotBoxColor.Value.z, ConfigPath);
	Visuals::BotBoxColor.Value.w = GetPrivateProfileFloat(("chair"), "BotBoxColor.W", Visuals::BotBoxColor.Value.w, ConfigPath);
			 
	Visuals::BotBoxFillColor.Value.x = GetPrivateProfileFloat(("chair"), "BotBoxFillColor.X", Visuals::BotBoxFillColor.Value.x, ConfigPath);
	Visuals::BotBoxFillColor.Value.y = GetPrivateProfileFloat(("chair"), "BotBoxFillColor.Y", Visuals::BotBoxFillColor.Value.y, ConfigPath);
	Visuals::BotBoxFillColor.Value.z = GetPrivateProfileFloat(("chair"), "BotBoxFillColor.Z", Visuals::BotBoxFillColor.Value.z, ConfigPath);
	Visuals::BotBoxFillColor.Value.w = GetPrivateProfileFloat(("chair"), "BotBoxFillColor.W", Visuals::BotBoxFillColor.Value.w, ConfigPath);
			 
	Visuals::BotBoneColor.Value.x = GetPrivateProfileFloat(("chair"), "BotBoneColor.X", Visuals::BotBoneColor.Value.x, ConfigPath);
	Visuals::BotBoneColor.Value.y = GetPrivateProfileFloat(("chair"), "BotBoneColor.Y", Visuals::BotBoneColor.Value.y, ConfigPath);
	Visuals::BotBoneColor.Value.z = GetPrivateProfileFloat(("chair"), "BotBoneColor.Z", Visuals::BotBoneColor.Value.z, ConfigPath);
	Visuals::BotBoneColor.Value.w = GetPrivateProfileFloat(("chair"), "BotBoneColor.W", Visuals::BotBoneColor.Value.w, ConfigPath);
			 
	Visuals::BotTracesColor.Value.x = GetPrivateProfileFloat(("chair"), "BotTracesColor.X", Visuals::BotTracesColor.Value.x, ConfigPath);
	Visuals::BotTracesColor.Value.y = GetPrivateProfileFloat(("chair"), "BotTracesColor.Y", Visuals::BotTracesColor.Value.y, ConfigPath);
	Visuals::BotTracesColor.Value.z = GetPrivateProfileFloat(("chair"), "BotTracesColor.Z", Visuals::BotTracesColor.Value.z, ConfigPath);
	Visuals::BotTracesColor.Value.w = GetPrivateProfileFloat(("chair"), "BotTracesColor.W", Visuals::BotTracesColor.Value.w, ConfigPath);

	Visuals::BotDistanceColor.Value.x = GetPrivateProfileFloat(("chair"), "BotTracesColor.X", Visuals::BotDistanceColor.Value.x, ConfigPath);
	Visuals::BotDistanceColor.Value.y = GetPrivateProfileFloat(("chair"), "BotTracesColor.Y", Visuals::BotDistanceColor.Value.y, ConfigPath);
	Visuals::BotDistanceColor.Value.z = GetPrivateProfileFloat(("chair"), "BotTracesColor.Z", Visuals::BotDistanceColor.Value.z, ConfigPath);
	Visuals::BotDistanceColor.Value.w = GetPrivateProfileFloat(("chair"), "BotTracesColor.W", Visuals::BotDistanceColor.Value.w, ConfigPath);



	Misc::TriggerBot = GetPrivateProfileIntA(("chair"), "TriggerBot", Misc::TriggerBot, ConfigPath);
	Misc::OnlyWhenAimbot = GetPrivateProfileIntA(("chair"), "OnlyWhenAimbot", Misc::OnlyWhenAimbot, ConfigPath);


	Misc::MenuColor.Value.x = GetPrivateProfileFloat(("chair"), "MenuColor.X", Misc::MenuColor.Value.x, ConfigPath);
	Misc::MenuColor.Value.y = GetPrivateProfileFloat(("chair"), "MenuColor.Y", Misc::MenuColor.Value.y, ConfigPath);
	Misc::MenuColor.Value.z = GetPrivateProfileFloat(("chair"), "MenuColor.Z", Misc::MenuColor.Value.z, ConfigPath);
	Misc::MenuColor.Value.w = GetPrivateProfileFloat(("chair"), "MenuColor.W", Misc::MenuColor.Value.w, ConfigPath);

	CloseRange::Enabled = GetPrivateProfileIntA(("chair"), "CloseRangeEnabled", CloseRange::Enabled, ConfigPath);
	
	CloseRange::TriggerBot = GetPrivateProfileIntA(("chair"), "CloseRangeTriggerbotBool", CloseRange::TriggerBot, ConfigPath);
	CloseRange::LockColors = GetPrivateProfileIntA(("chair"), "LockColors", CloseRange::LockColors, ConfigPath);
	CloseRange::DynamicFov = GetPrivateProfileIntA(("chair"), "DynamicFov", CloseRange::DynamicFov, ConfigPath);

	CloseRange::distance = GetPrivateProfileFloat(("chair"), "CloseRangeDistance", CloseRange::distance, ConfigPath);
	CloseRange::MaxFov = GetPrivateProfileFloat(("chair"), "CloseRangeFov", CloseRange::MaxFov, ConfigPath);

	CloseRange::BoxColor.Value.x = GetPrivateProfileFloat(("chair"), "CloseRangeBoxColor.X", CloseRange::BoxColor.Value.x, ConfigPath);
	CloseRange::BoxColor.Value.y = GetPrivateProfileFloat(("chair"), "CloseRangeBoxColor.Y", CloseRange::BoxColor.Value.y, ConfigPath);
	CloseRange::BoxColor.Value.z = GetPrivateProfileFloat(("chair"), "CloseRangeBoxColor.Z", CloseRange::BoxColor.Value.z, ConfigPath);
	CloseRange::BoxColor.Value.w = GetPrivateProfileFloat(("chair"), "CloseRangeBoxColor.W", CloseRange::BoxColor.Value.w, ConfigPath);

	CloseRange::TracesColor.Value.x = GetPrivateProfileFloat(("chair"), "CloseRangeTracesColor.X", CloseRange::TracesColor.Value.x, ConfigPath);
	CloseRange::TracesColor.Value.y = GetPrivateProfileFloat(("chair"), "CloseRangeTracesColor.Y", CloseRange::TracesColor.Value.y, ConfigPath);
	CloseRange::TracesColor.Value.z = GetPrivateProfileFloat(("chair"), "CloseRangeTracesColor.Z", CloseRange::TracesColor.Value.z, ConfigPath);
	CloseRange::TracesColor.Value.w = GetPrivateProfileFloat(("chair"), "CloseRangeTracesColor.W", CloseRange::TracesColor.Value.w, ConfigPath);

	CloseRange::BoneColor.Value.x = GetPrivateProfileFloat(("chair"), "CloseRangeBoneColor.X", CloseRange::BoneColor.Value.x, ConfigPath);
	CloseRange::BoneColor.Value.y = GetPrivateProfileFloat(("chair"), "CloseRangeBoneColor.Y", CloseRange::BoneColor.Value.y, ConfigPath);
	CloseRange::BoneColor.Value.z = GetPrivateProfileFloat(("chair"), "CloseRangeBoneColor.Z", CloseRange::BoneColor.Value.z, ConfigPath);
	CloseRange::BoneColor.Value.w = GetPrivateProfileFloat(("chair"), "CloseRangeBoneColor.W", CloseRange::BoneColor.Value.w, ConfigPath);

	CloseRange::lineThickness = GetPrivateProfileFloat(("chair"), "CloseRangeLineThickness", CloseRange::lineThickness, ConfigPath);

	CloseRange::SmoothX = GetPrivateProfileFloat(("chair"), "CloseRangeSmoothX", CloseRange::SmoothX, ConfigPath);
	CloseRange::SmoothY = GetPrivateProfileFloat(("chair"), "CloseRangeSmoothY", CloseRange::SmoothY, ConfigPath);
	CloseRange::LockSmooth = GetPrivateProfileIntA(("chair"), "CloseRangeLockSmooth", CloseRange::LockSmooth, ConfigPath);
	CloseRange::SmartSmooth = GetPrivateProfileIntA(("chair"), "SmartSmooth", CloseRange::SmartSmooth, ConfigPath);
	CloseRange::InstantInterpolation = GetPrivateProfileIntA(("chair"), "InstantInterpolation", CloseRange::InstantInterpolation, ConfigPath);
	CloseRange::InterpolateColor = GetPrivateProfileIntA(("chair"), "InterpolateColor", CloseRange::InterpolateColor, ConfigPath);

	CloseRange::CurrentFov = GetPrivateProfileFloat(("chair"), "CloseRangeCurrentFov", CloseRange::CurrentFov, ConfigPath);

	Misc::showFPS = GetPrivateProfileIntA(("chair"), "showFPS", Misc::showFPS, ConfigPath);
	Misc::FPSLimit = GetPrivateProfileIntA(("chair"), "FPSLimit", Misc::FPSLimit, ConfigPath);

}

