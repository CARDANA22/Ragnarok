//season 15 offset update
// 15/11/22
#include <cmath>
#pragma once

struct bone_t
{
	char pad[0xCC];
	float x;
	char pad2[0xC];
	float y;
	char pad3[0xC];
	float z;
};

struct Vector3 {
    float x, y, z;

    inline static float distance(Vector3 lhs, Vector3 rhs) {
        Vector3 delta = Vector3{ rhs.x - lhs.x, rhs.y - lhs.y, rhs.z - lhs.z };
        return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    }

    inline Vector3 operator+ (Vector3 v) const {
        return Vector3{ x + v.x, y + v.y, z + v.z };
    }

    inline Vector3 operator- (Vector3 v) const {
        return Vector3{ x - v.x, y - v.y, z - v.z };
    }

    inline Vector3 operator* (float scale) const {
        return Vector3{ x * scale, y * scale, z * scale };
    }

    inline bool operator== (Vector3 v) const {
        float xx = v.x;
        float yy = v.y;
        float zz = v.z;
        return x ==xx && y == yy && z == zz;
    }

    void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}
};

struct Vector2 {
    double x, y;

    inline static double distance(Vector2 lhs, Vector2 rhs) {
        Vector2 delta = Vector2{ rhs.x - lhs.x, rhs.y - lhs.y };
        return sqrt(delta.x * delta.x + delta.y * delta.y);
    }

    inline Vector2 operator+ (Vector2 v) const {
        return Vector2{ x + v.x, y + v.y };
    }

    inline Vector2 operator- (Vector2 v) const {
        return Vector2{ x - v.x, y - v.y};
    }

    inline Vector2 operator* (float scale) const {
        return Vector2{ x * scale, y * scale};
    }

    void Normalize()
	{
		while (x > 89.0f)
			x -= 180.f;

		while (x < -89.0f)
			x += 180.f;

		while (y > 180.f)
			y -= 360.f;

		while (y < -180.f)
			y += 360.f;
	}
};

namespace offsets
{
    // buttons
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long IN_ATTACK = 0x07678368; // [Buttons] -> in_attack
    // core
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long REGION = 0x140000000;     // [Mine]          -> Region
    const long LEVEL = 0x1462450;        // [Miscellaneous] -> LevelName
    const long LOCAL_PLAYER = 0x01ee8d50 + 0x8; // [Miscellaneous] -> LocalPlayer AVC_GameMovement+0x8
    const long ENTITY_LIST = 0x1b37b18;  // [Miscellaneous] -> cl_entitylist
    // entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long LOCAL_ORIGIN = 0x0158; // [DataMap.CBaseViewModel]    -> m_localOrigin
    // player
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const long OFFSET_YAW = 0x22b4 - 0x8;           // m_currentFramePlayer.m_ammoPoolCount - 0x8
    const long GLOW_ENABLE = 0x3C8;                 // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x8
    const long GLOW_THROUGH_WALL = 0x3D0;           // CONTEXT [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x10
    const long OFFSET_GLOW_MODE = (0x02c0 + 0x4);   // m_highlightFunctionBits + 0x4
    const long OFFSET_GLOW_COLOR  = (0x01b8 + 0x18);// m_highlightParams + 0x18
    const long OFFSET_GLOW_DISTANCE = (0x0380 + 0x34);        // m_highlightServerFadeEndTimes + 52(0x34)
    const long OFFSET_ITEM_ID = 0x1648;             // [RecvTable.DT_PropSurvival]      -> m_customScriptInt
    const long OFFSET_SIGNIFIER	= 0x0580;           // [RecvTable.DT_BaseEntity]        -> m_iSignifierName
    const long m_clrRender=0x0050;                  //m_clrRender

    const long TEAM_NUMBER = 0x044c;                // [RecvTable.DT_BaseEntity]        -> m_iTeamNum
    const long NAME = 0x0589;                       // [RecvTable.DT_BaseEntity]        -> m_iName
    const long LIFE_STATE = 0x0798;                 // [RecvTable.DT_Player]            -> m_lifeState
    const long OFFSET_HEALTH = 0x043c;              // [RecvTable.DT_Player]            -> m_iHealth
    const long OFFSET_SHIELD = 0x0170;              // [RecvTable.DT_Player]            -> m_shieldHealth
    const long OFFSET_MAX_SHIELD = 0x0174;          // [RecvTable.DT_Player]            -> m_shieldHealthMax
    const long CameraPos = 0x1f48;                  // CPlayer!camera_origin
    const long VEC_PUNCH_WEAPON_ANGLE = 0x24b0;     // [DataMap.C_Player]               -> m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
    const long VIEW_ANGLE = (0x25ac - 0x14);        // [DataMap.C_Player]               -> m_ammoPoolCapacity - 0x14
    const long BLEEDOUT_STATE = 0x2740;             // [RecvTable.DT_Player]            -> m_bleedoutState
    const long ZOOMING = 0x1c51;                    // [RecvTable.DT_Player]            -> m_bZooming
    const long LAST_VISIBLE_TIME = 0x1A78;          // [Miscellaneous]                  -> CPlayer!lastVisibleTime  1st offset from dump
    const long ABS_VELOCITY=0x0140;                 // C_Player : m_vecAbsVelocity
    const long m_hViewModels=0x2d60 + 0xC;                //m_hViewModels +0xC

    const long OFFSET_CURRENT_WEAPON = 0x1a1c;		// m_latestPrimaryWeapons
    const long OFFSET_BULLET_SPEED =0x1f18;         //CWeaponX!m_flProjectileSpeed // WeaponSettingsMeta.base + WeaponSettings.projectile_launch_speed

    const long bones=0x0f50; //m_bConstrainBetweenEndpoints

    const long GlobalVars=0x1461fa0; //GlobalVars

    const long mp_gamemode=0x01f21c50 + 0x58; //mp_gamemode
}


float DegToRad(float val)
{
    return ((val * M_PI) / 180);
}

Vector3 calcAngle(Vector3 src, Vector3 dst)
{
    Vector3 origin = dst - src;
	float dist = sqrt(origin.x * origin.x + origin.y * origin.y + origin.z * origin.z);
	Vector3 angles = { 0.f, 0.f, 0.f };
	angles.x = -asinf(origin.z / dist) * (180 / M_PI);
	angles.y = atan2f(origin.y, origin.x) * (180 / M_PI);
	return angles;
}

class WeaponX
{
    public:

};