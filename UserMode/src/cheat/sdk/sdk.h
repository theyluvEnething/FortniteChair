#pragma once
#include "../../util/util.h"

class SDK {
public:
	static Vector3 GetBoneWithRotation(uintptr_t mesh, int id);
	static Vector2 ProjectWorldToScreen(Vector3 WorldLocation);
	static Camera GetViewAngles();
};

namespace LocalPtrs {
	inline uintptr_t Player = 0; //our local pawn (our player)
	inline uintptr_t Mesh = 0;
	inline uintptr_t PlayerState = 0;
	inline uintptr_t RootComponent = 0;
	inline uintptr_t LocalPlayers = 0;
	inline uintptr_t PlayerController = 0;
	inline uintptr_t Gworld = 0; //uworld != gworld u little fuckers
}
inline Camera vCamera;

class FTextData
{
public:
	char pad_0x0000[0x28];  //0x0000
	wchar_t* Name;          //0x28 
	__int32 Length;         //0x40 
};

typedef struct _FText {
	FTextData* Data;
	char UnknownData[0x10];

	wchar_t* Get() const {
		if (Data)
			return Data->Name;

		return nullptr;
	}
} FText;