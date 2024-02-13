#include "memory.h"


PVOID get_system_module_base(const char* module_name)
{
	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	if (!bytes)
		return 0;

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x6D726178); // mrax 6D 72 61 78

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status))
		return 0;
	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;

	PVOID module_base = 0, module_size = 0;

	for (ULONG i = 0; i < modules->NumberOfModules; i++)
	{
		if (strcmp((char*)module[i].FullPathName, module_name) == 0)
		{
			module_base = module[i].ImageBase;
			module_size = (PVOID)module[i].ImageSize;
			break;
		}
	}

	if (modules)
		ExFreePoolWithTag(modules, 0);

	if (module_base <= 0)
		return 0;

	return module_base;
}

ULONG get_module_base_x64(PEPROCESS proc, UNICODE_STRING module_name)
{
	//return (ULONG64)PsGetProcessSectionBaseAddress(proc);
	PPEB pPeb = PsGetProcessPeb(proc);
	if (!pPeb)
	{
		return NULL;
	}

	KAPC_STATE state;

	KeStackAttachProcess(proc, &state);

	PPEB_LDR_DATA pLdr = (PPEB_LDR_DATA)pPeb->Ldr;

	if (!pLdr)
	{
		KeUnstackDetachProcess(&state);
		return NULL;
	}

	for (PLIST_ENTRY list = (PLIST_ENTRY)pLdr->ModuleListLoadOrder.Flink; list != &pLdr->ModuleListLoadOrder; list = (PLIST_ENTRY)list->Flink)
	{
		PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);

		if (RtlCompareUnicodeString(&pEntry->BaseDllName, &module_name, TRUE) == NULL)
		{
			ULONG64 baseAddr = (ULONG64)pEntry->DllBase;
			KeUnstackDetachProcess(&state);
			return baseAddr;
		}
	}

	KeUnstackDetachProcess(&state);
	return NULL;

}

HANDLE get_process_id(const char* process_name)
{
	ULONG buffer_size = 0;
	ZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &buffer_size);

	auto buffer = ExAllocatePoolWithTag(NonPagedPool, buffer_size, 'mrax');
	if (!buffer)
		return 0;

	ANSI_STRING process_name_ansi = {};
	UNICODE_STRING process_name_unicode = {};
	RtlInitAnsiString(&process_name_ansi, process_name);
	if (!NT_SUCCESS(RtlAnsiStringToUnicodeString(&process_name_unicode, &process_name_ansi, TRUE)))
		return 0;

	auto process_info = (PSYSTEM_PROCESS_INFO)buffer;
	if (NT_SUCCESS(ZwQuerySystemInformation(SystemProcessInformation, process_info, buffer_size, NULL)))
	{
		while (process_info->NextEntryOffset)
		{
			if (!RtlCompareUnicodeString(&process_name_unicode, &process_info->ImageName, true))
			{
				RtlFreeUnicodeString(&process_name_unicode);
				return process_info->UniqueProcessId;
			}
			process_info = (PSYSTEM_PROCESS_INFO)((BYTE*)process_info + process_info->NextEntryOffset);
		}
	}
	else
	{
		ExFreePoolWithTag(buffer, 'mrax');
		return 0;
	}
}
PVOID get_system_module_export(const char* module_name, LPCSTR routine_name)
{
	PVOID lpModule = get_system_module_base(module_name);

	if (!lpModule)
		return 0;

	return RtlFindExportedRoutineByName(lpModule, routine_name);
}

bool write_to_read_only_memory(void* address, void* buffer, size_t size) {
	PMDL Mdl = IoAllocateMdl(address, size, FALSE, FALSE, NULL);

	if (!Mdl)
		return false;

	MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	PVOID Mapping = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);
	MmProtectMdlSystemAddress(Mdl, PAGE_READWRITE);

	RtlCopyMemory(Mapping, buffer, size);

	MmUnmapLockedPages(Mapping, Mdl);
	MmUnlockPages(Mdl);
	IoFreeMdl(Mdl);

	return true;

}
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

ULONG get_winver()
{
	RTL_OSVERSIONINFOW ver = { 0 };
	RtlGetVersion(&ver);
	switch (ver.dwBuildNumber)
	{
	case win10_1803:
	case win10_1809:
		return 0x0278;
	case win10_1903:
	case win10_1909:
		return 0x0280;
	case win10_2004:
	case win10_20h2:
	case win10_21h1:
	case win10_21h2:
	case win10_22h2:
		return 0x0388;
	case win11_21h2:
	case win11_22h2:
		return 0x0390;
	default:
		return 0x0390;
	}
}

PVOID get_base_addressAlt(HANDLE pid)
{
	if (!pid) return 0;
	PEPROCESS process = 0;
	PsLookupProcessByProcessId((HANDLE)pid, &process);
	if (!process) return 0;
	PVOID image_base = PsGetProcessSectionBaseAddress(process);
	if (!image_base) return 0;
	ObDereferenceObject(process);
	return image_base;
}

uintptr_t saved_dirbase = 0;
bool already_attached = false;
uintptr_t get_process_cr3(PEPROCESS pprocess)
{
	if (!pprocess) return 0;
	uintptr_t process_dirbase = *(uintptr_t*)((UINT8*)pprocess + 0x28);
	if (process_dirbase == 0)
	{
		ULONG user_diroffset = get_winver();
		process_dirbase = *(uintptr_t*)((UINT8*)pprocess + user_diroffset);
	}
	if ((process_dirbase >> 0x38) == 0x40)
	{
		if (!already_attached) //find a way to reset this
		{
			KAPC_STATE apc_state{};
			KeStackAttachProcess(pprocess, &apc_state);
			saved_dirbase = __readcr3();
			KeUnstackDetachProcess(&apc_state);
			already_attached = true;
		}
		if (saved_dirbase) return saved_dirbase;
	}
	return process_dirbase;
}

NTSTATUS read_physical_memory(PVOID address, PVOID buffer, SIZE_T size, PSIZE_T bytes)
{
	if (!address) return STATUS_UNSUCCESSFUL;
	MM_COPY_ADDRESS to_read = { 0 };
	to_read.PhysicalAddress.QuadPart = (LONGLONG)address;
	return MmCopyMemory(buffer, to_read, size, MM_COPY_MEMORY_PHYSICAL, bytes);
}
uintptr_t translate_linear(uintptr_t directory_table_base, uintptr_t virtual_address)
{
	directory_table_base &= ~0xf;
	uintptr_t pageoffset = virtual_address & ~(~0ul << page_offset_size);
	uintptr_t pte = ((virtual_address >> 12) & (0x1ffll));
	uintptr_t pt = ((virtual_address >> 21) & (0x1ffll));
	uintptr_t pd = ((virtual_address >> 30) & (0x1ffll));
	uintptr_t pdp = ((virtual_address >> 39) & (0x1ffll));
	SIZE_T readsize = 0;
	uintptr_t pdpe = 0;
	read_physical_memory((PVOID)(directory_table_base + 8 * pdp), &pdpe, sizeof(pdpe), &readsize);
	if (~pdpe & 1) return 0;
	uintptr_t pde = 0;
	read_physical_memory((PVOID)((pdpe & pmask) + 8 * pd), &pde, sizeof(pde), &readsize);
	if (~pde & 1) return 0;
	if (pde & 0x80) return (pde & (~0ull << 42 >> 12)) + (virtual_address & ~(~0ull << 30));
	uintptr_t ptraddr = 0;
	read_physical_memory((PVOID)((pde & pmask) + 8 * pt), &ptraddr, sizeof(ptraddr), &readsize);
	if (~ptraddr & 1) return 0;
	if (ptraddr & 0x80) return (ptraddr & pmask) + (virtual_address & ~(~0ull << 21));
	virtual_address = 0;
	read_physical_memory((PVOID)((ptraddr & pmask) + 8 * pte), &virtual_address, sizeof(virtual_address), &readsize);
	virtual_address &= pmask;
	if (!virtual_address) return 0;
	return virtual_address + pageoffset;
}

NTSTATUS read_process_memory(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size)
{
	if (!pid) return STATUS_UNSUCCESSFUL;
	PEPROCESS process = 0;
	PsLookupProcessByProcessId(pid, &process);
	if (!process) return STATUS_UNSUCCESSFUL;
	uintptr_t process_base = get_process_cr3(process);
	if (!process_base) return STATUS_UNSUCCESSFUL;
	ObDereferenceObject(process);
	uintptr_t physical_address = translate_linear(process_base, (uintptr_t)address);
	if (!physical_address) return STATUS_UNSUCCESSFUL;
	uintptr_t final_size = min(PAGE_SIZE - (physical_address & 0xFFF), size);
	SIZE_T bytes_trough = 0;
	read_physical_memory((PVOID)physical_address, buffer, final_size, &bytes_trough);
	return STATUS_SUCCESS;
}


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

/*NTSTATUS protect_virtual_memory(HANDLE pid, PVOID address, ULONG size, ULONG protection, ULONG& protection_out)
{
	if (!pid || !address || !size || !protection)
		return STATUS_INVALID_PARAMETER;
	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS target_process = nullptr;

	if (!NT_SUCCESS(PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(pid), &target_process)))
	{
		return STATUS_NOT_FOUND;
	}

	ULONG protection_old = 0;

	KAPC_STATE state;
	KeStackAttachProcess(target_process, &state);

	status = ZwProtectVirtualMemory(NtCurrentProcess(), &address, &size, protection, &protection_old);

	KeUnstackDetachProcess(&state);

	if (NT_SUCCESS(status))
	{
		protection_out = protection_old;
	}

	ObDereferenceObject(target_process);
	return status;

}*/


