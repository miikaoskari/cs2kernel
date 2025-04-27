#pragma once

struct ViewMatrix
{
	float matrix[4][4];

	float* operator[](int index)
	{
		return matrix[index];
	}

	const float* operator[](int index) const
	{
		return matrix[index];
	}

	ViewMatrix()
	{
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				matrix[i][j] = 0.0f;
	}
};
