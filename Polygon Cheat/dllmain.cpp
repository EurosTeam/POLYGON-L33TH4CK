#include <Windows.h>
#include <iostream>
#include <thread>

uintptr_t norecoil_offset = 0x3E6FFC8;
uintptr_t rapidfire_offset = 0x3C10E64;

uintptr_t modBase = (uintptr_t)GetModuleHandleA("POLYGON-Win64-Shipping.exe");

DWORD WINAPI MainThread(HMODULE hModule)
{
    DWORD oldprotect;
    while (true)
    {
        VirtualProtect((LPVOID)(modBase + norecoil_offset), 100, PAGE_EXECUTE_READWRITE, &oldprotect);
        VirtualProtect((LPVOID)(modBase + rapidfire_offset), 100, PAGE_EXECUTE_READWRITE, &oldprotect);
        *(float*)(modBase + norecoil_offset) = 0.0f; // thicc n0 r3co1l
        *(int*)(modBase + rapidfire_offset) = 0; //thicc r4p1d f1r3
        if (GetAsyncKeyState(VK_END) & 1)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(0, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, 0));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

