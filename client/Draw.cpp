#include "Draw.h"
#include "imgui.h"
#include "Vector.h"
#include "ViewMatrix.h"
#include <dwmapi.h>



void Draw::drawPlayerBox(const Vector2& topLeft, const Vector2& bottomRight, ImColor color)
{
	//ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), color, 0.0F, ImDrawFlags_None);
	ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(topLeft.x, topLeft.y), 10.0F, color);
}

