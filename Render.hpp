#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <map>
#include "Entity.h"
#include "MenuConfig.hpp"
#include <string>
#include "Vars.h"


Vec3 TopPos3D;
CEntity LocalEntity;
const float MetricConversion = 0.0254f; 
namespace Render
{
	ImVec4 Get2DBox(CEntity& Entity)
	{
		const float FixedPlayerHeight = 75.0f;

		TopPos3D = Entity.Pawn.vOldOrigin;
		TopPos3D.z += FixedPlayerHeight;

		Vec2 GroundPos2D, TopPos2D;
		if (!gGame.View.WorldToScreen(Entity.Pawn.vOldOrigin, GroundPos2D) || !gGame.View.WorldToScreen(TopPos3D, TopPos2D))
			return ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };	


		if (TopPos2D.y < 0.0f || GroundPos2D.y > ImGui::GetIO().DisplaySize.y)
			return ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f };

		Vec2 Size, Pos;
		Size.y = std::abs(TopPos2D.y - GroundPos2D.y);
		Size.x = Size.y * 0.60;

		Pos.x = GroundPos2D.x - Size.x / 2;
		Pos.y = TopPos2D.y;

		return ImVec4{ Pos.x, Pos.y, Size.x, Size.y };
	}

	VOID GetItemText(CEntity& Entity)
	{
		if (!gGame.View.WorldToScreen(Entity.C_BaseEntity.ItemPos3D, Entity.C_BaseEntity.ItemPos2D))
			return; 

		if (Entity.C_BaseEntity.ItemPos2D.x < 0.0f || Entity.C_BaseEntity.ItemPos2D.x > ImGui::GetIO().DisplaySize.x ||
			Entity.C_BaseEntity.ItemPos2D.y < 0.0f || Entity.C_BaseEntity.ItemPos2D.y > ImGui::GetIO().DisplaySize.y)
			return; 

		if (Entity.C_BaseEntity.ItemPos3D.x == 0 || Entity.C_BaseEntity.ItemPos3D.y == 0 || Entity.C_BaseEntity.ItemPos3D.z == 0)
			return;

		if (MenuConfig::ItemESP)
		{
			if (!WeaponName2.empty())
				WeaponName2[0] = toupper(WeaponName2[0]);

			ImGui::GetBackgroundDrawList()->AddText(ImVec2(Entity.C_BaseEntity.ItemPos2D.x, Entity.C_BaseEntity.ItemPos2D.y), MenuConfig::ItemColor, WeaponName2.c_str());
		}

	}

	void DrawBone(CEntity& Entity, ImColor Color, float Thickness)
	{
		Vec3 LocalEntityPos = LocalEntity.Pawn.vOldOrigin;

		Vec3 EntityPos = Entity.Pawn.vOldOrigin;
		Vec3 BonePos27 = Entity.BoneData.BonePosList[27].Pos;

		float MaxDistance = 45.0f;
		float DistanceToBone27 = sqrt(
			pow(BonePos27.x - EntityPos.x, 2) +
			pow(BonePos27.y - EntityPos.y, 2) +
			pow(BonePos27.z - EntityPos.z, 2)
		);

		if (DistanceToBone27 > MaxDistance)
			return;

		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

		for (const auto& boneList : BoneJointList::List)
		{
			BoneJointPos Previous;
			bool isFirstBone = true;
			for (const auto& Index : boneList)
			{
				BoneJointPos Current = Entity.BoneData.BonePosList[Index];

				if (!isFirstBone && Previous.IsVisible && Current.IsVisible)
				{
					draw_list->AddLine(
						ImVec2(Previous.ScreenPos.x, Previous.ScreenPos.y),
						ImVec2(Current.ScreenPos.x, Current.ScreenPos.y),
						Color, Thickness
					);
				}

				isFirstBone = false;
				Previous = Current;
			}
		}



		Vec3 HeadPos = Entity.BoneData.BonePosList[BoneJointList::List.front().front()].Pos;
		float SquaredDistanceToHead = pow(HeadPos.x - LocalEntityPos.x, 2) +
			pow(HeadPos.y - LocalEntityPos.y, 2) +
			pow(HeadPos.z - LocalEntityPos.z, 2);

		float DistanceToHead = sqrt(SquaredDistanceToHead) * MetricConversion;

		if (DistanceToHead < 35.0f)
		{
			BoneJointPos Head = Entity.BoneData.BonePosList[BoneJointList::List.front().front()];
			draw_list->AddCircle(
				ImVec2(Head.ScreenPos.x, Head.ScreenPos.y),
				9.0f, Color, 12, Thickness
			);
		}

	}


	void DrawCustomBox(const ImVec2& rect_min, const ImVec2& rect_max, const ImColor& boxColor, float borderThickness, float rounding = 1.0f)
	{
		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
		draw_list->AddRect(rect_min, rect_max, boxColor, rounding, 0, borderThickness);
	}


	void DrawHealthBar(DWORD EntityAddress, float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, bool Horizontal = true)
	{
		ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

		float Proportion = CurrentHealth / MaxHealth;

		ImColor FirstStageColor = ImColor(96, 246, 113, 220);
		ImColor SecondStageColor = ImColor(247, 214, 103, 220);
		ImColor ThirdStageColor = ImColor(255, 95, 95, 220);
		ImColor BackGroundColor = ImColor(90, 90, 90, 220);
		ImColor Color;
		if (Proportion > 0.5)
			Color = FirstStageColor;
		else if (Proportion > 0.25)
			Color = SecondStageColor;
		else
			Color = ThirdStageColor;

		DrawList->AddRectFilled(Pos, { Pos.x + Size.x, Pos.y + Size.y }, BackGroundColor);

		if (Horizontal)
		{
			DrawList->AddRectFilled(Pos, { Pos.x + Size.x * Proportion, Pos.y + Size.y }, Color);
		}
		else
		{
			float healthHeight = Size.y * Proportion;
			DrawList->AddRectFilled({ Pos.x, Pos.y + Size.y - healthHeight }, { Pos.x + Size.x, Pos.y + Size.y }, Color);
		}

		ImColor BorderColor = ImColor(45, 45, 45, 220);
		DrawList->AddRect(Pos, { Pos.x + Size.x, Pos.y + Size.y }, BorderColor);
	}

}


VOID DrawFov()
{
	if (!MenuConfig::DrawFOV)
		return;

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	float screenWidth = ImGui::GetIO().DisplaySize.x;
	float screenHeight = ImGui::GetIO().DisplaySize.y;
	ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);
	draw_list->AddCircle(center, MenuConfig::FOV, IM_COL32(255, 255, 255, 255), 64, 2.0f);
}


BOOL RenderESP(CEntity& Entity)
{

	if (MenuConfig::ShowBoneESP)
	{
		Render::DrawBone(Entity, MenuConfig::BoneColor, MenuConfig::BoneThickness);
	}

	if (MenuConfig::ShowBoxESP)
	{
		ImVec2 Rect_Min = ImVec2(Rect.x, Rect.y);
		ImVec2 Rect_Max = ImVec2(Rect.x + Rect.z, Rect.y + Rect.w);

		Render::DrawCustomBox(Rect_Min, Rect_Max, MenuConfig::BoxColor, MenuConfig::BoxThickness);
	}

	if (MenuConfig::ShowHealthBar)
	{
		ImVec2 HealthBarPos, HealthBarSize;

		HealthBarPos = { Rect.x - 7.0f, Rect.y };
		HealthBarSize = { 7.0f, Rect.w };
		Render::DrawHealthBar(Entity.EntityAddress, 100, Entity.Controller.iHealth, HealthBarPos, HealthBarSize, false);

	}

	if (MenuConfig::ShowPlayerName)
	{

		if (!Entity.Controller.PlayerName.empty())
			Entity.Controller.PlayerName[0] = toupper(Entity.Controller.PlayerName[0]);



		ImVec2 TextSize = ImGui::CalcTextSize(Entity.Controller.PlayerName.c_str());
		ImVec2 TextPos = { Rect.x + Rect.z / 2 - TextSize.x / 2, Rect.y - 16 };
		ImGui::GetForegroundDrawList()->AddText(TextPos, ImColor(255, 255, 255, 255), Entity.Controller.PlayerName.c_str());

	}

	if (MenuConfig::ShowArmor)
	{
		std::string armorText = "A: " + std::to_string(Entity.Pawn.Armor);

		ImVec2 armorTextSize = ImGui::CalcTextSize(armorText.c_str());

		ImVec2 armorTextPos = { Rect.x + Rect.z / 2 - armorTextSize.x / 2, Rect.y - 12 - armorTextSize.y - 3 }; 

		ImGui::GetForegroundDrawList()->AddText(armorTextPos, ImColor(255, 255, 255, 255), armorText.c_str());
	}

	if (MenuConfig::ShowWeaponESP)
	{
		if (!Entity.Pawn.PlayerWeapon.empty())
			Entity.Pawn.PlayerWeapon[0] = toupper(Entity.Pawn.PlayerWeapon[0]);

		ImVec2 textSize = ImGui::CalcTextSize(Entity.Pawn.PlayerWeapon.c_str());
		ImVec2 textPosition;
		textPosition.x = Rect.x + (Rect.z - textSize.x) * 0.5f;
		textPosition.y = Rect.y + Rect.w;

		ImGui::GetForegroundDrawList()->AddText(textPosition, ImColor(255, 255, 255, 255), Entity.Pawn.PlayerWeapon.c_str());
	}


	if (MenuConfig::ShowDistance)
	{
		float DistanceToLocal = LocalEntity.Pawn.vOldOrigin.DistanceTo(Entity.Pawn.vOldOrigin);
		float Distance2_Meters = DistanceToLocal * 0.0254;

		int RoundedDistance = static_cast<int>(Distance2_Meters);

		std::string distanceText = std::to_string(RoundedDistance) + "m";

		ImVec2 distanceTextSize = ImGui::CalcTextSize(distanceText.c_str());

		ImVec2 distanceTextPos = { Rect.x + Rect.z / 2 - distanceTextSize.x / 2, Rect.y - 12 - distanceTextSize.y - 3 };

		ImGui::GetForegroundDrawList()->AddText(distanceTextPos, ImColor(255, 255, 255, 255), distanceText.c_str());
	}

	return TRUE;
}