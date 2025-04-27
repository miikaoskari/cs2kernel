#include "View.h"
#include "ViewMatrix.h"
#include "Overlay.h"

bool worldToScreen(const Vector3& pos, Vector2& toPos)
{
	float view = 0.f;
	float sightX = ws.x / 2, sightY = ws.y / 2;

	ViewMatrix matrix;
	view = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	if (view <= 0.01)
		return false;

	toPos.x = sightX + (matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3]) / view * sightX;
	toPos.y = sightY - (matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3]) / view * sightY;

	return true;
}

bool project3DWorldTo2D(const Vector3& worldPos, Vector2& screenPos, const ViewMatrix& viewMatrix)
{
	float clip_x = worldPos.x * viewMatrix.matrix[0][0] + worldPos.y * viewMatrix.matrix[0][1] + worldPos.z * viewMatrix.matrix[0][2] + viewMatrix.matrix[0][3];
	float clip_y = worldPos.x * viewMatrix.matrix[1][0] + worldPos.y * viewMatrix.matrix[1][1] + worldPos.z * viewMatrix.matrix[1][2] + viewMatrix.matrix[1][3];
	float clip_z = worldPos.x * viewMatrix.matrix[2][0] + worldPos.y * viewMatrix.matrix[2][1] + worldPos.z * viewMatrix.matrix[2][2] + viewMatrix.matrix[2][3];
	float clip_w = worldPos.x * viewMatrix.matrix[3][0] + worldPos.y * viewMatrix.matrix[3][1] + worldPos.z * viewMatrix.matrix[3][2] + viewMatrix.matrix[3][3];

	if (clip_w < 0.1f) // To prevent division by zero and culling objects behind the camera
		return false;

	// Normalize to NDC (Normalized Device Coordinates)
	float ndc_x = clip_x / clip_w;
	float ndc_y = clip_y / clip_w;

	// Convert to screen coordinates
	screenPos.x = (ws.x / 2 * ndc_x) + (ndc_x + ws.x / 2);
	screenPos.y = -(ws.y / 2 * ndc_y) + (ndc_y + ws.y / 2);

	return true;
}
