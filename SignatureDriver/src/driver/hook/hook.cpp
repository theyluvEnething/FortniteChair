#include "hook.h"
#include "memory.h"
#include "../driver.h"
#include "../core/Core.h"
#include "../communication.h"
#include "../memory/memory.h"

PVOID(__fastcall* HookedFunction)(PVOID);
//PVOID(__fastcall* NtCompareSigningLevelsOrig)(PVOID, PVOID);


NTSTATUS hook::HookHandler(PVOID CalledParam) {
	if (ExGetPreviousMode() != UserMode) {
		return STATUS_SUCCESS(HookedFunction(CalledParam));
	}
	DriverCommunicationMessage Request = { 0 };
	if (!Core::ReadVirtualMemory(&Request, CalledParam, sizeof(DriverCommunicationMessage)) || Request.SecurityCode != COMMUNICATION_KEY) {
		return STATUS_UNSUCCESSFUL;
		//return STATUS_SUCCESS(HookedFunction(CalledParam));
	}

	DriverCommunicationMessage* Msg = (DriverCommunicationMessage*)CalledParam;

	switch (Request.Code) {
		case InitDriver: {
			*(int*)Msg->Buffer = 1;
			//DbgPrintEx(0, 0, "[+] Driver has been initialized!\n");
		} break;

		case CheckDriver: {
			*(int*)Msg->Buffer = 1;
			//DbgPrintEx(0, 0, "[+] Driver is still active!\n");
		} break;

		case GetProcId: {
			Msg->ProcId = get_process_id(Msg->ProcessName);
			//DbgPrintEx(0, 0, "[+] ProcId requested by Usermode for %s : %d\n", Msg->ProcessName, Msg->ProcId);
		} break;

		case GetBaseId: {
			Msg->BaseId = (ULONG64)get_base_addressAlt(Msg->ProcId);
			//DbgPrintEx(0, 0, "[+] BaseId requested by Usermode for %s : %lld\n", Msg->ProcessName, Msg->BaseId);
		} break;

		case GetPeb: {
			PEPROCESS process = NULL;
			Msg->Buffer = (PVOID)PsGetProcessPeb(process);
			//DbgPrintEx(0, 0, "[+] Peb requested by Usermode.\n");
		} break;

		case DoReadReq: {
			//DbgPrintEx(0, 0, "[+] Read requested: %lld", Msg->Address);
			
			read_process_memory((HANDLE)Msg->ProcId, (PVOID)Msg->Address, (PVOID)Msg->Buffer, Msg->bSize);
			//ReadVirtualMemory((HANDLE)Msg->ProcId, (PVOID)Msg->Address, (PVOID)Msg->Buffer, Msg->bSize);
		} break;

		case DoWriteReq: {
			// Not implemented for UD
			//DbgPrintEx(0, 0, "[+] Write not implemented bozo");
		} break;

		default: {
			return STATUS_UNSUCCESSFUL;
		} break;
	}

	return STATUS_SUCCESS;
}
// NtUserGetCurrentDpiInfoForWindow
#define sig_signature "\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xFF\x15\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x33\xC9"
#define sig_mask "xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xxxx?xx????xxx?xxxxxxxxxxxxx?xxx????xx"

bool hook::WriteDataPointer() {

	//DbgPrintEx(0, 0, "init driver\n");
	auto base = Core::GetModuleBase("\\SystemRoot\\System32\\win32k.sys");
	if (!base)
	{
		//DbgPrintEx(0, 0, "failed getting base\n");
		return FALSE;
	}
	else
	{
		//DbgPrintEx(0, 0, "base: 0x%x\n", base);

	}



	auto addr = Core::FindPattern(base,
		sig_signature,
		sig_mask);

	if (!addr) {
		//DbgPrintEx(0, 0, "couldn't find signature\n");
		return FALSE;
	}

	addr = RVA(addr, 7);
	//DbgPrintEx(0, 0, "found address: 0x%x\n", addr);

	*(PVOID*)&HookedFunction =
		InterlockedExchangePointer(
			(volatile PVOID*)addr,
			hook::HookHandler
		);

	//DbgPrintEx(0, 0, "swapped pointa\n");
	//printf("[mapper] swapped pointer -> 0x%x to 0x%x", addr, &hook::HookHandler);

	return TRUE;
}