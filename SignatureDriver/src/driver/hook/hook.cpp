#include "hook.h"
#include "memory.h"
#include "../driver.h"
#include "../core/Core.h"
#include "../communication.h"
#include "../memory/memory.h"

PVOID(__fastcall* HookedFunction)(PVOID);

NTSTATUS hook::HookHandler(PVOID CalledParam) {

	if (ExGetPreviousMode() != UserMode) {
		return STATUS_SUCCESS(HookedFunction(CalledParam));
	}

	DriverCommunicationMessage Msg = { 0 };
	if (!Core::ReadVirtualMemory(&Msg, CalledParam, sizeof(DriverCommunicationMessage)) || Msg.Check != COMMUNICATION_KEY) {
		return STATUS_SUCCESS(HookedFunction(CalledParam));
	}


	switch (Msg.Code) {
		case InitDriver: {
			*(int*)Msg.Buffer = 1;
			//DbgPrintEx(0, 0, "[+] Driver has been initialized!\n");
		} break;

		case CheckDriver: {
			*(int*)Msg.Buffer = 1;
			//DbgPrintEx(0, 0, "[+] Driver is still active!\n");
		} break;

		case GetProcId: {
			Msg.ProcId = get_process_id(Msg.ProcessName);
			//DbgPrintEx(0, 0, "[+] ProcId requested by Usermode for %s : %d\n", Msg->ProcessName, Msg->ProcId);
		} break;

		case GetBaseId: {
			Msg.BaseId = (ULONG64)get_base_addressAlt(Msg.ProcId);
			//DbgPrintEx(0, 0, "[+] BaseId requested by Usermode for %s : %lld\n", Msg->ProcessName, Msg->BaseId);
		} break;

		case GetPeb: {
			PEPROCESS process = NULL;
			Msg.Buffer = (PVOID)PsGetProcessPeb(process);
			//DbgPrintEx(0, 0, "[+] Peb requested by Usermode.\n");
		} break;

		case DoReadReq: {
			read_process_memory((HANDLE)Msg.ProcId, (PVOID)Msg.Address, (PVOID)Msg.Buffer, Msg.bSize);
			//DbgPrintEx(0, 0, "[+] Read requested: %lld", instructions->Address);
			//ReadVirtualMemory((HANDLE)Msg->ProcId, (PVOID)Msg->Address, (PVOID)Msg->Buffer, Msg->bSize);
		} break;

		case DoWriteReq: {
			// Not implemented for UD
		} break;

		default: {
			return STATUS_UNSUCCESSFUL;
		} break;
	}

	return STATUS_SUCCESS;
}


bool hook::WriteDataPointer() {

	auto base = Core::GetModuleBase(0);
	if (!base)
		return FALSE;
	else
		printf("[mapper] ntoskrnl.exe -> 0x%x", base);

	auto addr = Core::FindPattern(base,
		"\x4C\x8B\x05\x00\x00\x00\x00\x33\xC0\x4D\x85\xC0\x74\x08\x49\x8B\xC0\xE8\x00\x00\x00\x00\xF7\xD8",
		"xxx????xxxxxxxxxxx????xx");

	if (!addr) {
		printf("[mapper] Unable to find signature!");
		return STATUS_FAILED_DRIVER_ENTRY;
	}

	addr = RVA(addr, 7);
	printf("[mapper] addr -> 0x%x", addr);

	*(PVOID*)&HookedFunction =
		InterlockedExchangePointer(
			(volatile PVOID*)addr,
			hook::HookHandler
		);

	printf("[mapper] swapped pointer -> 0x%x to 0x%x", addr, &hook::HookHandler);

	return TRUE;
}