#pragma once

#include "includes.h"
#include "MenuConfig.hpp"
#include "xor.hpp"


const char* Bones[] =
{
	"Head", // head = 6
	"Neck", // neck_0 = 5
	"Spine 1", // spine_1 = 4
	"Spine 2", // spine_2 = 2
	"Pelvis", // pelvis = 0
	"Left Upper Arm", // arm_upper_L = 8
	"Left Lower Arm", // arm_lower_L = 9
	"Left Hand", // hand_L = 10
	"Right Upper Arm", // arm_upper_R = 13
	"Right Lower Arm", // arm_lower_R = 14
	"Right Hand", // hand_R = 15
	"Left Upper Leg", // leg_upper_L = 22
	"Left Lower Leg", // leg_lower_L = 23
	"Left Ankle", // ankle_L = 24
	"Right Upper Leg", // leg_upper_R = 25
	"Right Lower Leg", // leg_lower_R = 26
	"Right Ankle" // ankle_R = 27
};

BONEINDEX BoneIndexMap[] =
{
	BONEINDEX::head,        // 0 "Head" maps to BONEINDEX::head (6)
	BONEINDEX::neck_0,      // 1 "Neck" maps to BONEINDEX::neck_0 (5)
	BONEINDEX::spine_1,     // 2 "Spine 1" maps to BONEINDEX::spine_1 (4)
	BONEINDEX::spine_2,     // 3 "Spine 2" maps to BONEINDEX::spine_2 (2)
	BONEINDEX::pelvis,      // 4 "Pelvis" maps to BONEINDEX::pelvis (0)
	BONEINDEX::arm_upper_L, // 5 "Left Upper Arm" maps to BONEINDEX::arm_upper_L (8)
	BONEINDEX::arm_lower_L, // 6 "Left Lower Arm" maps to BONEINDEX::arm_lower_L (9)
	BONEINDEX::hand_L,      // 7 "Left Hand" maps to BONEINDEX::hand_L (10)
	BONEINDEX::arm_upper_R, // 8 "Right Upper Arm" maps to BONEINDEX::arm_upper_R (13)
	BONEINDEX::arm_lower_R, // 9 "Right Lower Arm" maps to BONEINDEX::arm_lower_R (14)
	BONEINDEX::hand_R,      // 10 "Right Hand" maps to BONEINDEX::hand_R (15)
	BONEINDEX::leg_upper_L, // 11 "Left Upper Leg" maps to BONEINDEX::leg_upper_L (22)
	BONEINDEX::leg_lower_L, // 12 "Left Lower Leg" maps to BONEINDEX::leg_lower_L (23)
	BONEINDEX::ankle_L,     // 13 "Left Ankle" maps to BONEINDEX::ankle_L (24)
	BONEINDEX::leg_upper_R, // 14 "Right Upper Leg" maps to BONEINDEX::leg_upper_R (25)
	BONEINDEX::leg_lower_R, // 15 "Right Lower Leg" maps to BONEINDEX::leg_lower_R (26)
	BONEINDEX::ankle_R      // 16 "Right Ankle" maps to BONEINDEX::ankle_R (27)
};



VOID MenuESP()
{
	ImGui::Checkbox(xorstr_("Enable Box ESP"), &MenuConfig::ShowBoxESP);
	ImGui::SameLine(190);
	ImGui::ColorEdit4(xorstr_("##BoxColor"), reinterpret_cast<float*>(&MenuConfig::BoxColor), ImGuiColorEditFlags_NoInputs);
	ImGui::SameLine(240);
	ImGui::Text(xorstr_("Thickness:"));
	ImGui::SameLine(330);
	ImGui::SliderFloat(xorstr_("##BoxThickness"), &MenuConfig::BoxThickness, 0.5f, 2.0f, "%.2f");

	ImGui::Spacing();
	ImGui::Checkbox(xorstr_("Enable Bone ESP"), &MenuConfig::ShowBoneESP);
	ImGui::SameLine(190);
	ImGui::ColorEdit4(xorstr_("##BoneColor"), reinterpret_cast<float*>(&MenuConfig::BoneColor), ImGuiColorEditFlags_NoInputs);
	ImGui::SameLine(240);
	ImGui::Text(xorstr_("Thickness:"));
	ImGui::SameLine(330);
	ImGui::SliderFloat(xorstr_("##BoneThickness"), &MenuConfig::BoneThickness, 0.5f, 2.0f, "%.2f");

	ImGui::Spacing();
	ImGui::Checkbox(xorstr_("Team Check"), &MenuConfig::TeamCheck);
	ImGui::Checkbox(xorstr_("Health ESP"), &MenuConfig::ShowHealthBar);
	ImGui::Checkbox(xorstr_("Distance ESP"), &MenuConfig::ShowDistance);
	ImGui::Checkbox(xorstr_("Weapon ESP"), &MenuConfig::ShowWeaponESP);
	ImGui::Checkbox(xorstr_("Name ESP"), &MenuConfig::ShowPlayerName);
	ImGui::Checkbox(xorstr_("Armour ESP"), &MenuConfig::ShowArmor);
	ImGui::Checkbox(xorstr_("Item ESP"), &MenuConfig::ItemESP);
	ImGui::SameLine(190);
	ImGui::ColorEdit4(xorstr_("##ItemColor"), reinterpret_cast<float*>(&MenuConfig::ItemColor), ImGuiColorEditFlags_NoInputs);

}


VOID MenuAimBot()
{
	ImGui::Checkbox(xorstr_("AimBot"), &MenuConfig::EnableAimbot);
	ImGui::SliderFloat(xorstr_("Smoothness"), &MenuConfig::AimSmoothness, 1.0f, 10.0f);
	ImGui::SliderFloat(xorstr_("FOV"), &MenuConfig::FOV, 1.0f, 1000.0f);
	ImGui::Checkbox(xorstr_("Draw FOV"), &MenuConfig::DrawFOV);
	ImGui::Checkbox(xorstr_("Aim Closest Bone"), &MenuConfig::ClosestBone);



	if (ImGui::Combo(xorstr_("Bone Selection"), &MenuConfig::CurrentBoneIndex, Bones, IM_ARRAYSIZE(Bones)))
	{
		BONEINDEX SelectedBoneIndex = BoneIndexMap[MenuConfig::CurrentBoneIndex];
		MenuConfig::SelectedBoneIndex = SelectedBoneIndex;
	}

	if (ImGui::Combo(xorstr_("AimKey"), &CurrentHotkey, HotKeys, IM_ARRAYSIZE(HotKeys)))
		MenuConfig::SelectedHotkey = CurrentHotkey;



}



VOID TriggerBotMenu()
{
	ImGui::Checkbox(xorstr_("Enable TriggerBot"), &MenuConfig::TriggerBot);
	ImGui::SliderFloat(xorstr_("TriggerBot Delay/ms"), &MenuConfig::TriggerBotDelay, 1.0f, 1000.0f);
}


VOID Renderer()
{

	ImVec4 clear_color = ImVec4(0.05f, 0.1f, 0.15f, 1.0f);
	ImGuiWindowFlags WindowFlags =
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowBgAlpha(0.90f);
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.3f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.3f, 0.2f, 1.0f);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, clear_color);
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, clear_color);
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, clear_color);
	ImGui::SetNextWindowSize(ImVec2(650, 470));

	if (MenuConfig::ShowMenu)
	{
		ImGui::Begin(xorstr_("CS2 INTERNAL"), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button(xorstr_("ESP"), ImVec2(185, 0)))
			MenuConfig::MenuState = 1;
		ImGui::SameLine();
		if (ImGui::Button(xorstr_("Aimbot"), ImVec2(185, 0)))
			MenuConfig::MenuState = 2;
		ImGui::SameLine();
		if (ImGui::Button(xorstr_("TriggerBot"), ImVec2(185, 0)))
			MenuConfig::MenuState = 3;

		switch (MenuConfig::MenuState)
		{
		case 1:
			ImGui::SetWindowSize(ImVec2(650, 470));
			MenuESP();
			break;
		case 2:
			ImGui::SetWindowSize(ImVec2(650, 470));
			MenuAimBot();
			break;

		case 3:
			ImGui::SetWindowSize(ImVec2(650, 470));
			TriggerBotMenu();
			break;

		default:
			break;

		}

		ImGui::End();

	}

	ImGui::PopStyleColor(3);
}
