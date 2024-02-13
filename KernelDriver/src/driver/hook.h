#pragma once
#include <ntifs.h>
#include "memory.h"
#pragma warning (disbale : 4130)

namespace hook {
	NTSTATUS HookHandler(PVOID CalledParam);
	bool CallKernelFunction(PVOID KernelFunctionAddress);
}