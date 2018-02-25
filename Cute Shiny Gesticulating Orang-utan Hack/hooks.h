#pragma once
#include "vmtBase.h"
#include "valve_sdk\sdk.hpp"
#include "helpers\utils.hpp"
#include "valve_sdk\csgostructs.hpp"
#include "RCS.h"
#include "AimAssist.h"
#include "glowESP.h"

typedef void(__stdcall* createMoveFn)(float, CUserCmd*);
typedef void(__stdcall* paintTraverseFn)(vgui::VPANEL, bool, bool);

createMoveFn _CreateMove;
paintTraverseFn _PaintTraverse;

void __stdcall hookedPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
{
	static auto panelId = vgui::VPANEL{ 0 };

	_PaintTraverse(panel, forceRepaint, allowForce);

	if (!panelId) {
		const auto panelName = g_VGuiPanel->GetName(panel);
		Utils::ConsolePrint("%s\n", panelName);
		if (!strcmp(panelName, "FocusOverlayPanel")) {
			panelId = panel;
		}
	}
	else if (panelId == panel) {
		if (g_EngineClient->IsInGame() && !g_EngineClient->IsTakingScreenshot()) {

			if (!g_LocalPlayer)
				return;
			/*do things here*/
		}
	}
}
bool violentAim = false;
bool triggerBot = false;
bool __stdcall HookedCreateMove(float SampleTime, CUserCmd* UserCmd)
	{
		if (!UserCmd->command_number)
			return true;


		/* code goes here */
		QAngle oldPunchAngle, viewAngles;

		if (g_EngineClient->IsInGame() && g_EngineClient->IsConnected() && g_LocalPlayer->IsAlive())
		{
			if (aimAssistActive) {
				AimAssist::aimAssist(UserCmd);
			}
			if (violentAim) {
				AimAssist::violentAim(UserCmd);
			}
			Glow::glowESP(UserCmd);
			AimAssist::triggerbot(UserCmd);
		}

		return false;
	}
class Hooks {
public:
	void Hook() {
		void** BaseClientDllVMT = *(void***)g_CHLClient;
		IClientMode** ClientMode = *(IClientMode***)((DWORD)BaseClientDllVMT[10] + 5);
		CVMTHookManager* createMoveHook = new CVMTHookManager(*(DWORD***)ClientMode);
		_CreateMove = (createMoveFn)createMoveHook->HookFunction(24, HookedCreateMove);
		createMoveHook->HookTable(true);
		/*
		Utils::ConsolePrint("Hooking paint traverse");
		void** BaseGuiPanelDllVMT = *(void***)g_VGuiPanel;
		CVMTHookManager* paintTraverseMoveHook = new CVMTHookManager(g_VGuiPanel);
		_PaintTraverse = (paintTraverseFn)paintTraverseMoveHook->HookFunction(41, hookedPaintTraverse);
		paintTraverseMoveHook->HookTable(true);
		*/
	}

};

Hooks hooks;