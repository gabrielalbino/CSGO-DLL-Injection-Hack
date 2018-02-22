#pragma once
#include "valve_sdk\csgostructs.hpp"
#include "valve_sdk\sdk.hpp"
#include "helpers\math.hpp"
#include <math.h>

#define sniperFov 30.f
#define defaultFov 20.f

namespace AimAssist{
	float closestPlayerDistance, minimumFov;

	QAngle getPlayerAimPoint(CUserCmd* UserCmd, C_BasePlayer* entity) {
		QAngle ang = UserCmd->viewangles;
		QAngle playerPoint;
		if (g_LocalPlayer->m_hActiveWeapon()->IsSniper() || g_LocalPlayer->m_hActiveWeapon()->IsPistol()) {
			playerPoint = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetBonePos(6));
		}
		else {
			playerPoint = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetBonePos(37));
		}
		return playerPoint;
	}

	double DegToRad(double deg) {
		return deg*0.01745329251;
	}

	float AngleDifference(QAngle ViewAngles, QAngle TargetAngles, float Distance)
	{
		float pitch = sin(DegToRad(ViewAngles.pitch - TargetAngles.pitch)) * Distance;
		float yaw = sin(DegToRad(ViewAngles.yaw - TargetAngles.yaw)) * Distance;

		return sqrt(powf(pitch, 2.0) + powf(yaw, 2.0));
	}

	int getClosestPlayer(CUserCmd* UserCmd, float* menorDistancia) {
		int contador;
		*menorDistancia = 1000;
		int index = -1;
		float distancia;
		for (contador = 0; contador < g_EntityList->NumberOfEntities(true); contador++) {
			auto entity = C_BasePlayer::GetPlayerByIndex(contador);
			if(!entity || entity == g_LocalPlayer || entity->IsDormant() || !entity->IsAlive() || (entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())){
				continue;
			}
			else {
				distancia = AngleDifference(UserCmd->viewangles, getPlayerAimPoint(UserCmd, entity), sqrt(pow(g_LocalPlayer->m_vecOrigin().x - entity->m_vecOrigin().x, 2) + pow(g_LocalPlayer->m_vecOrigin().y - entity->m_vecOrigin().y, 2) + pow(g_LocalPlayer->m_vecOrigin().z - entity->m_vecOrigin().z, 2)));
				if (distancia < *menorDistancia) {
					*menorDistancia = distancia;
					index = contador;
				}
			}
		}
		return index;
	}

	void aimAssist(CUserCmd* UserCmd) {
		int closestPlayerIndex;
		float smoothing = 4.0f;
		g_LocalPlayer->m_hActiveWeapon()->IsSniper() ? minimumFov = sniperFov : minimumFov = defaultFov;
		closestPlayerIndex = getClosestPlayer(UserCmd, &closestPlayerDistance);
		if ((GetAsyncKeyState(VK_MENU) & 0X8000 || (UserCmd->buttons & IN_ATTACK))) {
			if (closestPlayerIndex != -1 && closestPlayerDistance < minimumFov) {
				QAngle angles = getPlayerAimPoint(UserCmd, C_BasePlayer::GetPlayerByIndex(closestPlayerIndex));
				if (C_BasePlayer::GetPlayerByIndex(closestPlayerIndex)->m_iTeamNum() == g_LocalPlayer->m_iTeamNum()) {
					return;
				}
				Math::NormalizeAngles(angles);
				Math::ClampAngles(angles);
				if (!g_LocalPlayer->m_hActiveWeapon()->IsSniper()) {
					QAngle playerAngles = UserCmd->viewangles;
					float pitch = angles.pitch - playerAngles.pitch;
					float yaw = angles.yaw - playerAngles.yaw;
					yaw /= smoothing;
					pitch /= smoothing;
					QAngle toSet;
					toSet.pitch = playerAngles.pitch + pitch;
					toSet.yaw = playerAngles.yaw + yaw;
					toSet.roll = 0;
					g_EngineClient->SetViewAngles(toSet);
				}
				else {
					g_EngineClient->SetViewAngles(angles);
				}
			}
		}
	}
}