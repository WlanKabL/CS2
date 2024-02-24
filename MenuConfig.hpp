#pragma once
#include "Game.h"
#include "Bone.h"

inline int CurrentHotkey;


namespace MenuConfig
{
	inline bool ShowBoneESP = true;
	inline bool ShowBoxESP = true;
	inline bool ShowHealthBar = true;
	inline int SelectedHotkey;
	inline bool ShowWeaponESP = true;
	inline int SelectedBoneIndex;
	inline bool TriggerBot = false;
	inline bool EnableAimbot = false;
	inline bool ShowDistance = true;
	inline bool ItemESP = true;
	inline bool ShowPlayerName = true;
	inline bool VisibleCheck = true;
	inline bool DrawFOV = false;
	inline ImColor BoneColor = ImColor(255, 255, 255, 255);
	inline ImColor BoxColor = ImColor(255, 0, 0, 255); 
	inline ImColor ItemColor = ImColor(255, 0, 0, 255);
	float BoxThickness = 1.0f;
	float BoneThickness = 1.0f; 
	float AimSmoothness = 1.0f;
	inline bool ClosestBone = false;
	float TriggerBotDelay = 0.050;
	float FOV = 100.0f;;
	inline bool ShowMenu = true;
	inline bool TeamCheck = true;
	inline int  BoxType = 0;
	int MenuState = 1;
	inline bool AntiFlashbang = true;
	inline bool ShowArmor = false;
	inline bool ShowTimer = true;
	inline int CurrentBoneIndex = 6;

}


inline const char* HotKeys[] =
{
	"Shift", "Alt",
	"Left Mouse Button", "Right Mouse Button", "Middle Mouse Button",
	"Mouse Button 4", "Mouse Button 5"
};

inline int HotKeyCodes[] = 
{
	VK_SHIFT, // "Shift"
	VK_MENU,  // "Alt"
	VK_LBUTTON, // "Left Mouse Button"
	VK_RBUTTON, // "Right Mouse Button"
	VK_MBUTTON, // "Middle Mouse Button"
	VK_XBUTTON1, // "Mouse Button 4"
	VK_XBUTTON2, // "Mouse Button 5"
};

