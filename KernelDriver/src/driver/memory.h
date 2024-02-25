#pragma once
#include "driver.h"
#include <stdint.h>
#include <intrin.h>
#include <wdm.h>  

#define page_offset_size 12
static const uintptr_t pmask = (~0xfull << 8) & 0xfffffffffull;

#define win10_1803 17134
#define win10_1809 17763
#define win10_1903 18362
#define win10_1909 18363
#define win10_2004 19041
#define win10_20h2 19042
#define win10_21h1 19043
#define win10_21h2 19044
#define win10_22h2 19045
#define win11_21h2 22000
#define win11_22h2 22621
ULONG get_winver();

PVOID GetSystemModuleBase(const char* module_name);
ULONG GetModuleBase64x(PEPROCESS proc, UNICODE_STRING module_name);
HANDLE GetProcessId(const char* process_name);
PVOID GetSystemModuleExport(const char* module_name, LPCSTR routine_name);

NTSTATUS write_physical_memory(PVOID address, PVOID buffer, SIZE_T size, PSIZE_T bytes);
NTSTATUS write_process_memory(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size);
bool write_to_read_only_memory(void* address, void* buffer, size_t size);
PVOID get_base_addressAlt(HANDLE pid);
uintptr_t GetProcessCr3(PEPROCESS pprocess);
NTSTATUS ReadPhysicalMemory(PVOID address, PVOID buffer, SIZE_T size, PSIZE_T bytes);
uintptr_t TranslateLinearAddress(uintptr_t directory_table_base, uintptr_t virtual_address);
NTSTATUS ReadProcessMemory(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size);

NTSTATUS WritePhysicalMemory(PVOID TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesWritten);
NTSTATUS WriteVirtualMemory(uint64_t dirbase, uint64_t address, uint8_t* buffer, SIZE_T size, SIZE_T* written);
NTSTATUS WriteProcessMemory(HANDLE ProcId, PVOID Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* written);


/*bool read_kernel_memory(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size)
{
	if (!address || !buffer || !size)
		return 0;
	SIZE_T bytes = 0;
	PEPROCESS process;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &process)))
		return false;

	return MmCopyVirtualMemory(process, address, PsGetCurrentProcess(), buffer, size, KernelMode, &bytes) == STATUS_SUCCESS;
}

bool write_kernel_memory(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size)
{
	if (!address || !buffer || !size)
		return 0;
	SIZE_T bytes = 0;
	PEPROCESS process;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &process)))
		return false;

	return MmCopyVirtualMemory(PsGetCurrentProcess(), address, process, buffer, size, KernelMode, &bytes) == STATUS_SUCCESS;
}*/