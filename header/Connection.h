#pragma once
#include <array>

enum class Connection
{
	DO_NOT_CARE, ///< The connection is irrelevant (no Glyph objects should ever be assigned this value, it is for usage in functions)
	EMPTY,       ///< The empty connection, where nothing is passing through this edge
	DIAG_BOTH,   ///< There are 2 strands, which pass over each other diagonally, at the the midpoint of the edge
	ORTHO_BOTH,  ///< There are 2 strands, which are parallel to each other and are orthogonal to the edge
	DIAG_FRONT,  ///< Only the strand from \c DIAG_BOTH that is visually "on top" or "in front"
	DIAG_BACK,   ///< Only the strand from \c DIAG_BOTH that is visually "on the bottom" or "behind"
	ORTHO_UP,    ///< Only the strand from \c ORTHO_BOTH that is running horizontally, on the upper half
	ORTHO_DOWN,  ///< Only the strand from \c ORTHO_BOTH that is running horizontally, on the lower half
	ORTHO_LEFT,  ///< Only the strand from \c ORTHO_BOTH that is running vertical, on the left half
	ORTHO_RIGHT, ///< Only the strand from \c ORTHO_BOTH that is running vertical, on the right half
};

struct ConnectionTransformations
{
	using enum Connection;

	static constexpr std::array rotate_90{ DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_RIGHT, ORTHO_LEFT, ORTHO_UP, ORTHO_DOWN };
	static constexpr std::array rotate_180{ DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_DOWN, ORTHO_UP, ORTHO_RIGHT, ORTHO_LEFT };

	static constexpr std::array mirror_x{ DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_DOWN, ORTHO_UP, ORTHO_LEFT, ORTHO_RIGHT };
	static constexpr std::array mirror_y{ DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_UP, ORTHO_DOWN, ORTHO_RIGHT, ORTHO_LEFT };

	static constexpr std::array mirror_forward_diagonal{ DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_RIGHT, ORTHO_LEFT, ORTHO_DOWN, ORTHO_UP };
	static constexpr std::array mirror_backward_diagonal{ DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_LEFT, ORTHO_RIGHT, ORTHO_UP, ORTHO_DOWN };
};

constexpr Connection rotate_90(Connection input) { return ConnectionTransformations::rotate_90[static_cast<int>(input)]; }
constexpr Connection rotate_180(Connection input) { return ConnectionTransformations::rotate_180[static_cast<int>(input)]; }

constexpr Connection mirror_x(Connection input) { return ConnectionTransformations::mirror_x[static_cast<int>(input)]; }
constexpr Connection mirror_y(Connection input) { return ConnectionTransformations::mirror_y[static_cast<int>(input)]; }

constexpr Connection mirror_forward_diagonal(Connection input) { return ConnectionTransformations::mirror_forward_diagonal[static_cast<int>(input)]; }
constexpr Connection mirror_backward_diagonal(Connection input) { return ConnectionTransformations::mirror_backward_diagonal[static_cast<int>(input)]; }
