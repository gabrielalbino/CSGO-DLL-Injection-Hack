#include <Windows.h>
#include "../valve_sdk/sdk.hpp"
#include "../helpers/utils.hpp"
#include "../hooks.h"

bool rcsActive = false;
bool aimAssistActive = false;
bool triggerBotActive = false;

LPTHREAD_START_ROUTINE WINAPI startHack() {
	Utils::AttachConsole();
	Utils::ConsolePrint("Iniciando...\n\n", 3);
	Interfaces::Initialize();
	Interfaces::Dump();

	NetvarSys::Get().Initialize();

	hooks.Hook();

	for (;true;) {
		if (GetAsyncKeyState(VK_F1) & 0x1) {
			aimAssistActive = !aimAssistActive;
			if (aimAssistActive) {
				violentAim = false;
			}
			Utils::ConsolePrint("AimAssist: %d\n", aimAssistActive);
			Utils::ConsolePrint("ViolentAim: %d\n\n", violentAim);
		}

		if (GetAsyncKeyState(VK_F2) & 0x1) {
			violentAim = !violentAim;
			if (violentAim) {
				aimAssistActive = false;
			}
			Utils::ConsolePrint("AimAssist: %d\n", aimAssistActive);
			Utils::ConsolePrint("ViolentAim: %d\n\n", violentAim);
		}
		if (GetAsyncKeyState(VK_F3) & 0x3) {
			triggerBot = !triggerBot;
			if (triggerBot) {
				violentAim = false;
			}
			Utils::ConsolePrint("TriggerBot: %d\n", triggerBot);
		}
	}

	return 0;
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL,  DWORD fdwReason, LPVOID lpReserved) 
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, startHack(), 0, 0, 0);
		break;
	}
}
