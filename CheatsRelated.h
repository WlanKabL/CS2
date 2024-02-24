#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <set>
#include "Memory.h"
#include <unordered_set>
#include <iomanip>
#include "algorithm"

BOOL InitializeOffsets()
{
    uintptr_t StartAddress = ClientDLLBase;
    uintptr_t EndAddress = ClientDLLBase + 0x167C000;

    std::string EntityList = "48 8B ?? ?? ?? ?? ?? 48 89 7C 24 30 8B FA C1 EB 0E 81 E7 FF 3F";
    std::string ViewMatrix = "48 8D ?? ?? ?? ?? ?? 48 C1 E0 06 48 03 C1 C3 CC CC";
    std::string LocalPlayerController = "48 8D ?? ?? ?? ?? ?? 48 8B 04 C1 48 83 C4 28 C3 CC CC CC";
    std::string LocalPlayerPawn = "48 89 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 33 C0 89 0D";
    std::string GameRules = "48 8B ?? ?? ?? ?? ?? 48 85 C0 ?? ?? 45 85 C0";


    std::string pClippingWeapon = "48 8B 87 ?? ?? ?? ?? 48 85 C0 ?? ?? ?? ?? ?? ?? 48 3B C3";
    std::string vOldOrigin = "F2 0F 11 ?? ?? ?? ?? ?? 8B 40 ?? 89 83 ?? ?? 00 00 8B 83 ?? ?? 00 00";
    std::string iTeamNum = "0F B6 86 ?? ?? ?? ?? 48 8B CE 89 47 ?? 48 8B 06";
    std::string pArmorValue = "8B 83 ?? ?? ?? ?? 89 83 ?? ?? ?? ?? 48 83 C4 20 5B C3 CC CC CC CC CC CC";
    std::string pGameSceneNode = "48 8B 81 ?? ?? ?? ?? 48 8B 48 ?? 48 85 C9";
    std::string pBoneArray = "48 8B 97 ?? ?? ?? ?? 4C 8D ?? ?? 4D 8B ?? 49 8B ??";
    std::string bPawnIsAlive = "0F B6 81 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC B0 01 C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 89";
    std::string iHealth = "8B 83 ?? ?? ?? ?? 89 83 ?? ?? ?? ?? 48 8b ?? ?? ?? 48 83 C4 20 ?? C3";



    uintptr_t dwEntityList = PatternScan(StartAddress, EndAddress, EntityList, true, false, 3);
    uintptr_t dwViewMatrix = PatternScan(StartAddress, EndAddress, ViewMatrix, true, false, 3);
    uintptr_t dwLocalPlayerController = PatternScan(StartAddress, EndAddress, LocalPlayerController, true, false, 3);
    uintptr_t dwLocalPlayerPawn = PatternScan(StartAddress, EndAddress, LocalPlayerPawn, true, false, 3);
    uintptr_t dwGameRules = PatternScan(StartAddress, EndAddress, GameRules, true, false, 3);



    uintptr_t m_pClippingWeapon = PatternScan(StartAddress, EndAddress, pClippingWeapon, false, true, 3);
    uintptr_t m_vOldOrigin = PatternScan(StartAddress, EndAddress, vOldOrigin, false, true, 4);
    uintptr_t m_iTeamNum = PatternScan(StartAddress, EndAddress, iTeamNum, false, true, 3);
    uintptr_t m_ArmorValue = PatternScan(StartAddress, EndAddress, pArmorValue, false, true, 2);
    uintptr_t m_pGameSceneNode = PatternScan(StartAddress, EndAddress, pGameSceneNode, false, true, 3);
    uintptr_t m_pBoneArray = PatternScan(StartAddress, EndAddress, pBoneArray, false, true, 3);
    uintptr_t m_bPawnIsAlive = PatternScan(StartAddress, EndAddress, bPawnIsAlive, false, true, 3);
    uintptr_t m_iHealth = PatternScan(StartAddress, EndAddress, iHealth, false, true, 2);


    dwEntityList = dwEntityList - ClientDLLBase;
    dwViewMatrix = dwViewMatrix - ClientDLLBase;
    dwLocalPlayerController = dwLocalPlayerController - ClientDLLBase;
    dwLocalPlayerPawn = (dwLocalPlayerPawn + 0x48) - ClientDLLBase;
    dwGameRules = dwGameRules - ClientDLLBase;

    if (dwEntityList != 0)
    {
        Offset::dwEntityList = dwEntityList;
        Offset::dwViewMatrix = dwViewMatrix;
        Offset::dwLocalPlayerController = dwLocalPlayerController;
        Offset::dwLocalPlayerPawn = dwLocalPlayerPawn;
        Offset::dwGameRules = dwGameRules;
        Offset::C_CSPlayerPawnBase.m_pClippingWeapon = m_pClippingWeapon;
        Offset::C_CSPlayerPawnBase.m_vOldOrigin = m_vOldOrigin;
        Offset::C_CSPlayerPawnBase.m_ArmorValue = m_ArmorValue;
        Offset::CCSPlayerController.m_iTeamNum = m_iTeamNum;
        Offset::CCSPlayerController.m_pGameSceneNode = m_pGameSceneNode;
        Offset::CCSPlayerController.m_pBoneArray = m_pBoneArray;
        Offset::CCSPlayerController.m_bPawnIsAlive = m_bPawnIsAlive;
        Offset::CCSPlayerController.m_iszPlayerName = m_bPawnIsAlive - 0x1B4;
        Offset::CCSPlayerController.m_hPlayerPawn = m_bPawnIsAlive - 0x8;
        Offset::CCSPlayerController.m_iHealth = m_iHealth;
        return TRUE;
    }

    return FALSE;
}

VOID Initialization()
{
    if (!GetModule)
    {
        ClientDLLBase = reinterpret_cast<DWORD64>(GetModuleHandle(L"client.dll"));

        if (InitializeOffsets())
        {
            gGame.View.MatrixAddress = ClientDLLBase + Offset::dwViewMatrix;
            ReadMemoryInternal(ClientDLLBase + Offset::dwEntityList, EntityList, sizeof(EntityList));
            ReadMemoryInternal(EntityList + 0x10, EntityListEntry, sizeof(EntityListEntry));
            GetModule = TRUE;
        }
    }
}


VOID GetWeapon(CEntity& Entity)
{
    DWORD64 WeaponAdress;
    ReadMemoryInternal(Entity.PawnAddress + Offset::C_CSPlayerPawnBase.m_pClippingWeapon, WeaponAdress, sizeof(WeaponAdress));

    ReadMemoryInternal(WeaponAdress + 0x10, WeaponAdress, sizeof(WeaponAdress));
    ReadMemoryInternal(WeaponAdress + 0x20, WeaponAdress, sizeof(WeaponAdress));
    ReadMemoryInternal(WeaponAdress, WeaponBuffer, sizeof(WeaponBuffer) - 1);

    WeaponBuffer[sizeof(WeaponBuffer) - 1] = '\0';
    WeaponName = std::string(WeaponBuffer);
    std::size_t UnderScoreIndex = WeaponName.find('_');

    if (UnderScoreIndex != std::string::npos)
        Entity.Pawn.PlayerWeapon = WeaponName.substr(UnderScoreIndex + 1);



}

VOID UpdatePlayerBoneData(CEntity& Entity)
{
    DWORD64 GameSceneNode;
    DWORD64 BoneArrayAddress;

    ReadMemoryInternal<DWORD64>(Entity.PawnAddress + Offset::CCSPlayerController.m_pGameSceneNode, GameSceneNode, sizeof(GameSceneNode));
    ReadMemoryInternal<DWORD64>(GameSceneNode + Offset::CCSPlayerController.m_pBoneArray, BoneArrayAddress, sizeof(BoneArrayAddress));

    BoneJointData BoneArray[30]{};
    ReadMemoryInternal(BoneArrayAddress, BoneArray, sizeof(BoneArray));

    Entity.BoneData.BonePosList.clear();

    for (int i = 0; i < 30; ++i)
    {
        Vec2 ScreenPos;
        bool IsVisible = gGame.View.WorldToScreen(BoneArray[i].Pos, ScreenPos);
        Entity.BoneData.BonePosList.push_back({ BoneArray[i].Pos, ScreenPos, IsVisible });
    }

}


VOID GetPlayerName(CEntity& Entity)
{
    DWORD64 PlayerNameAddress = Entity.EntityAddress + Offset::CCSPlayerController.m_iszPlayerName;
    ReadMemoryInternal(PlayerNameAddress, Buffer, 20);
    Entity.Controller.PlayerName = Buffer;
}


VOID GetPlayerEntity()
{
    EntityPlayerVector.clear();
    PawnVector.clear();

    for (int i = 0; i < 64; i++)
    {
        DWORD64 EntityAddress;
        DWORD PawnIndex;
        DWORD64 EntityPawnAddress;

        ReadMemoryInternal<DWORD64>(EntityListEntry + (i + 1) * 0x78, EntityAddress, sizeof(EntityAddress));
        ReadMemoryInternal<DWORD>(EntityAddress + Offset::CCSPlayerController.m_hPlayerPawn, PawnIndex, sizeof(PawnIndex));
        DWORD64 EntityPawnListEntry;
        ReadMemoryInternal<DWORD64>(ClientDLLBase + Offset::dwEntityList, EntityPawnListEntry, sizeof(EntityPawnListEntry));
        EntityPawnAddress = EntityPawnListEntry + 0x10 + 8 * ((PawnIndex & 0x7FFF) >> 9);
        ReadMemoryInternal<DWORD64>(EntityPawnAddress, EntityPawnAddress, sizeof(EntityPawnAddress));
        EntityPawnAddress += 0x78 * (PawnIndex & 0x1FF);
        ReadMemoryInternal<DWORD64>(EntityPawnAddress, EntityPawnAddress, sizeof(EntityPawnAddress));

        if (EntityAddress > 0x7FFFF && EntityAddress != 0 && EntityAddress < 0x7FFD5DC070000000)
        {
            EntityPlayerVector.push_back(EntityAddress);
            PawnVector.push_back(EntityPawnAddress);
        }
        
    }
}

 
VOID GetEntityWeapon()
{
    EntityWeaponVector.clear();
    ReadMemoryInternal<int>(EntityList + Offset::dwGameEntitySystem.GetHighestEntityIndex, HighestEntityIndex, sizeof(HighestEntityIndex));

    for (int i = 64; i < HighestEntityIndex; i++)
    {
        DWORD64 EntityAddress;

        ReadMemoryInternal<DWORD64>(EntityListEntry + (i + 1) * 0x78, EntityAddress, sizeof(EntityAddress));
        EntityWeaponVector.push_back(EntityAddress);

    }
}



VOID UpdateItemESP(CEntity& Entity) 
{
    DWORD64 Offset10;
    DWORD64 Offset20;

    ReadMemoryInternal<DWORD64>(Entity.C_BaseEntity.BaseEntity + 0x10, Offset10, sizeof(Offset10));
    ReadMemoryInternal<DWORD64>(Offset10 + 0x20, Offset20, sizeof(Offset20));


    if (ReadMemoryInternal(Offset20, WeaponBuffer2, sizeof(WeaponBuffer2) - 1))
    {
        WeaponBuffer2[sizeof(WeaponBuffer2) - 1] = '\0';
        WeaponName2 = std::string(WeaponBuffer2);
        std::size_t UnderScoreIndex = WeaponName2.find('_');

        if (strncmp(WeaponBuffer2, "weapon_", 7) == 0) 
        {
            if (UnderScoreIndex != std::string::npos)
                WeaponName2 = WeaponName2.substr(UnderScoreIndex + 1);

            DWORD64 ItemSceneNode;

   

            ReadMemoryInternal<DWORD64>(Entity.C_BaseEntity.BaseEntity + Offset::CCSPlayerController.m_pGameSceneNode, ItemSceneNode, sizeof(ItemSceneNode));
            ReadMemoryInternal<Vec3>(ItemSceneNode + 0x80, Entity.C_BaseEntity.ItemPos3D , sizeof(Entity.C_BaseEntity.ItemPos3D));
        }
    }
}



VOID UpdatePlayersESP(CEntity& Entity)
{
    ReadMemoryInternal<int>(Entity.EntityAddress + Offset::CCSPlayerController.m_bPawnIsAlive, Entity.Controller.AliveStatus, sizeof(Entity.Controller.AliveStatus));
    ReadMemoryInternal<int>(Entity.PawnAddress + Offset::CCSPlayerController.m_iHealth, Entity.Controller.iHealth, sizeof(Entity.Controller.iHealth));
    ReadMemoryInternal<int>(Entity.EntityAddress + Offset::CCSPlayerController.m_iTeamNum, Entity.Controller.TeamID, sizeof(Entity.Controller.TeamID));
    ReadMemoryInternal<int>(Entity.PawnAddress + Offset::C_CSPlayerPawnBase.m_ArmorValue, Entity.Pawn.Armor, sizeof(Entity.Pawn.Armor));
    ReadMemoryInternal<Vec3>(Entity.PawnAddress + Offset::C_CSPlayerPawnBase.m_vOldOrigin, Entity.Pawn.vOldOrigin, sizeof(Entity.Pawn.vOldOrigin));

}



VOID TriggerBot()
{
    int CurrentHealth;
    bool TriggerShoot = false;
    int TeamID;

    ReadMemoryInternal(ClientDLLBase + Offset::dwLocalPlayerPawn, LocalEntity.LocalPawn, sizeof(LocalEntity.LocalPawn));
    ReadMemoryInternal(LocalEntity.LocalPawn + 0x15A4, LocalEntity.uHandle, sizeof(LocalEntity.uHandle));

    if (LocalEntity.uHandle == -1)
        return;

    DWORD64 EntityPawnAddress;
    EntityPawnAddress = EntityList + 0x10 + 8 * ((LocalEntity.uHandle & 0x7FFF) >> 9);
    ReadMemoryInternal<DWORD64>(EntityPawnAddress, EntityPawnAddress, sizeof(EntityPawnAddress));
    EntityPawnAddress += 0x78 * (LocalEntity.uHandle & 0x1FF);
    ReadMemoryInternal<DWORD64>(EntityPawnAddress, EntityPawnAddress, sizeof(EntityPawnAddress));
    ReadMemoryInternal<int>(EntityPawnAddress + 0x334, CurrentHealth, sizeof(CurrentHealth));
    ReadMemoryInternal<int>(EntityPawnAddress + Offset::CCSPlayerController.m_iTeamNum, TeamID, sizeof(TeamID));

    
    if (MenuConfig::TeamCheck)
        TriggerShoot = LocalEntity.Controller.TeamID != TeamID && CurrentHealth > 0;
    else
    
        TriggerShoot = CurrentHealth > 0;


    if (!TriggerShoot)
        return;


    static std::chrono::time_point<std::chrono::steady_clock> LastTimePoint = std::chrono::steady_clock::now();
    auto CurTimePoint = std::chrono::steady_clock::now();

    if (CurTimePoint - LastTimePoint >= std::chrono::milliseconds(static_cast<long long>(MenuConfig::TriggerBotDelay)))
    {
        const bool isAlreadyShooting = GetAsyncKeyState(VK_LBUTTON) < 0;

        if (!isAlreadyShooting)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }

        LastTimePoint = CurTimePoint;
    }
}



VOID EnableCheats()
{
    Initialization();


    gGame.View.Matrix = reinterpret_cast<float(*)[4][4]>(gGame.View.MatrixAddress);

    Vec2 MousePos = ImGui::GetMousePos();
    float ClosestDistance = 1000000.0f;
    Vec2 AimAtPosition = { 0, 0 };

    ReadMemoryInternal(ClientDLLBase + Offset::dwLocalPlayerController, LocalEntity.BaseEntityAddress, sizeof(LocalEntity.BaseEntityAddress));
    ReadMemoryInternal<int>(LocalEntity.BaseEntityAddress + Offset::CCSPlayerController.m_iTeamNum, LocalEntity.Controller.TeamID, sizeof(LocalEntity.Controller.TeamID));

    GetPlayerEntity();
    GetEntityWeapon();
    for (int i = 0; i < PawnVector.size(); i++)
    {
        CEntity Entity;

        Entity.EntityAddress = EntityPlayerVector[i];
        Entity.PawnAddress = PawnVector[i];


        if (Entity.EntityAddress == LocalEntity.BaseEntityAddress)
            ReadMemoryInternal<Vec3>(Entity.PawnAddress + Offset::C_CSPlayerPawnBase.m_vOldOrigin, LocalEntity.Pawn.vOldOrigin, sizeof(LocalEntity.Pawn.vOldOrigin));

        if (Entity.EntityAddress == LocalEntity.BaseEntityAddress)
            continue;

        UpdatePlayersESP(Entity);
        UpdatePlayerBoneData(Entity);
        GetPlayerName(Entity);
        GetWeapon(Entity);

        if (Entity.BoneData.BonePosList.size() == 0)
            continue;

        if (MenuConfig::TeamCheck && Entity.Controller.TeamID == LocalEntity.Controller.TeamID)
            continue;

        if (!Entity.IsAlive())
            continue;


        Rect = Render::Get2DBox(Entity);
        RenderESP(Entity);

        if (MenuConfig::EnableAimbot)
        {
            if (MenuConfig::ClosestBone)
            {
                float MinDistanceToMouse = FLT_MAX;
                Vec3 ClosestBoneWorldPos;
                Vec2 ClosestBoneScreenPos;

                for (int i = 0; i < Entity.BoneData.BonePosList.size(); ++i)
                {
                    Vec3 BoneWorldPos = Entity.BoneData.BonePosList[i].Pos;
                    Vec2 BoneScreenPos;
                    if (gGame.View.WorldToScreen(BoneWorldPos, BoneScreenPos))
                    {
                        float Distance = MousePos.DistanceTo(BoneScreenPos);
                        if (Distance < MinDistanceToMouse)
                        {
                            MinDistanceToMouse = Distance;
                            ClosestBoneWorldPos = BoneWorldPos;
                            ClosestBoneScreenPos = BoneScreenPos;
                        }
                    }
                }

                float DistanceToLocal = LocalEntity.Pawn.vOldOrigin.DistanceTo(Entity.Pawn.vOldOrigin);
                float Distance2_Meters = DistanceToLocal * 0.0254;

                if (Distance2_Meters < ClosestDistance && MinDistanceToMouse <= MenuConfig::FOV)
                {
                    ClosestDistance = Distance2_Meters;
                    AimAtPosition = ClosestBoneScreenPos;
                }
            }
            else
            {
                Vec3 EnemySelectedBoneWorldPos = Entity.BoneData.BonePosList[MenuConfig::SelectedBoneIndex].Pos;
                Vec2 EnemyHeadScreenPos;

                if (gGame.View.WorldToScreen(EnemySelectedBoneWorldPos, EnemyHeadScreenPos))
                {
                    float DistanceToLocal = LocalEntity.Pawn.vOldOrigin.DistanceTo(Entity.Pawn.vOldOrigin);
                    float Distance2_Meters = DistanceToLocal * 0.0254;

                    float Distance = MousePos.DistanceTo(EnemyHeadScreenPos);

                    if (Distance2_Meters < ClosestDistance && Distance <= MenuConfig::FOV)
                    {
                        ClosestDistance = Distance2_Meters;
                        AimAtPosition = EnemyHeadScreenPos;
                    }
                }
            }
        }
    }
 
    
    for (int i = 0; i < EntityWeaponVector.size(); i++)
    {
        CEntity Entity;
        Entity.C_BaseEntity.BaseEntity = EntityWeaponVector[i];

       UpdateItemESP(Entity);

       Render::GetItemText(Entity);
    }
    
    if (MenuConfig::TriggerBot)
        TriggerBot();

    if (MenuConfig::EnableAimbot)
    {
        if (ClosestDistance != 1000000.0f && GetAsyncKeyState(HotKeyCodes[MenuConfig::SelectedHotkey]) & 0x8000)
        {
            float SmoothFactor = 1.0f / MenuConfig::AimSmoothness;
            float DeltaX = (AimAtPosition.x - MousePos.x) * SmoothFactor;
            float DeltaY = (AimAtPosition.y - MousePos.y) * SmoothFactor;

            mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(DeltaX), static_cast<DWORD>(DeltaY), 0, 0);
        }

    }

    

}