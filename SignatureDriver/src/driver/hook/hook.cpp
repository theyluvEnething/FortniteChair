#include "hook.h"
#include "memory.h"
#include "../driver.h"
#include "../core/Core.h"
#include "../communication.h"
#include "../memory/memory.h"
#include <cstdint>


PVOID(__fastcall* HookedFunction)(PVOID);
//PVOID(__fastcall* NtCompareSigningLevelsOrig)(PVOID, PVOID);


NTSTATUS hook::HookHandler(PVOID CalledParam) {
	/*if (ExGetPreviousMode() != UserMode) {
		return STATUS_SUCCESS(HookedFunction(CalledParam));
	}*/
	//DbgPrintEx(0, 0, "handler called \n");
	DriverCommunicationMessage Request = { 0 };
	if (!Core::ReadVirtualMemory(&Request, CalledParam, sizeof(DriverCommunicationMessage)) || Request.SecurityCode != COMMUNICATION_KEY) {
		//return STATUS_UNSUCCESSFUL;
		//DbgPrintEx(0, 0, "call NOT from usermode. goto orig func\n");
		return STATUS_SUCCESS(HookedFunction(CalledParam));
	}

	//DbgPrintEx(0, 0, "call from usermode!\n");

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
// WINDOWS 10
#define sig_signature_win10 "\x48\x8B\x05\x7D\x02\x06\x00"
#define sig_mask_win10 "xxxxxxx"

// WINDOWS 11
#define sig_signature_win11 "\x48\x8B\x05\xD9\x9C\x06\x00"
#define sig_mask_win11 "xxxxxxx"

bool hook::WriteDataPointer(PVOID KernelFunction) {


	DbgPrintEx(0, 0, "init driver\n");
	//auto base = Core::GetModuleBase("\\SystemRoot\\System32\\win32k.sys");
	auto baseEhStorClass = Core::GetModuleBase("\\SystemRoot\\System32\\drivers\\EhStorClass.sys");
	if (!baseEhStorClass)
	{
		DbgPrintEx(0, 0, "failed base ehstoreclass\n");
		return FALSE;
	}
	auto baseWin32k = Core::GetModuleBase("\\SystemRoot\\System32\\win32k.sys");
	if (!baseWin32k)
	{
		DbgPrintEx(0, 0, "failed base win32k\n");
		return FALSE;
	}
	auto offset = 0xB880;
	auto hookFunc = (ULONG64)baseEhStorClass + offset;
	//DbgPrintEx(0, 0, "windows11check: %p\n", windows11check());
	//DbgPrintEx(0, 0, "baseEhStorClass: %p\n", baseEhStorClass);
	//DbgPrintEx(0, 0, "baseWin32k: %p\n", baseWin32k);
	//DbgPrintEx(0, 0, "hook func: %p\n", hookFunc);
	//DbgPrintEx(0, 0, "hook handler: %p\n", hook::HookHandler);

	uint8_t* addressBytes = reinterpret_cast<uint8_t*>(&KernelFunction);

	///*DbgPrintEx(0, 0, "[+] addressbyte 1: %p\n", addressBytes[0]);
	//DbgPrintEx(0, 0, "[+] addressbyte 2: %p\n", addressBytes[1]);
	//DbgPrintEx(0, 0, "[+] addressbyte 3: %p\n", addressBytes[2]);
	//DbgPrintEx(0, 0, "[+] addressbyte 4: %p\n", addressBytes[3]);
	//DbgPrintEx(0, 0, "[+] addressbyte 5: %p\n", addressBytes[4]);
	//DbgPrintEx(0, 0, "[+] addressbyte 6: %p\n", addressBytes[5]);
	//DbgPrintEx(0, 0, "[+] addressbyte 7: %p\n", addressBytes[6]);
	//DbgPrintEx(0, 0, "[+] addressbyte 8: %p\n", addressBytes[7]);*/

	BYTE new_shell_code[]
	{
		//0x48, 0xC7, 0xC0, addressBytes[4], addressBytes[5], addressBytes[6], addressBytes[7],
		//0x48, 0xC7, 0xC3, addressBytes[0], addressBytes[1], addressBytes[2], addressBytes[3],
		//0x48, 0x8D, 0x05, addressBytes[4], addressBytes[5], addressBytes[6], addressBytes[7], //lea rax
		//0x48, 0x8D, addressBytes[0], addressBytes[1], addressBytes[2], addressBytes[3],
		//0x48, 0x81, 0xC0, addressBytes[4], addressBytes[5], addressBytes[6], addressBytes[7],
		0x48, 0xBB, addressBytes[0], addressBytes[1], addressBytes[2], addressBytes[3], 0x00, 0x00, 0x00, 0x00,
		0x48, 0xB8, addressBytes[4], addressBytes[5], addressBytes[6], addressBytes[7], 0x00, 0x00, 0x00, 0x00,
		0x48, 0xC1, 0xE0, 0x20,
		0x48, 0x09, 0xD8,
		0xFF, 0xE0
		//0xBB, 0xF0, 0x71, 0x15, 0xD8,
		//0xFF, 0x64, 0x24
	};
	PVOID functionExe = reinterpret_cast<PVOID*>(hookFunc);
	if (!WriteToReadOnlyMemory(functionExe, &new_shell_code, sizeof(new_shell_code)))
	{
		return FALSE;
	}
	PBYTE addr;
	if (windows11check())
	{

		addr = Core::FindPattern(baseWin32k,
			sig_signature_win11,
			sig_mask_win11);
	}
	else
	{

		addr = Core::FindPattern(baseWin32k,
			sig_signature_win10,
			sig_mask_win10);
	}

	if (!addr) {
		DbgPrintEx(0, 0, "couldn't find signature\n");
		return FALSE;
	}

	addr = RVA(addr, 7);
	//DbgPrintEx(0, 0, "found address: 0x%x\n", addr);

	*(PVOID*)&HookedFunction =
		InterlockedExchangePointer(
			(volatile PVOID*)addr,
			functionExe
		);

	DbgPrintEx(0, 0, "swapped pointa\n");
	//printf("[mapper] swapped pointer -> 0x%x to 0x%x", addr, &hook::HookHandler);

	return TRUE;
}