#include <Windows.h>
#include <iostream>
#include <vector>

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i) {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}

DWORD WINAPI MainThread(HMODULE hModule) {
    AllocConsole();
    FILE* pFile = nullptr;

    freopen_s(&pFile, "CONOUT$", "w", stdout);
    freopen_s(&pFile, "CONIN$", "r", stdin);
    
    SetConsoleTitle(L"Using fibre eor");
    std::cout << "Edit on release" << std::endl;
    std::cout << "Supported fortnite version - 9.41" << std::endl;
    std::cout << "Input your edit bind key" << std::endl;
    std::cout << "(This is only for confirming the edit and the rest of the dll is internal)" << std::endl;
    std::cout << "Press F6 to reset the bool address but not during the loading screen cause the game will crash (press it when you get into a game)" << std::endl;
    
    char key;
    std::cin >> key;

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"FortniteClient-Win64-Shipping.exe");

    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x05F9C7C0;
    
    std::vector<unsigned int> offsets = { 0x0, 0x368, 0x93B};
    uintptr_t editbool = FindDMAAddy(GetCurrentProcess(), dynamicPtrBaseAddr, offsets);
    
    while (true) {
        if (*(bool*)editbool == 1) {
            // Check if mouse button 1 is pressed
            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
                while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && *(bool*)editbool == 1) {
                    Sleep(1);
                    // Wait for mouse button 1 to be released
                }
                
                // Check if still in edit mode
                if (*(bool*)editbool == 1) {
                    keybd_event(key, 0, 0, 0);
                    Sleep(5);
                    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
                }
            }
        }

        if (GetAsyncKeyState(VK_F6) & 0x8000) {
            editbool = FindDMAAddy(GetCurrentProcess(), dynamicPtrBaseAddr, offsets);
            std::cout << "Updated edit bool address" << std::endl;
        }

        Sleep(1);
    }
    
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0 , nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
