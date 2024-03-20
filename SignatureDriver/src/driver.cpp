#include "driver/driver.h"
#include "driver/hook/hook.h"

extern "C" NTSTATUS DriverEntry(DRIVER_OBJECT pDriverObject, UNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pRegistryPath);

	DbgPrintEx(0, 0, "[+] Driver started!\n");

	if (hook::WriteDataPointer(&hook::HookHandler)) {
		DbgPrintEx(0, 0, "[+] Driver loaded!\n");
		DbgPrintEx(0, 0, "[+] NtOpenCompositionSurfaceSectionInfo function hooked!\n");
		return STATUS_SUCCESS;
	}
	else {
		//DbgPrintEx(0, 0, "[!] Unable to hook function!\n");
		return STATUS_FAILED_DRIVER_ENTRY;
	}
}