#pragma once
#include "Game.h"
#include "View.hpp"
#include "Bone.h"

class PlayerPawn
{
public:
    Vec3 vOldOrigin;
    int Armor;
    std::string PlayerWeapon;
};


class PlayerController
{
public:
    int AliveStatus;
    int TeamID;
    int iHealth;
    std::string PlayerName;

};

class EntityBase
{
public:
    DWORD64 BaseEntity;
    Vec2 ItemPos2D;
    Vec3 ItemPos3D;

};

class CEntity
{
public:
    PlayerController Controller;
    PlayerPawn Pawn;
    EntityBase C_BaseEntity;
    DWORD64 BaseEntityAddress;
    DWORD64 PawnAddress;
    DWORD64 LocalPawn;
    DWORD uHandle;
    CBone BoneData;
    int MaxEntities;
    DWORD64 EntityAddress;
    BOOL IsAlive();
};