#pragma once
#include <stdint.h>
#include <basetsd.h>
#include <ntdef.h>

enum CommunicationCode {
	InitDriver		= 0,
	CheckDriver		= 1,
	GetProcId		= 2,
	GetBaseId		= 3,
	GetPeb			= 4,
	DoReadReq			= 5,
	DoWriteReq		= 6
};

typedef struct _DriverCommunicationMessage {
	uint8_t		Code;
	ULONG64		BaseId;
	HANDLE		ProcId;

	ULONG64		Address;
	PVOID		Buffer;
	ULONG64		bSize;

	const char*	ProcessName;
} DriverCommunicationMessage, * PDriverCommunicationMessage;
typedef DriverCommunicationMessage DCM, * PDCM;