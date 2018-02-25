#pragma once
#include "valve_sdk\csgostructs.hpp"
#include "valve_sdk\sdk.hpp"
#include "helpers\math.hpp"
#include "AimAssist.h"
#include <math.h>
Color* color;
namespace Glow {
	void glowESP(CUserCmd* userCmd) {
		for (int contador = 0; contador < g_GlowObjManager->m_GlowObjectDefinitions.Count(); contador++) {
			auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[contador];
			auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

			if (!entity || entity->IsDormant())
				continue;

			if (glowObject.IsUnused())
				continue;

			if (entity->GetClientClass()->m_ClassID == ClassId_CCSPlayer) {
				entity->m_bShouldGlow() = true;
				if (entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum()) {
					glowObject.m_flRed = 1.f;
					glowObject.m_flGreen =0.f;
					glowObject.m_flBlue = 0.f;
					glowObject.m_flAlpha = 1.f;
					glowObject.m_bRenderWhenOccluded = true;
					glowObject.m_bRenderWhenUnoccluded = false;
				}
				else if (entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum()) {
					glowObject.m_flRed = 0.f;
					glowObject.m_flGreen = 0.f;
					glowObject.m_flBlue = 1.f;
					glowObject.m_flAlpha = 1.f;
					glowObject.m_bRenderWhenOccluded = true;
					glowObject.m_bRenderWhenUnoccluded = false;
				}
			}
		}
	}
	
}