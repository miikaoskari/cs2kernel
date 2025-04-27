#pragma once

#include "Vector.h"
#include "ViewMatrix.h"

bool worldToScreen(const Vector3& pos, Vector2& toPos);

bool project3DWorldTo2D(const Vector3& worldPos, Vector2& screenPos, const ViewMatrix& viewMatrix);
