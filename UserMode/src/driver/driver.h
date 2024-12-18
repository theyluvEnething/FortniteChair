#pragma once
#include <Windows.h>
#include <memory>
#include <cstdint>
#include <vector>
#include <string_view>
#include <iostream>
#include "communication.h"
#include "../util/util.h"

typedef char* bCHAR;

extern ULONG ProcId;
extern uintptr_t BaseId;
extern bCHAR ProcName;

#define USE_FUNCTION_HOOK_DRIVER FALSE
#define USE_SIGNATURE_SCAN_DRIVER TRUE


class driver
{
public:
	static BOOL WhichDriver;
	//static NTSTATUS(*HookFunc)(PVOID);

	static uint64_t(*HookFunc)(PVOID);

	static BOOL setup();
	static ULONG find_process(const char* proc_name);
	static ULONG64 find_image();
	static bool check();
	static bool is_valid(const uintptr_t address);
	static bool read(const uintptr_t address, void* buffer, size_t size);
	static bool write(const uintptr_t address, void* buffer, size_t size);

	template <typename ... Arg>
	static uint64_t call_hook(const Arg ... args);

	template <class T>
	static T read(const uintptr_t ReadAddress) {
		T response{};

		DriverCommunicationMessage Msg = { 0 };
		Msg.SecurityCode = DRIVER_CHECK_CODE;
		Msg.Code = DoReadReq;
		Msg.ProcId = (HANDLE)ProcId;
		Msg.BaseId = (ULONG64)BaseId;
		Msg.Address = ReadAddress;
		Msg.Buffer = &response;
		Msg.bSize = sizeof(T);
		driver::call_hook(&Msg);

		return response;
	}


	template <class T>
	static bool write(const uintptr_t WriteAddress, T value) {
		DriverCommunicationMessage Msg = { 0 };
		Msg.SecurityCode = DRIVER_CHECK_CODE;
		Msg.Code = DoWriteReq;
		Msg.ProcId = (HANDLE)ProcId;
		Msg.BaseId = (ULONG64)BaseId;
		Msg.Address = WriteAddress;
		Msg.Buffer = (PVOID)&value;
		Msg.bSize = sizeof(T);

		driver::call_hook(&Msg);
		return TRUE;
	}


private:
	// static bool write_memory(uintptr_t write_address, uintptr_t source_address, uintptr_t write_size);

	struct HandleDisposer {

		using pointere = HANDLE;
		void operator()(HANDLE handle) const {
			if (handle != NULL || handle != INVALID_HANDLE_VALUE) {
				CloseHandle(handle);
			}
		}
	};
	using unique_handle = std::unique_ptr < HANDLE, HandleDisposer>;
};

template <class T>
T read(uintptr_t read_address) {
	return driver::read<T>(read_address);
}

template <class T>
bool write(uintptr_t write_address, T& value) {
	return driver::write<T>(write_address, value);
}