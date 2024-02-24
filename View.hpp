#pragma once
#include "Math.hpp"

class CView
{
public:
	float(*Matrix)[4][4];
	DWORD64 MatrixAddress;

    inline BOOL WorldToScreen(const Vec3& Pos, Vec2& ToPos)
    {
        float(*ViewMatrix)[4] = *Matrix;

        const float w = ViewMatrix[3][0] * Pos.x + ViewMatrix[3][1] * Pos.y + ViewMatrix[3][2] * Pos.z + ViewMatrix[3][3];

        if (w <= 0.01)
            return FALSE;

        const float invW = 1.0f / w;
        const float SightX = ImGui::GetIO().DisplaySize.x * 0.5f;
        const float SightY = ImGui::GetIO().DisplaySize.y * 0.5f;

        ToPos.x = SightX + ((ViewMatrix[0][0] * Pos.x + ViewMatrix[0][1] * Pos.y + ViewMatrix[0][2] * Pos.z + ViewMatrix[0][3]) * invW * SightX);
        ToPos.y = SightY - ((ViewMatrix[1][0] * Pos.x + ViewMatrix[1][1] * Pos.y + ViewMatrix[1][2] * Pos.z + ViewMatrix[1][3]) * invW * SightY);

        return TRUE;
    }
};
