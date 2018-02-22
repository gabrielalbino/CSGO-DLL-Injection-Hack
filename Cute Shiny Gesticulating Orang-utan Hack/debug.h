#pragma once
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <FCNTL.H>
#include <io.h>

namespace Console {

	static BOOL WINAPI MyConsoleCtrlHandler(DWORD dwCtrlEvent) { return dwCtrlEvent == CTRL_C_EVENT; }

	void attachToConsole()
	{
		FILE * pConsole;
		AllocConsole();
		freopen_s(&pConsole, "CONOUT$", "wb", stdout);
	}

}