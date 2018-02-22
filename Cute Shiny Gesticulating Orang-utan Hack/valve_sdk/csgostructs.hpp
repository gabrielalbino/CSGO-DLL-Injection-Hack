#pragma once

#include "sdk.hpp"

#define NETVAR(type, name, table, netvar)                           \
    type& name() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return *(type*)((uintptr_t)this + _##name);                 \
    }

#define PNETVAR(type, name, table, netvar)                           \
    type* name() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return (type*)((uintptr_t)this + _##name);                 \
    }

enum CSWeaponType
{
    WEAPONTYPE_KNIFE = 0,
    WEAPONTYPE_PISTOL,
    WEAPONTYPE_SUBMACHINEGUN,
    WEAPONTYPE_RIFLE,
    WEAPONTYPE_SHOTGUN,
    WEAPONTYPE_SNIPER_RIFLE,
    WEAPONTYPE_MACHINEGUN,
    WEAPONTYPE_C4,
    WEAPONTYPE_PLACEHOLDER,
    WEAPONTYPE_GRENADE,
    WEAPONTYPE_UNKNOWN
};

class C_BaseEntity;

// Created with ReClass.NET by KN4CK3R
class CHudTexture
{
public:
    char szShortName[64];    //0x0000
    char szTextureFile[64];  //0x0040
    bool bRenderUsingFont;   //0x0080
    bool bPrecached;         //0x0081
    int8_t cCharacterInFont; //0x0082
    uint8_t pad_0083[1];     //0x0083
    uint32_t hFont;          //0x0084
    int32_t iTextureId;      //0x0088
    float afTexCoords[4];    //0x008C
    uint8_t pad_009C[16];    //0x009C
};

#pragma pack(push, 1)
class CCSWeaponInfo
{
public:
  void*   vftbl;
  char*   consoleName;
  char    pad_0008[12];
  int	    iMaxClip1;
  int	    iMaxClip2;
  int	    iDefaultClip1;
  int	    iDefaultClip2;
  char    pad_0024[8];
  char*   szWorldModel;
  char*   szViewModel;
  char*   szDroppedModel;
  char    pad_0038[4];
  char*   N0000023E;
  char    pad_0040[56];
  char*   szEmptySound;
  char    pad_007C[4];
  char*   szBulletType;
  char    pad_0084[4];
  char*   szHudName;
  char*   szWeaponName;
  char    pad_0090[56];
  int     WeaponType;
  int	    iWeaponPrice;
  int	    iKillAward;
  char*   szAnimationPrefix;
  float   flCycleTime;
  float   flCycleTimeAlt;
  float   flTimeToIdle;
  float   flIdleInterval;
  bool    bFullAuto;
  char    pad_0x00E5[3];
  int	    iDamage;
  float   flArmorRatio;
  int	    iBullets;
  float   flPenetration;
  float   flFlinchVelocityModifierLarge;
  float   flFlinchVelocityModifierSmall;
  float   flRange;
  float   flRangeModifier;
  float   flThrowVelocity;
  char    pad_0x010C[12];
  bool    bHasSilencer;
  char    pad_0x0119[3];
  char*   pSilencerModel;
  int	    iCrosshairMinDistance;
  int	    iCrosshairDeltaDistance;
  float   flMaxPlayerSpeed;
  float   flMaxPlayerSpeedAlt;
  float   flSpread;
  float   flSpreadAlt;
  float   flInaccuracyCrouch;
  float   flInaccuracyCrouchAlt;
  float   flInaccuracyStand;
  float   flInaccuracyStandAlt;
  float   flInaccuracyJumpInitial;
  float   flInaccuracyJump;
  float   flInaccuracyJumpAlt;
  float   flInaccuracyLand;
  float   flInaccuracyLandAlt;
  float   flInaccuracyLadder;
  float   flInaccuracyLadderAlt;
  float   flInaccuracyFire;
  float   flInaccuracyFireAlt;
  float   flInaccuracyMove;
  float   flInaccuracyMoveAlt;
  float   flInaccuracyReload;
  int	    iRecoilSeed;
  float   flRecoilAngle;
  float   flRecoilAngleAlt;
  float   flRecoilAngleVariance;
  float   flRecoilAngleVarianceAlt;
  float   flRecoilMagnitude;
  float   flRecoilMagnitudeAlt;
  float   flRecoilMagnitudeVariance;
  float   flRecoilMagnitudeVarianceAlt;
  float   flRecoveryTimeCrouch;
  float   flRecoveryTimeStand;
  float   flRecoveryTimeCrouchFinal;
  float   flRecoveryTimeStandFinal;
  int	    iRecoveryTransitionStartBullet;
  int	    iRecoveryTransitionEndBullet;
  bool    bUnzoomAfterShot;
  bool    bHideViewModelZoomed;
  char    pad_0x01B5[2];
  char    iZoomLevels[3];
  int	    iZoomFOV[2];
  float   fZoomTime[3];
  char*   szWeaponClass;
  float   flAddonScale;
  char    pad_0x01DC[4];
  char*   szEjectBrassEffect;
  char*   szTracerEffect;
  int	    iTracerFrequency;
  int	    iTracerFrequencyAlt;
  char*   szMuzzleFlashEffect_1stPerson;
  char    pad_0x01F4[4];
  char*   szMuzzleFlashEffect_3rdPerson;
  char    pad_0x01FC[4];
  char*   szMuzzleSmokeEffect;
  float   flHeatPerShot;
  char*   szZoomInSound;
  char*   szZoomOutSound;
  float   flInaccuracyPitchShift;
  float   flInaccuracySoundThreshold;
  float   flBotAudibleRange;
  char    pad_0x0218[8];
  char*   pWrongTeamMsg;
  bool    bHasBurstMode;
  char    pad_0x0225[3];
  bool    bIsRevolver;
  bool    bCannotShootUnderwater;
};
#pragma pack(pop)

class C_EconItemView
{
public:
    NETVAR(int32_t, m_bInitialized,          "DT_BaseAttributableItem", "m_bInitialized");
    NETVAR(int32_t, m_iItemDefinitionIndex,  "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
    NETVAR(int32_t, m_iEntityLevel,          "DT_BaseAttributableItem", "m_iEntityLevel");
    NETVAR(int32_t, m_iAccountID,            "DT_BaseAttributableItem", "m_iAccountID");
    NETVAR(int32_t, m_iItemIDLow,            "DT_BaseAttributableItem", "m_iItemIDLow");
    NETVAR(int32_t, m_iItemIDHigh,           "DT_BaseAttributableItem", "m_iItemIDHigh");
    NETVAR(int32_t, m_iEntityQuality,        "DT_BaseAttributableItem", "m_iEntityQuality");
};

class C_BaseEntity : public IClientEntity
{
public:
    static __forceinline C_BaseEntity* GetEntityByIndex(int index)
    {
        return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
    }
    static __forceinline C_BaseEntity* get_entity_from_handle(CBaseHandle h)
    {
        return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntityFromHandle(h));
    }

    NETVAR(int32_t,               m_nModelIndex,    "DT_BaseEntity",  "m_nModelIndex");
    NETVAR(int32_t,               m_iTeamNum,       "DT_BaseEntity",  "m_iTeamNum");
    NETVAR(Vector,                m_vecOrigin,      "DT_BaseEntity",  "m_vecOrigin");
    NETVAR(bool,                  m_bShouldGlow,    "DT_DynamicProp", "m_bShouldGlow");
    NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity,   "DT_BaseEntity",  "m_hOwnerEntity");
    
    const matrix3x4_t& m_rgflCoordinateFrame()
    {
        static auto _m_rgflCoordinateFrame = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
        return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
    }

    bool IsPlayer();
    bool IsWeapon();
    bool IsPlantedC4();
    bool IsDefuseKit();
};

class C_PlantedC4
{
public:
    NETVAR(bool,                  m_bBombTicking,      "DT_PlantedC4", "m_bBombTicking");
    NETVAR(bool,                  m_bBombDefused,      "DT_PlantedC4", "m_bBombDefused");
    NETVAR(float,                 m_flC4Blow,          "DT_PlantedC4", "m_flC4Blow");
    NETVAR(float,                 m_flTimerLength,     "DT_PlantedC4", "m_flTimerLength");
    NETVAR(float,                 m_flDefuseLength,    "DT_PlantedC4", "m_flDefuseLength");
    NETVAR(float,                 m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");
    NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser,      "DT_PlantedC4", "m_hBombDefuser");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
    NETVAR(uint64_t, m_OriginalOwnerXuid,    "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidLow,  "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
    NETVAR(int32_t, m_nFallbackStatTrak,     "DT_BaseAttributableItem", "m_nFallbackStatTrak");
    NETVAR(int32_t, m_nFallbackPaintKit,     "DT_BaseAttributableItem", "m_nFallbackPaintKit");
    NETVAR(int32_t, m_nFallbackSeed,         "DT_BaseAttributableItem", "m_nFallbackSeed");
    NETVAR(float_t, m_flFallbackWear,        "DT_BaseAttributableItem", "m_flFallbackWear");

    C_EconItemView& m_Item()
    {
        // Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
        // A real fix for this requires a rewrite of the netvar manager
        return *(C_EconItemView*)this;
    }
};


class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
    NETVAR(float_t, m_flNextPrimaryAttack,   "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
    NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
    NETVAR(int32_t, m_iClip1,                "DT_BaseCombatWeapon", "m_iClip1");
    NETVAR(int32_t, m_iClip2,                "DT_BaseCombatWeapon", "m_iClip2");
    NETVAR(float_t, m_flRecoilIndex,         "DT_WeaponCSBase", "m_flRecoilIndex");

    CCSWeaponInfo* GetCSWeaponData();
    bool HasBullets();
    bool CanFire();
    bool IsGrenade();
    bool IsKnife();
    bool IsReloading();
	bool IsRifle();
	bool IsPistol();
	bool IsSniper();
    float GetInaccuracy();
    float GetSpread();
    void UpdateAccuracyPenalty();

};

class C_BasePlayer : public C_BaseEntity
{
public:
    static __forceinline C_BasePlayer* GetPlayerByUserId(int id)
    {
        return static_cast<C_BasePlayer*>(GetEntityByIndex(g_EngineClient->GetPlayerForUserID(id)));
    }
    static __forceinline C_BasePlayer* GetPlayerByIndex(int i)
    {
        return static_cast<C_BasePlayer*>(GetEntityByIndex(i));
    }

    NETVAR(bool,                             m_bHasDefuser,          "DT_CSPlayer", "m_bHasDefuser");
    NETVAR(bool,                             m_bGunGameImmunity,     "DT_CSPlayer", "m_bGunGameImmunity");
    NETVAR(int32_t,                          m_iShotsFired,          "DT_CSPlayer", "m_iShotsFired");
    NETVAR(QAngle,                           m_angEyeAngles,         "DT_CSPlayer", "m_angEyeAngles[0]");
    NETVAR(int,                              m_ArmorValue,           "DT_CSPlayer", "m_ArmorValue");
    NETVAR(bool,                             m_bHasHelmet,           "DT_CSPlayer", "m_bHasHelmet");
    NETVAR(bool,                             m_bIsScoped,            "DT_CSPlayer", "m_bIsScoped");;
    NETVAR(float,                            m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
    NETVAR(float,			     m_flFlashDuration,      "DT_CSPlayer", "m_flFlashDuration");
    NETVAR(int32_t,                          m_iHealth,              "DT_BasePlayer", "m_iHealth");
    NETVAR(int32_t,                          m_lifeState,            "DT_BasePlayer", "m_lifeState");
    NETVAR(int32_t,                          m_fFlags,               "DT_BasePlayer", "m_fFlags");
    NETVAR(int32_t,                          m_nTickBase,            "DT_BasePlayer", "m_nTickBase");
    NETVAR(Vector,                           m_vecViewOffset,        "DT_BasePlayer", "m_vecViewOffset[0]");
    NETVAR(QAngle,                           m_viewPunchAngle,       "DT_BasePlayer", "m_viewPunchAngle");
    NETVAR(QAngle,                           m_aimPunchAngle,        "DT_BasePlayer", "m_aimPunchAngle");
    NETVAR(CHandle<C_BaseViewModel>,         m_hViewModel,           "DT_BasePlayer", "m_hViewModel[0]");
    NETVAR(Vector,                           m_vecVelocity,          "DT_BasePlayer", "m_vecVelocity[0]");
    NETVAR(float,                            m_flMaxspeed,           "DT_BasePlayer", "m_flMaxspeed");
    NETVAR(CHandle<C_BasePlayer>,            m_hObserverTarget,      "DT_BasePlayer", "m_hObserverTarget");
    NETVAR(CHandle<C_BaseCombatWeapon>,      m_hActiveWeapon,        "DT_BaseCombatCharacter", "m_hActiveWeapon");
    PNETVAR(CHandle<C_BaseCombatWeapon>,     m_hMyWeapons,           "DT_BaseCombatCharacter", "m_hMyWeapons");
    PNETVAR(CHandle<C_BaseAttributableItem>, m_hMyWearables,         "DT_BaseCombatCharacter", "m_hMyWearables");
    
    CUserCmd*& m_pCurrentCommand();

    Vector        GetEyePos();
    player_info_t GetPlayerInfo();
    bool          IsAlive();
    bool          HasC4();
    Vector        GetHitboxPos(int hitbox_id);
    bool          GetHitboxPos(int hitbox, Vector &output);
    Vector        GetBonePos(int bone);
    bool          CanSeePlayer(C_BasePlayer* player, int hitbox);
    bool          CanSeePlayer(C_BasePlayer* player, const Vector& pos);
};

class C_BaseViewModel : public C_BaseEntity
{
public:
    NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
    NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
    NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
    NETVAR(CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
};
