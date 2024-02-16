#include "../driver/driver.h"
#include "../../util/util.h"

ULONG ProcId = 0x00000000;
uintptr_t BaseId = 0x00000000;
bCHAR ProcName = 0x00000000;

ULONG driver::find_process(const char* proc_name) {
    DriverCommunicationMessage Msg = { 0 };
    Msg.Code = GetProcId;
    Msg.ProcessName = proc_name;
    call_hook(&Msg);

    return (ULONG)Msg.ProcId;
}

ULONG64 driver::find_image() {
    DriverCommunicationMessage Msg = { 0 };
    Msg.Code = GetBaseId;
    Msg.ProcId = (HANDLE)ProcId;
    Msg.ProcessName = ProcName;
    call_hook(&Msg);

    return Msg.BaseId;
}

template <typename ... Arg>
uint64_t driver::call_hook(const Arg ... args) {
    LoadLibrary(L"user32.dll");

    void* hooked_func = GetProcAddress(LoadLibrary(L"win32u.dll"), "NtFlipObjectConsumerAdjustUsageReference");


    // ======================== //
    //     KERNEL FUNCTION HOOK    //
    // ======================== // 

    auto func = static_cast<uint64_t(_stdcall*)(Arg...)>(hooked_func);
    return func(args ...);
}

bool driver::check() {
    int response = 0;
    DriverCommunicationMessage Msg = { 0 };
    Msg.Code = CheckDriver;
    Msg.Buffer = &response;
    call_hook(&Msg);

    return response;
}