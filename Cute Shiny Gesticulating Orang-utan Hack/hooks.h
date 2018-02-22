#pragma once
#include "vmtBase.h"
#include "valve_sdk\sdk.hpp"
#include "helpers\utils.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "RCS.h"
#include "AimAssist.h"
#include "glowESP.h"
typedef void(__stdcall* createMoveFn)(float, CUserCmd*);

createMoveFn _CreateMove;

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
			if (rcsActive) {
				RCS::controlRecoil(UserCmd);
			}
			Glow::glowESP(UserCmd);
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
	}

};

Hooks hooks;