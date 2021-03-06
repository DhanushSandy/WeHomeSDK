// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <windows.h>
#include "platform.h"

#pragma comment(lib,"LinkApi.lib")
#pragma comment(lib,"PPCS_API.lib")
#pragma comment(lib,"pthreadVSE2.lib")
#pragma comment(lib,"NDT_API_PPCS.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		{
			initWinSocket();
		}
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		{
			deInitWinSocket();
		}
        break;
    }
    return TRUE;
}

