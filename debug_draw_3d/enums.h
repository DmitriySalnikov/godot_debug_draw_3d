#pragma once

enum BlockPosition : int {
	LeftTop = 0,
	RightTop = 1,
	LeftBottom = 2,
	RightBottom = 3,
};

enum GraphTextFlags : int {
	None = 0,
	Current = 1 << 0,
	Avarage = 1 << 1,
	Max = 1 << 2,
	Min = 1 << 3,
	All = Current | Avarage | Max | Min
};
