#pragma once
#include "../memory/memory.h"
#pragma warning (disbale : 4130)

namespace hook {
	NTSTATUS HookHandler(PVOID CalledParam);
	bool WriteDataPointer(PVOID Kernelfunction);
}