#pragma once
#include "../driver.h"

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

PVOID get_system_module_base(const char* module_name);
ULONG get_module_base_x64(PEPROCESS proc, UNICODE_STRING module_name);
HANDLE get_process_id(const char* process_name);
PVOID get_system_module_export(const char* module_name, LPCSTR routine_name);

bool write_to_read_only_memory(void* address, void* buffer, size_t size);
PVOID get_base_addressAlt(HANDLE pid);
uintptr_t get_process_cr3(PEPROCESS pprocess);
NTSTATUS read_physical_memory(PVOID address, PVOID buffer, SIZE_T size, PSIZE_T bytes);
uintptr_t translate_linear(uintptr_t directory_table_base, uintptr_t virtual_address);
NTSTATUS read_process_memory(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size);


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