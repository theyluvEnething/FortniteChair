#pragma once

enum CommunicationCode {
	InitDriver = 0,
	CheckDriver = 1,
	GetProcId = 2,
	GetBaseId = 3,
	GetPeb = 4,
	DoReadReq = 5,
	DoWriteReq = 6
};

#define DRIVER_CHECK_CODE 0xDEADBEEF

typedef struct _DriverCommunicationMessage {
	DWORD		SecurityCode;
	UINT8		Code;
	ULONG64		BaseId;
	HANDLE		ProcId;

	ULONG64		Address;
	PVOID		Buffer;
	ULONG64		bSize;

	const char* ProcessName;
} DriverCommunicationMessage, * PDriverCommunicationMessage;
typedef DriverCommunicationMessage DCM, * PDCM;