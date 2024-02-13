#pragma once
#include <vector>
#include "data/cache.h"
#include "../util/util.h"

class Cheat
{
public:

	static uintptr_t TargetPawn;
	static float ClosestDistance;
	static int FovSize;
	static float Smooth;
	static bool bAimbot;
	static bool bIsTargeting;

public:
	static std::vector<Vector3> ActorArray;
	static void Init(); 
	static void Present();

	static void Aimbot();
	static void ESP();
};

