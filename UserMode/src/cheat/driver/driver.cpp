#include "../driver/driver.h"
#include "../../util/util.h"

ULONG ProcId = 0x00000000;
uintptr_t BaseId = 0x00000000;
bCHAR ProcName = 0x00000000;

BOOL driver::WhichDriver = USE_SIGNATURE_SCAN_DRIVER;
uint64_t(*driver::HookFunc)(PVOID) = nullptr;


BOOL driver::setup() {
    if (driver::WhichDriver == USE_FUNCTION_HOOK_DRIVER) {
        std::cout << "[+] Using Function Hook Driver!" << std::endl;
        LoadLibrary(L"user32.dll");
        void* hooked_func = GetProcAddress(LoadLibrary(L"win32u.dll"), "NtFlipObjectEnablePresentStatisticsType");
        HookFunc = static_cast<uint64_t(_stdcall*)(PVOID)>(hooked_func);
        return TRUE;
    }
    if (driver::WhichDriver == USE_SIGNATURE_SCAN_DRIVER) {
        std::cout << "[+] Using Signature Scan Driver!" << std::endl;
        auto ntdll = LoadLibraryA("ntdll.dll");
        if (!ntdll) {
            std::cout << "[!] Failed to load " << "NtCompareSigningLevels" << ".dll" << std::endl;
            return FALSE;
        }
        auto addr = GetProcAddress(ntdll, "NtCompareSigningLevels");
        if (!addr) {
            std::cout << "[!] Failed to find routine address!" << std::endl;
            return FALSE;
        }
        *(PVOID*)&HookFunc = addr;
        return TRUE;
    }
}

ULONG driver::find_process(const char* proc_name) {
    DriverCommunicationMessage Msg = { 0 };
    Msg.Code = GetProcId;
    Msg.SecurityCode = DRIVER_CHECK_CODE;
    Msg.ProcessName = proc_name;

    call_hook(&Msg);
    return (ULONG)Msg.ProcId;
}

ULONG64 driver::find_image() {
    DriverCommunicationMessage Msg = { 0 };
    Msg.Code = GetBaseId;
    Msg.SecurityCode = DRIVER_CHECK_CODE;
    Msg.ProcId = (HANDLE)ProcId;
    Msg.ProcessName = ProcName;
    
    call_hook(&Msg);
    return Msg.BaseId;
}

template <typename ... Arg>
uint64_t driver::call_hook(const Arg ... args) {
    return HookFunc(args ...);
}

bool driver::check() {
    int response = 0;
    DriverCommunicationMessage Msg = { 0 };
    Msg.Code = CheckDriver;
    Msg.SecurityCode = DRIVER_CHECK_CODE;
    Msg.Buffer = &response;
    call_hook(&Msg);

    return response;
}