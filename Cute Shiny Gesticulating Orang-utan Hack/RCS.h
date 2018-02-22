#pragma once
#include "valve_sdk\csgostructs.hpp"
#include "valve_sdk\sdk.hpp"
#include "helpers\math.hpp"
namespace RCS {
	void controlRecoil(CUserCmd* UserCmd) {
			static QAngle vOldPunch = { 0.0f, 0.0f, 0.0f };
			if (g_LocalPlayer->m_iShotsFired() > 1) {
				QAngle vNewPunch = g_LocalPlayer->m_aimPunchAngle();
				vNewPunch *= 2.f;

				vNewPunch -= vOldPunch;
				vNewPunch *= 0.1f;
				vNewPunch += vOldPunch;

				QAngle vFinal = UserCmd->viewangles - (vNewPunch - vOldPunch);

				Math::NormalizeAngles(vFinal);
				Math::ClampAngles(vFinal);
				Utils::ConsolePrint("%f\t%f\n", vFinal.yaw, vFinal.pitch);
				g_EngineClient->SetViewAngles(vFinal);

				vOldPunch = vNewPunch;
			}
			else {
				vOldPunch = { 0.0f, 0.0f, 0.0f };
			}
		}
	
}