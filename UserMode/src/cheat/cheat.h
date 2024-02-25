#pragma once
#include <vector>
#include "data/cache.h"
#include "../util/util.h"

class Cheat
{
public:

	static uintptr_t TargetPawn;
	static uint64_t TargetMesh;
	static char TargetPawnTeamId;
	static float ClosestDistance;

public:
	static void Init(); 
	static void Present();
	static void Update();
	static void LateUpdate();

	static void Aimbot();
	static void Esp();
	static void TriggerBot();
};

