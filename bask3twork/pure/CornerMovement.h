#pragma once

enum class Corner
{
	upper_left,
	upper_right,
	lower_left,
	lower_right,
};

enum class Movement
{
	up,
	down,
	left,
	right,
};

struct CornerMovement
{
	Corner corner;
	Movement movement;

	/// This operator allows CornerMovement to be used in switch statements
	constexpr operator int() const
	{
		return (static_cast<int>(corner) << 2) | (static_cast<int>(movement));
	}	
};

constexpr CornerMovement operator|(Corner corner, Movement movement)
{
	return { corner, movement };
}
