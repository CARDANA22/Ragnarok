#pragma once

namespace Offsets {
    const unsigned long LocalPlayer = 0x01ee8b70 + 0x8; // [Miscellaneous] -> LocalPlayer probably also updated
    const unsigned long LevelName = 0x1462460; // [Miscellaneous] -> LevelName
    const unsigned long cl_entitylist = 0x1b37938; // [Miscellaneous] -> cl_entitylist
    
    const unsigned long lastVisibleTime = 0x1a70; // [Miscellaneous] -> CPlayer!lastVisibleTime probably also updated

    const unsigned long in_attack = 0x076780e8; // [Buttons] -> in_attack
    const unsigned long in_zoom = 0x0bc9f770; // [Buttons] -> in_zoom

    const unsigned long m_localOrigin = 0x0054; // [RecvTable.DT_BaseEntity] -> m_localOrigin
    const unsigned long m_viewAngles = 0x25ac - 0x14; // [DataMap.C_Player] -> m_ammoPoolCapacity - 0x14
    const unsigned long m_frameYaw = 0x22b4 - 0x8; // [DataMap.C_Player] -> m_currentFramePlayer.m_ammoPoolCount - 0x8
    const unsigned long m_vecPunchWeapon_Angle = 0x24b0; // [DataMap.C_Player] -> m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
    const unsigned long camera_origin = 0x1f48; // [Miscellaneous] -> CPlayer!camera_origin

    const unsigned long bones = 0x0f50;  // m_bConstrainBetweenEndpoints
   
    const unsigned long m_iName = 0x0589; // [RecvTable.DT_BaseEntity] -> m_iName
    const unsigned long m_lifeState = 0x0798; // [RecvTable.DT_Player] -> m_lifeState
    const unsigned long m_bleedoutState = 0x2740; // [RecvTable.DT_Player] -> m_bleedoutState
    const unsigned long m_iHealth = 0x043c; // [RecvTable.DT_Player] -> m_iHealth
    const unsigned long m_shieldHealth = 0x0170; // [RecvTable.DT_BaseEntity] -> m_shieldHealth
    const unsigned long m_shieldHealthMax = m_shieldHealth + 0x4; // [RecvTable.DT_BaseEntity] -> m_shieldHealth + 0x4
    const unsigned long m_iTeamNum = 0x044c; // [RecvTable.DT_BaseEntity] -> m_iTeamNum
    const unsigned long m_iSignifierName	= 0x0580; // [RecvTable.DT_BaseEntity]  -> m_iSignifierName

    const unsigned long glowEnable = 0x03c0 + 0x8; // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x8
    const unsigned long glowThroughWall = 0x03c0 + 0x10; // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x10
    const unsigned long glowColor = 0x01b8 + 0x18; // [RecvTable.DT_HighlightSettings] -> m_highlightParams + 0x18
    const unsigned long glowDistance = 0x3b4;        // m_highlightServerFadeEndTimes + 52(0x34)

    const unsigned long m_customScriptInt = 0x1648; // [RecvTable.DT_PropSurvival] -> m_customScriptInt

    const unsigned long GlobalVars=0x1461fa0; //GlobalVars
}