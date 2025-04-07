#pragma once

class Boundary {

public:
	float left;
	float right;
	float front;
	float back;

	Boundary(float l, float r, float f, float b) :
		left(l),
		right(r),
		front(f),
		back(b)
	{
	}
};