#pragma once
#include "valve_sdk\csgostructs.hpp"
#include "valve_sdk\sdk.hpp"
#include "helpers\math.hpp"
#include <math.h>

namespace TriggerBot {
	void TriggerBot(CUserCmd* UserCmd) {
		if (triggerBotActive) {
			if (!g_LocalPlayer->m_hActiveWeapon()->CanFire()) {
				return;
			}
			trace_t tr;

			Vector traceStart = g_LocalPlayer->GetEyePos();
			Vector traceEnd = traceStart + (traceEnd * 1200.f);

			Ray_t ray;
			ray.Init(traceStart, traceEnd);

			CTraceFilter traceFilter;
			traceFilter.pSkip = g_LocalPlayer;

			g_EngineTrace->TraceRay(ray, 0x46004003, &traceFilter, &tr);

		}
	}
}