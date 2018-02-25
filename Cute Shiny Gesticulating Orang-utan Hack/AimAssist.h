#pragma once
#include "valve_sdk\csgostructs.hpp"
#include "valve_sdk\sdk.hpp"
#include "helpers\math.hpp"
#include <math.h>
#include "helpers\utils.hpp"

#define defaultFov 50.f

C_BaseCombatWeapon* activeWeapon;
QAngle ViewAngles(0, 0, 0);
float AimFOV, BestDist;
int BestTarget, bone;
C_BasePlayer* BestEntity;

namespace AimAssist{
	static QAngle vOldPunch = { 0.0f, 0.0f, 0.0f };
	float GetFOVDist(QAngle ViewAngles, Vector EyePos, Vector Target)
	{
		float FOV = Math::GetFOV(ViewAngles, EyePos, Target);

		FOV = FOV + ((Target.DistTo(EyePos) * 0.002f) * FOV);

		return FOV;
	}

	bool CanSeePlayer(C_BasePlayer* player, int bone)
	{
		CGameTrace tr;
		Ray_t ray;
		CTraceFilter filter;
		filter.pSkip = g_LocalPlayer;
		auto endpos = player->GetBonePos(bone);
		ray.Init(g_LocalPlayer->GetEyePos(), endpos);
		g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);
		return tr.hit_entity == player || tr.fraction > 0.97f;
	}

	void inline SinCos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}

	enum
	{
		PITCH = 0,	// up / down
		YAW,		// left / right
		ROLL		// fall over
	};

	void AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
		SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}
	C_BaseEntity* getTargetUnderCrosshair(CUserCmd* cmd, bool enemiesOnly, bool visibleOnly, trace_t* trace)
	{
		Utils::ConsolePrint("Getting target\n");
		Vector vEnd;
		auto view = cmd->viewangles;

		trace_t tr;
		Ray_t ray;

		CTraceFilter filter;
		filter.pSkip = g_LocalPlayer;

		AngleVectors(view, &vEnd, nullptr, nullptr);

		vEnd = vEnd * 8192.0f + g_LocalPlayer->GetEyePos();
		Utils::ConsolePrint("vEnd = %f %f %f\n", vEnd.x, vEnd.y, vEnd.z);
		ray.Init(g_LocalPlayer->GetEyePos(), vEnd);
		g_EngineTrace->TraceRay(ray, CONTENTS_SOLID | CONTENTS_GRATE | CONTENTS_HITBOX, &filter, &tr);
		Utils::ConsolePrint("tr.Hit_Entity: %p\n", tr.hit_entity);
		if (!tr.hit_entity) {
			return nullptr;
		}
		Utils::ConsolePrint("Enemy on xhair!\n", tr.hit_entity);
		if (tr.hit_entity->GetClientClass()->m_ClassID == ClassId_CCSPlayer) {
			Utils::ConsolePrint("Is CCSPlayer!\n", tr.hit_entity);
			if (tr.hit_entity->GetBaseEntity()->m_iTeamNum() == g_LocalPlayer->m_iTeamNum() && enemiesOnly)
				return nullptr;
			if (tr.hitgroup <= 7 && tr.hitgroup > 0)
			{
				if (trace) {
					trace->endpos = tr.endpos;
					trace->hitgroup = tr.hitgroup;
				}
				return (C_BasePlayer*)tr.hit_entity;
			}
			else
				return nullptr;
		}
		Utils::ConsolePrint("Is not CCSPlayer!\n", tr.hit_entity);
	}

	void triggerbot(CUserCmd* cmd)
	{

		if (GetAsyncKeyState(VK_MENU) & 0x8000) {
			Utils::ConsolePrint("Calling triggerbot");
			C_BaseEntity* target = nullptr;
			auto bShoot = false;
			auto localPlayerWep = g_LocalPlayer->m_hActiveWeapon();
			if (!localPlayerWep)return;
			auto maxdist = localPlayerWep->GetCSWeaponData()->flRange;//as far as a bullet can travel

			trace_t tr;
			if ((target = getTargetUnderCrosshair(cmd, false, true, &tr)))
			{
				Utils::ConsolePrint("Shooting target!\n", tr.hit_entity);
				auto vecToTarget = tr.endpos - g_LocalPlayer->GetEyePos();
				if (tr.hitgroup > 0 && tr.hitgroup <= 3)
					bShoot = true;

				if (vecToTarget.Length() < maxdist && bShoot)
				{
					static auto bFlip = false;

					if (localPlayerWep->m_flNextPrimaryAttack() < g_GlobalVars->curtime)
					{
						cmd->buttons |= IN_ATTACK;
					}
					else
					{
						cmd->buttons &= ~IN_ATTACK;
					}
				}
			}
		}
	}

	void AimAtTarget(CUserCmd* UserCmd) {
		if (CanSeePlayer(BestEntity, 8)) {
			Vector BonePos;
			float SmoothStrength = 0.005f;
			Vector SmoothBone = BestEntity->GetBonePos(bone);
			Vector AccurateBone = BestEntity->GetBonePos(bone);
			SmoothBone = SmoothBone * (1 - SmoothStrength) + AccurateBone * SmoothStrength;
			BonePos = SmoothBone;

			QAngle AimAngles = Math::CalcAngle(g_LocalPlayer->GetEyePos(), BonePos);

			Math::NormalizeAngles(AimAngles);
			Math::ClampAngles(AimAngles);

			QAngle Angles = UserCmd->viewangles;
			Angles += g_LocalPlayer->m_viewPunchAngle() * 2.f;
			QAngle Delta = AimAngles - Angles;
			Math::NormalizeAngles(Delta);

			if (!Delta.IsZero()) {
				Delta /= 1.3;
				AimAngles = Angles + Delta;
				Math::NormalizeAngles(AimAngles);
			}

			Math::ClampAngles(AimAngles);
			UserCmd->viewangles = AimAngles;
			g_EngineClient->SetViewAngles(UserCmd->viewangles);
		}
	}

	void FindTarget() {
		float BestDist = AimFOV;
		for (int i = 0; i < 64; i++) {
			auto Entity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));
			if (!Entity)
				continue;
			if (Entity->IsDormant() || !Entity->IsAlive() || Entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum()) {
				continue;
			}
			if (g_LocalPlayer->GetEyePos() == Entity->GetEyePos()) {
				continue;
			}
			float FOV = GetFOVDist(ViewAngles, g_LocalPlayer->GetEyePos(), Entity->GetBonePos(bone));

			if (FOV <= BestDist)
			{
				BestDist = FOV;
				BestTarget = i;
				BestEntity = Entity;
			}
		}
	}

	void FindTargetViolent() {
		float BestDist = AimFOV;
		for (int i = 0; i < 64; i++) {
			auto Entity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));
			if (!Entity)
				continue;
			if (Entity->GetClientClass()->m_ClassID != ClassId_CCSPlayer) {
				continue;
			}
			if (Entity->IsDormant() || !Entity->IsAlive() || Entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum()) {
				continue;
			}
			if (g_LocalPlayer->GetEyePos() == Entity->GetEyePos()) {
				continue;
			}
			if (CanSeePlayer(Entity, 8)) {
				float FOV = GetFOVDist(ViewAngles, g_LocalPlayer->GetEyePos(), Entity->GetBonePos(8));
				BestDist = FOV;
				BestTarget = i;
				BestEntity = Entity;
			}
		}
	}
	

	void AimAtTargetViolent(CUserCmd* UserCmd) {
		float SmoothStrength = 0.005f;
		Vector BonePos = BestEntity->GetBonePos(bone);

		QAngle AimAngles = Math::CalcAngle(g_LocalPlayer->GetEyePos(), BonePos);
		AimAngles += g_LocalPlayer->m_viewPunchAngle() * 2.f;
		Math::NormalizeAngles(AimAngles);
		Math::ClampAngles(AimAngles);

		Math::ClampAngles(AimAngles);
		UserCmd->viewangles = AimAngles;
		if(CanSeePlayer(BestEntity, 8))
			UserCmd->buttons |= IN_ATTACK;
	}

	void violentAim(CUserCmd* UserCmd) {
		BestTarget = -1;
		BestEntity = nullptr;
		ViewAngles = UserCmd->viewangles;
		activeWeapon = g_LocalPlayer->m_hActiveWeapon();
		if (!activeWeapon->CanFire() || !activeWeapon->HasBullets()) {
			return;
		}
		bone = 8;
		FindTargetViolent();
		if (BestTarget != -1)
			AimAtTargetViolent(UserCmd);

	}

	void aimAssist(CUserCmd* UserCmd) {
		BestTarget = -1;
		BestEntity = nullptr;
		ViewAngles = UserCmd->viewangles;
		AimFOV = defaultFov;
		Math::NormalizeAngles(ViewAngles);
		activeWeapon = g_LocalPlayer->m_hActiveWeapon();
		if (!activeWeapon->CanFire() || !activeWeapon->HasBullets()) {
			return;
		}
		if (activeWeapon->IsRifle() || activeWeapon->IsPistol()) {
			bone = 8;
		}
		else {
			bone = 6;
		}
		AimFOV = AimFOV + ((g_LocalPlayer->m_aimPunchAngle().Length() * 0.1f) * AimFOV);
		FindTarget();
		if (BestTarget != -1)
			AimAtTarget(UserCmd);

	}

}