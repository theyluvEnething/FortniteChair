#pragma once
#include "../driver.h"


namespace Core {

	PVOID GetModuleBase(LPCSTR moduleName) {
		PVOID moduleBase = NULL;
		ULONG info = 0;
		NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, info, &info);

		if (!info) {
			return moduleBase;
		}

		PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, info, 'ENTH');

		status = ZwQuerySystemInformation(SystemModuleInformation, modules, info, &info);

		if (!NT_SUCCESS(status)) {
			return moduleBase;
		}

		PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;


		if (modules->NumberOfModules > 0) {

			if (!moduleName) {
				moduleBase = modules->Modules[0].ImageBase;
			}
			else {

				for (auto i = 0; i < modules->NumberOfModules; i++) {

					if (!strcmp((CHAR*)module[i].FullPathName, moduleName)) {
						moduleBase = module[i].ImageBase;
					}
				}
			}
		}

		if (modules) {
			ExFreePoolWithTag(modules, 'ENTH');
		}

		return moduleBase;
	}

	PIMAGE_NT_HEADERS GetHeader(PVOID module) {
		return (PIMAGE_NT_HEADERS)((PBYTE)module + PIMAGE_DOS_HEADER(module)->e_lfanew);
	}

	PBYTE FindPattern(PVOID module, DWORD size, LPCSTR pattern, LPCSTR mask) {

		auto checkMask = [](PBYTE buffer, LPCSTR pattern, LPCSTR mask) -> BOOL
			{
				for (auto x = buffer; *mask; pattern++, mask++, x++) {
					auto addr = *(BYTE*)(pattern);
					if (addr != *x && *mask != '?')
						return FALSE;
				}

				return TRUE;
			};

		for (auto x = 0; x < size - strlen(mask); x++) {

			auto addr = (PBYTE)module + x;
			if (checkMask(addr, pattern, mask))
				return addr;
		}

		return NULL;
	}

	PBYTE FindPattern(PVOID base, LPCSTR pattern, LPCSTR mask) {

		auto header = GetHeader(base);
		auto section = IMAGE_FIRST_SECTION(header);

		for (auto x = 0; x < header->FileHeader.NumberOfSections; x++, section++) {

			/*
			* Avoids non paged memory,
			* As well as greatly speeds up the process of scanning 30+ sections.
			*/
			if (!memcmp(section->Name, ".text", 5) || !memcmp(section->Name, "PAGE", 4)) {
				auto addr = FindPattern((PBYTE)base + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);
				if (addr) {
					printf("[mapper] Found in Section -> [ %s ]", section->Name);
					return addr;
				}
			}
		}

		return NULL;
	}

	BOOL ReadVirtualMemory(PVOID Output, PVOID Address, SIZE_T Size) {

		size_t pSize;
		if (NT_SUCCESS(MmCopyVirtualMemory(PsGetCurrentProcess(), Address,
			PsGetCurrentProcess(), Output, Size, KernelMode, &pSize)
		) && Size == pSize) {
			return TRUE;
		}

		return FALSE;
	}
}


