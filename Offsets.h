#pragma once
#include <Windows.h>
#include <string>


namespace Offset
{
    inline DWORD64 dwEntityList;
    inline DWORD64 dwViewMatrix;
    inline DWORD64 dwLocalPlayerController;
    inline DWORD64 dwLocalPlayerPawn;
    inline DWORD64 dwGameRules;
    inline DWORD64 ReturnAddress;

    struct
    {
        DWORD m_bPawnIsAlive;
        DWORD m_iszPlayerName;
        DWORD m_hPlayerPawn;
        DWORD m_iHealth;
        DWORD m_iTeamNum;
        DWORD m_pGameSceneNode;
        DWORD m_pBoneArray;


    }CCSPlayerController;

    struct
    {
        DWORD m_vOldOrigin;
        DWORD m_pClippingWeapon;
        DWORD m_ArmorValue;
    

    }C_CSPlayerPawnBase;


    struct
    {
        DWORD GetHighestEntityIndex = 0x1510;

    }dwGameEntitySystem;

}




