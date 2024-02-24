#pragma once
#include <Windows.h>
#include <vector>
#include "View.hpp"
#include <string>
inline bool GetModule = false;
inline DWORD64 ClientDLLBase;
inline char Buffer[20];
inline std::string WeaponName;
inline char WeaponBuffer[64];
DWORD PawnIndex;
DWORD64 EntityListEntry;
DWORD64 EntityList;
int HighestEntityIndex;
Vec2 ScreenPos;
inline ImVec4 Rect;
int MenuIndex;
bool Loading = false;
ImFont* CustomFont = nullptr;
std::vector<DWORD64> EntityPlayerVector;
std::vector<DWORD64> EntityWeaponVector;
std::vector<DWORD64> PawnVector;
inline std::string WeaponName2;
inline char WeaponBuffer2[64];