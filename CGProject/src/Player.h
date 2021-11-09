#pragma once

#include "Matrix.h"


class Player {
public:
	Player();
	~Player();
	const Matrix& transform() const { return Transform; }
	Matrix Transform;
};