#include "hook.h"
#include "communication.h"

void ReadVirtualMemory(HANDLE ProcId, PVOID Address, PVOID Buffer, SIZE_T Size) {
	if (!Address || !Buffer || !Size || (int)ProcId == 976)
		return;

	PEPROCESS Process;
	if (!NT_SUCCESS(PsLookupProcessByProcessId(ProcId, &Process)))
		return;
	
	MmCopyVirtualMemory(Process, Address, PsGetCurrentProcess(), Buffer, Size, KernelMode, NULL);
}

NTSTATUS hook::HookHandler(PVOID CalledParam) {

	DriverCommunicationMessage *Msg = (DriverCommunicationMessage*)CalledParam;
	if (Msg->SecurityCode != DRIVER_CHECK_CODE) {
		return STATUS_UNSUCCESSFUL;
	}

	switch (Msg->Code) {
		case InitDriver: {
			*(int*)Msg->Buffer = 1;
			DbgPrintEx(0, 0, "[+] Driver has been initialized!\n");
		} break;

		case CheckDriver: {
			*(int*)Msg->Buffer = 1;
			DbgPrintEx(0, 0, "[+] Driver is still active!\n");
		} break;

		case GetProcId: {
			Msg->ProcId = get_process_id(Msg->ProcessName);
			DbgPrintEx(0, 0, "[+] ProcId requested by Usermode for %s : %d\n", Msg->ProcessName, Msg->ProcId);
		} break;
		
		case GetBaseId: {
			Msg->BaseId = (ULONG64)get_base_addressAlt(Msg->ProcId);
			DbgPrintEx(0, 0, "[+] BaseId requested by Usermode for %s : %lld\n", Msg->ProcessName, Msg->BaseId);
		} break;
		
		case GetPeb: {
			PEPROCESS process = NULL;
			Msg->Buffer = (PVOID)PsGetProcessPeb(process);
			DbgPrintEx(0, 0, "[+] Peb requested by Usermode.\n");
		} break;
			
		case DoReadReq: {
			read_process_memory((HANDLE)Msg->ProcId, (PVOID)Msg->Address, (PVOID)Msg->Buffer, Msg->bSize);
			DbgPrintEx(0, 0, "[+] Read requested by Usermode.\n");
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


bool hook::CallKernelFunction(PVOID KernelFunctionAddress) {
	if (!KernelFunctionAddress)
		return FALSE;


	// ======================== //
	//	 KERNEL FUNCTION HOOK	//
	// ======================== // 
	PVOID* hookFunction = reinterpret_cast<PVOID*>(get_system_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys", "NtFlipObjectEnablePresentStatisticsType"));
	
	// NtGdiDdDDINetDispGetNextChunkInfo


	if (!hookFunction)
		return FALSE;

	// { ex4C, ex8B, exDC, ex49, ex89, ex5B, ex18, ex4D, ex89, ex4B, ex2ø, øx49, øx89, øx4B, øxø8 };
							 
	//BYTE original_func[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };



	/*BYTE ud_shell_start[]
	{
		// 0x48, 0xB8			// 0x48 is mov, 0xBB is rax; then our func address rax
		//0x48, 0x89, 0xE0,
		// 0x6A, 1337,
		0x48, 0xB8				// 0x48 is mov, 0xB8 is rax; then our func address rax
	};
	BYTE ud_shell_end[]
	{
		// 0x48, 0x89, 0xC8, 
		
		// 0xFF, 0xC1,
		0x48, 0xFF, 0xC0,		// inc rax
		
		//0x48, 0x89, 0xC8,		// mov rax, rcx
		//0x48, 0x89, 0xC1,		// mov rcx, rax
		//0x89, 0xC0,			// mov eac, rax
		

		//0xFF, 0xE1				// jmp rcx
		0xFF, 0xE0			// jump rax 
	};*/
	

	//DbgPrintEx(0, 0, "[+] Kernel Function Address: %p\n", KernelFunctionAddress);
	uint8_t* addressBytes = reinterpret_cast<uint8_t*>(&KernelFunctionAddress);
	/*DbgPrintEx(0, 0, "[+] addressbyte 1: %p\n", addressBytes[0]);
	DbgPrintEx(0, 0, "[+] addressbyte 2: %p\n", addressBytes[1]);
	DbgPrintEx(0, 0, "[+] addressbyte 3: %p\n", addressBytes[2]);
	DbgPrintEx(0, 0, "[+] addressbyte 4: %p\n", addressBytes[3]);
	DbgPrintEx(0, 0, "[+] addressbyte 5: %p\n", addressBytes[4]);
	DbgPrintEx(0, 0, "[+] addressbyte 6: %p\n", addressBytes[5]);
	DbgPrintEx(0, 0, "[+] addressbyte 7: %p\n", addressBytes[6]);
	DbgPrintEx(0, 0, "[+] addressbyte 8: %p\n", addressBytes[7]);*/
	/*RtlSecureZeroMemory(&original_func, sizeof(original_func));
	memcpy((PVOID)((ULONG_PTR)original_func), &ud_shell_start, sizeof(ud_shell_start));
	uintptr_t test_address = reinterpret_cast<uintptr_t>((PVOID)(((long long)KernelFunctionAddress) - 1));
	memcpy((PVOID)((ULONG_PTR)original_func + sizeof(ud_shell_start)), &test_address, sizeof(void*));
	memcpy((PVOID)((ULONG_PTR)original_func + sizeof(ud_shell_start) + sizeof(void*)), &ud_shell_end, sizeof(ud_shell_end));*/

	/*BYTE new_shell_code[]
	{
		0x48, 0x31, 0xC0, // xor rax, rax
		0x48, 0x89, 0xD3, addressBytes[7], addressBytes[6], addressBytes[5], addressBytes[4], // mov rbx, segment
		0x48, 0xC1, 0xE3, 20,
		0x48, 0x89, 0xC1, addressBytes[3], addressBytes[2], addressBytes[1], addressBytes[0],
		0x48, 0x09, 0xCB,
		0xFF, 0xE3
	};*/

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


	write_to_read_only_memory(hookFunction, &new_shell_code, sizeof(new_shell_code));
	return TRUE;
}