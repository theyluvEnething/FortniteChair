#pragma once
#include <vector>
#include "sdk/sdk.h"
#include "data/cache.h"
#include "../util/util.h"

class Cheat
{
public:
	static std::vector<APlayerInformation> PlayerArray;

	static uintptr_t TargetPawn;
	static uint64_t TargetMesh;
	static float ClosestDistance;

public:
	static void Init(); 
	static void Present();
	static void Update();
	static void LateUpdate();

	static void Aimbot();
	static void ActorLoop();
	static void TriggerBot();
};

