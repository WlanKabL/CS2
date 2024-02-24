#include <Windows.h>
#include "d3d.h"


void AllocateConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD AttachReason, LPVOID lpReserved)
{

    UNREFERENCED_PARAMETER(lpReserved);

    if (AttachReason != DLL_PROCESS_ATTACH)
        return FALSE;


    AllocateConsole();
    hkRender();


    return TRUE;
}