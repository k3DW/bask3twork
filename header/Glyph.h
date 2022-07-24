#pragma once
#include "Constants.h"
#include "Connection.h"
/// \file

/// The bit flag for each of the properties of a Glyph object
enum class GlyphFlag : ull {
	NONE	= 0,			///< No flag
	UP		= 0b1111 <<  0,	///< The upper side 4-bit mask
	DOWN	= 0b1111 <<  4, ///< The lower side 4-bit mask
	LEFT	= 0b1111 <<  8, ///< The left side 4-bit mask
	RIGHT	= 0b1111 << 12, ///< The right side 4-bit mask
	SA_ROT4  = 1LL << 16, ///< Is this Glyph the same after rotating by 90 degrees
	SA_ROT2  = 1LL << 17, ///< Is this Glyph the same after rotating by 180 degrees
	SA_MIRX  = 1LL << 18, ///< Is this Glyph the same after mirroring across the horizontal line
	SA_MIRY  = 1LL << 19, ///< Is this Glyph the same after mirroring across the vertical line
	CT_ROT4U = 1LL << 20, ///< Can this Glyph connect to its 90-degree-rotated counterpart on its own upper side
	CT_ROT4D = 1LL << 21, ///< Can this Glyph connect to its 90-degree-rotated counterpart on its own lower side
	CT_ROT4L = 1LL << 22, ///< Can this Glyph connect to its 90-degree-rotated counterpart on its own left side
	CT_ROT4R = 1LL << 23, ///< Can this Glyph connect to its 90-degree-rotated counterpart on its own right side
	CT_ROT2U = 1LL << 24, ///< Can this Glyph connect to its 180-degree-rotated counterpart on its own upper side
	CT_ROT2D = 1LL << 25, ///< Can this Glyph connect to its 180-degree-rotated counterpart on its own lower side
	CT_ROT2L = 1LL << 26, ///< Can this Glyph connect to its 180-degree-rotated counterpart on its own left side
	CT_ROT2R = 1LL << 27, ///< Can this Glyph connect to its 180-degree-rotated counterpart on its own right side
	CT_MIRU  = 1LL << 28, ///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its upper side
	CT_MIRD  = 1LL << 29, ///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its lower side
	CT_MIRL  = 1LL << 30, ///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its left side
	CT_MIRR  = 1LL << 31, ///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its right side
	SA_MIRFD = 1LL << 32, ///< Is this Glyph the same after mirroring across the forward diagonal
	SA_MIRBD = 1LL << 33, ///< Is this Glyph the same after mirroring across the backward diagonal
	CT_SELFU = 1LL << 34, ///< Can this Glyph connect to itself on the upper side of the Glyph
	CT_SELFD = 1LL << 35, ///< Can this Glyph connect to itself on the lower side of the Glyph
	CT_SELFL = 1LL << 36, ///< Can this Glyph connect to itself on the left side of the Glyph
	CT_SELFR = 1LL << 37, ///< Can this Glyph connect to itself on the right side of the Glyph

	COND_MASK = 0b1111111111111111111111LL << 16, ///< The mask of all the non-Side flags
};
constexpr inline GlyphFlag operator|(GlyphFlag flag1, GlyphFlag flag2)
/// Allows GlyphFlag values to have \c operator| used on them, to generate new GlyphFlag values
{
	return static_cast<GlyphFlag>(static_cast<ull>(flag1) | static_cast<ull>(flag2));
}
constexpr inline GlyphFlag operator&(GlyphFlag flag1, GlyphFlag flag2)
/// Allows GlyphFlag values to have \c operator& used on them, to generate new GlyphFlag values
{
	return static_cast<GlyphFlag>(static_cast<ull>(flag1) & static_cast<ull>(flag2));
}
constexpr inline GlyphFlag toFlag(Connection con, GlyphFlag sideFlag)
/// Converts a Connection on a specific side to a GlyphFlag of that specific Connection on the side
{
	return static_cast<GlyphFlag>((static_cast<ull>(con) * 0b1000100010001) & static_cast<ull>(sideFlag));
}
constexpr GlyphFlag operator*(GlyphFlag flag, bool b)
{
	return static_cast<GlyphFlag>(static_cast<ull>(flag) * b);
}

/** A struct to store the information for all the possible glyphs in the Celtic Knots font,
	where each individual flag contained within corresponds to a GlyphFlag */
struct Glyph {
	const wxString chr; ///< The character that gets displayed

	const Glyph* const rotated4;	///< The pointer to the Glyph, of this Glyph rotated by 90 degrees clockwise
	const Glyph* const rotated2;	///< The pointer to the Glyph, of this Glyph rotated by 180 degrees clockwise
	const Glyph* const mirroredX;	///< The pointer to the Glyph, of this Glyph mirrored across the horizontal line
	const Glyph* const mirroredY;	///< The pointer to the Glyph, of this Glyph mirrored across the veritcal line
	const Glyph* const mirroredFD;	///< The pointer to the Glyph, of this Glyph mirrored across the forward diagonal line
	const Glyph* const mirroredBD;	///< The pointer to the Glyph, of this Glyph mirrored across the backward diagonal line

	Connection up;
	Connection down;
	Connection left;
	Connection right;

	GlyphFlag flags;	///< The total signature of this glyph, in a union with all the other flags to access the individual flags simultaneously

	/// All the \c connectTo____ flags are determined from the other parameters, but the other parameters are given explicitly
	Glyph(wxString chr, const Glyph* rotated4, const Glyph* rotated2, const Glyph* mirroredX, const Glyph* mirroredY, const Glyph* mirroredFD, const Glyph* mirroredBD,
	const Connection up, const Connection down, const Connection left, const Connection right,
	unsigned int sameAfterRotate4, unsigned int sameAfterRotate2, unsigned int sameAfterMirrorX, unsigned int sameAfterMirrorY, unsigned int sameAfterMirrorFwdDiag, unsigned int sameAfterMirrorBackDiag) :
		chr{ chr }, rotated4{ rotated4 }, rotated2{ rotated2 }, mirroredX{ mirroredX }, mirroredY{ mirroredY }, mirroredFD{ mirroredFD }, mirroredBD{ mirroredBD },
		up{ up }, down{ down }, left{ left }, right{ right },
		flags
		{
			(GlyphFlag::SA_ROT4 * sameAfterRotate4) |
			(GlyphFlag::SA_ROT2 * sameAfterRotate2) |
			(GlyphFlag::SA_MIRX * sameAfterMirrorX) |
			(GlyphFlag::SA_MIRY * sameAfterMirrorY) |
			(GlyphFlag::CT_ROT4U * (up    == rotate_90(right))) |
			(GlyphFlag::CT_ROT4D * (down  == rotate_90(left)))  |
			(GlyphFlag::CT_ROT4L * (left  == rotate_90(up)))    |
			(GlyphFlag::CT_ROT4R * (right == rotate_90(down)))  |
			(GlyphFlag::CT_ROT2U * (up    == rotate_180(up)))    |
			(GlyphFlag::CT_ROT2D * (down  == rotate_180(down)))  |
			(GlyphFlag::CT_ROT2L * (left  == rotate_180(left)))  |
			(GlyphFlag::CT_ROT2R * (right == rotate_180(right))) |
			(GlyphFlag::CT_MIRU * (up    == mirror_x(up)))    |
			(GlyphFlag::CT_MIRD * (down  == mirror_x(down)))  |
			(GlyphFlag::CT_MIRL * (left  == mirror_y(left)))  |
			(GlyphFlag::CT_MIRR * (right == mirror_y(right))) |
			(GlyphFlag::SA_MIRFD * sameAfterMirrorFwdDiag)  |
			(GlyphFlag::SA_MIRBD * sameAfterMirrorBackDiag) |
			(GlyphFlag::CT_SELFU * (up == down))    |
			(GlyphFlag::CT_SELFD * (up == down))    |
			(GlyphFlag::CT_SELFL * (left == right)) |
			(GlyphFlag::CT_SELFR * (left == right))
		}
	{}

	static constexpr int TOTAL = 190; ///< The total number of glyphs used
};
using GlyphVec1 = std::vector<const Glyph*>;	///< Syntactic sugar for a 1D vector of pointers to \c const \c Glyph
using GlyphVec2 = std::vector<GlyphVec1>;		///< Syntactic sugar for a 2D vector of pointers to \c const \c Glyph

/// The array of every Glyph in the program (generated in Excel), the only place where a Glyph object is initialized;
/// every other place a Glyph is referenced in the whole codebase is actually a pointer to one of the Glyphs in \c AllGlyphs.
const inline std::array<Glyph, Glyph::TOTAL> AllGlyphs{ {
	{ wxUniChar(32), &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 1, 1, 1, 1, 1, 1 },
	{ wxUniChar(33), &AllGlyphs[94], &AllGlyphs[1], &AllGlyphs[1], &AllGlyphs[1], &AllGlyphs[94], &AllGlyphs[94], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(34), &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1, 1, 1 },
	{ wxUniChar(35), &AllGlyphs[32], &AllGlyphs[3], &AllGlyphs[3], &AllGlyphs[3], &AllGlyphs[32], &AllGlyphs[32], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(36), &AllGlyphs[54], &AllGlyphs[50], &AllGlyphs[50], &AllGlyphs[4], &AllGlyphs[54], &AllGlyphs[38], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(37), &AllGlyphs[40], &AllGlyphs[52], &AllGlyphs[52], &AllGlyphs[5], &AllGlyphs[40], &AllGlyphs[39], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(38), &AllGlyphs[43], &AllGlyphs[41], &AllGlyphs[53], &AllGlyphs[10], &AllGlyphs[28], &AllGlyphs[42], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(39), &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 1, 1, 1, 1, 1, 1 },
	{ wxUniChar(40), &AllGlyphs[9], &AllGlyphs[48], &AllGlyphs[47], &AllGlyphs[9], &AllGlyphs[48], &AllGlyphs[8], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(41), &AllGlyphs[48], &AllGlyphs[47], &AllGlyphs[48], &AllGlyphs[8], &AllGlyphs[9], &AllGlyphs[47], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(42), &AllGlyphs[28], &AllGlyphs[53], &AllGlyphs[41], &AllGlyphs[6], &AllGlyphs[43], &AllGlyphs[45], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(43), &AllGlyphs[93], &AllGlyphs[91], &AllGlyphs[93], &AllGlyphs[63], &AllGlyphs[11], &AllGlyphs[91], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(44), &AllGlyphs[85], &AllGlyphs[74], &AllGlyphs[75], &AllGlyphs[77], &AllGlyphs[23], &AllGlyphs[73], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(45), &AllGlyphs[61], &AllGlyphs[29], &AllGlyphs[29], &AllGlyphs[13], &AllGlyphs[61], &AllGlyphs[59], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(46), &AllGlyphs[25], &AllGlyphs[27], &AllGlyphs[76], &AllGlyphs[15], &AllGlyphs[79], &AllGlyphs[16], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(47), &AllGlyphs[79], &AllGlyphs[76], &AllGlyphs[27], &AllGlyphs[14], &AllGlyphs[25], &AllGlyphs[80], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(48), &AllGlyphs[15], &AllGlyphs[79], &AllGlyphs[80], &AllGlyphs[25], &AllGlyphs[27], &AllGlyphs[14], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(49), &AllGlyphs[64], &AllGlyphs[17], &AllGlyphs[17], &AllGlyphs[17], &AllGlyphs[64], &AllGlyphs[64], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(50), &AllGlyphs[19], &AllGlyphs[18], &AllGlyphs[18], &AllGlyphs[18], &AllGlyphs[19], &AllGlyphs[19], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(51), &AllGlyphs[18], &AllGlyphs[19], &AllGlyphs[19], &AllGlyphs[19], &AllGlyphs[18], &AllGlyphs[18], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(52), &AllGlyphs[86], &AllGlyphs[82], &AllGlyphs[82], &AllGlyphs[20], &AllGlyphs[86], &AllGlyphs[70], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(53), &AllGlyphs[72], &AllGlyphs[84], &AllGlyphs[84], &AllGlyphs[21], &AllGlyphs[72], &AllGlyphs[71], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(54), &AllGlyphs[78], &AllGlyphs[89], &AllGlyphs[89], &AllGlyphs[22], &AllGlyphs[78], &AllGlyphs[66], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(55), &AllGlyphs[75], &AllGlyphs[73], &AllGlyphs[85], &AllGlyphs[24], &AllGlyphs[12], &AllGlyphs[74], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(56), &AllGlyphs[12], &AllGlyphs[85], &AllGlyphs[73], &AllGlyphs[23], &AllGlyphs[75], &AllGlyphs[77], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(57), &AllGlyphs[27], &AllGlyphs[80], &AllGlyphs[79], &AllGlyphs[16], &AllGlyphs[15], &AllGlyphs[76], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(58), &AllGlyphs[31], &AllGlyphs[30], &AllGlyphs[31], &AllGlyphs[44], &AllGlyphs[26], &AllGlyphs[30], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(59), &AllGlyphs[80], &AllGlyphs[14], &AllGlyphs[15], &AllGlyphs[76], &AllGlyphs[16], &AllGlyphs[79], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(60), &AllGlyphs[53], &AllGlyphs[42], &AllGlyphs[43], &AllGlyphs[45], &AllGlyphs[6], &AllGlyphs[41], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(61), &AllGlyphs[59], &AllGlyphs[13], &AllGlyphs[13], &AllGlyphs[29], &AllGlyphs[59], &AllGlyphs[61], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(62), &AllGlyphs[44], &AllGlyphs[26], &AllGlyphs[44], &AllGlyphs[31], &AllGlyphs[30], &AllGlyphs[26], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(63), &AllGlyphs[30], &AllGlyphs[44], &AllGlyphs[26], &AllGlyphs[30], &AllGlyphs[44], &AllGlyphs[31], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(64), &AllGlyphs[3], &AllGlyphs[32], &AllGlyphs[32], &AllGlyphs[32], &AllGlyphs[3], &AllGlyphs[3], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(65), &AllGlyphs[55], &AllGlyphs[36], &AllGlyphs[33], &AllGlyphs[36], &AllGlyphs[56], &AllGlyphs[55], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(66), &AllGlyphs[62], &AllGlyphs[46], &AllGlyphs[34], &AllGlyphs[46], &AllGlyphs[57], &AllGlyphs[62], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(67), &AllGlyphs[58], &AllGlyphs[49], &AllGlyphs[37], &AllGlyphs[58], &AllGlyphs[49], &AllGlyphs[35], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(68), &AllGlyphs[56], &AllGlyphs[33], &AllGlyphs[36], &AllGlyphs[33], &AllGlyphs[55], &AllGlyphs[56], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(69), &AllGlyphs[35], &AllGlyphs[58], &AllGlyphs[35], &AllGlyphs[49], &AllGlyphs[37], &AllGlyphs[58], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(70), &AllGlyphs[4], &AllGlyphs[54], &AllGlyphs[38], &AllGlyphs[54], &AllGlyphs[50], &AllGlyphs[4], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(71), &AllGlyphs[5], &AllGlyphs[40], &AllGlyphs[39], &AllGlyphs[40], &AllGlyphs[52], &AllGlyphs[5], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(72), &AllGlyphs[52], &AllGlyphs[39], &AllGlyphs[40], &AllGlyphs[39], &AllGlyphs[5], &AllGlyphs[52], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(73), &AllGlyphs[45], &AllGlyphs[6], &AllGlyphs[10], &AllGlyphs[53], &AllGlyphs[42], &AllGlyphs[28], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(74), &AllGlyphs[10], &AllGlyphs[28], &AllGlyphs[45], &AllGlyphs[43], &AllGlyphs[41], &AllGlyphs[6], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(75), &AllGlyphs[41], &AllGlyphs[45], &AllGlyphs[28], &AllGlyphs[42], &AllGlyphs[10], &AllGlyphs[53], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(76), &AllGlyphs[26], &AllGlyphs[31], &AllGlyphs[30], &AllGlyphs[26], &AllGlyphs[31], &AllGlyphs[44], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(77), &AllGlyphs[6], &AllGlyphs[43], &AllGlyphs[42], &AllGlyphs[28], &AllGlyphs[53], &AllGlyphs[10], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(78), &AllGlyphs[57], &AllGlyphs[34], &AllGlyphs[46], &AllGlyphs[34], &AllGlyphs[62], &AllGlyphs[57], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(79), &AllGlyphs[8], &AllGlyphs[9], &AllGlyphs[8], &AllGlyphs[48], &AllGlyphs[47], &AllGlyphs[9], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(80), &AllGlyphs[47], &AllGlyphs[8], &AllGlyphs[9], &AllGlyphs[47], &AllGlyphs[8], &AllGlyphs[48], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(81), &AllGlyphs[37], &AllGlyphs[35], &AllGlyphs[58], &AllGlyphs[37], &AllGlyphs[35], &AllGlyphs[49], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(82), &AllGlyphs[38], &AllGlyphs[4], &AllGlyphs[4], &AllGlyphs[50], &AllGlyphs[38], &AllGlyphs[54], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(83), &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1, 1, 1 },
	{ wxUniChar(84), &AllGlyphs[39], &AllGlyphs[5], &AllGlyphs[5], &AllGlyphs[52], &AllGlyphs[39], &AllGlyphs[40], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(85), &AllGlyphs[42], &AllGlyphs[10], &AllGlyphs[6], &AllGlyphs[41], &AllGlyphs[45], &AllGlyphs[43], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(86), &AllGlyphs[50], &AllGlyphs[38], &AllGlyphs[54], &AllGlyphs[38], &AllGlyphs[4], &AllGlyphs[50], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(87), &AllGlyphs[36], &AllGlyphs[56], &AllGlyphs[56], &AllGlyphs[55], &AllGlyphs[36], &AllGlyphs[33], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(88), &AllGlyphs[33], &AllGlyphs[55], &AllGlyphs[55], &AllGlyphs[56], &AllGlyphs[33], &AllGlyphs[36], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(89), &AllGlyphs[34], &AllGlyphs[62], &AllGlyphs[62], &AllGlyphs[57], &AllGlyphs[34], &AllGlyphs[46], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(90), &AllGlyphs[49], &AllGlyphs[37], &AllGlyphs[49], &AllGlyphs[35], &AllGlyphs[58], &AllGlyphs[37], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(91), &AllGlyphs[13], &AllGlyphs[61], &AllGlyphs[59], &AllGlyphs[61], &AllGlyphs[29], &AllGlyphs[13], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(92), &AllGlyphs[92], &AllGlyphs[60], &AllGlyphs[60], &AllGlyphs[60], &AllGlyphs[92], &AllGlyphs[92], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(93), &AllGlyphs[29], &AllGlyphs[59], &AllGlyphs[61], &AllGlyphs[59], &AllGlyphs[13], &AllGlyphs[29], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(94), &AllGlyphs[46], &AllGlyphs[57], &AllGlyphs[57], &AllGlyphs[62], &AllGlyphs[46], &AllGlyphs[34], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(95), &AllGlyphs[11], &AllGlyphs[93], &AllGlyphs[91], &AllGlyphs[11], &AllGlyphs[93], &AllGlyphs[63], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(96), &AllGlyphs[17], &AllGlyphs[64], &AllGlyphs[64], &AllGlyphs[64], &AllGlyphs[17], &AllGlyphs[17], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(97), &AllGlyphs[87], &AllGlyphs[68], &AllGlyphs[65], &AllGlyphs[68], &AllGlyphs[88], &AllGlyphs[87], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(98), &AllGlyphs[22], &AllGlyphs[78], &AllGlyphs[66], &AllGlyphs[78], &AllGlyphs[89], &AllGlyphs[22], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(99), &AllGlyphs[90], &AllGlyphs[81], &AllGlyphs[69], &AllGlyphs[90], &AllGlyphs[81], &AllGlyphs[67], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(100), &AllGlyphs[88], &AllGlyphs[65], &AllGlyphs[68], &AllGlyphs[65], &AllGlyphs[87], &AllGlyphs[88], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(101), &AllGlyphs[67], &AllGlyphs[90], &AllGlyphs[67], &AllGlyphs[81], &AllGlyphs[69], &AllGlyphs[90], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(102), &AllGlyphs[20], &AllGlyphs[86], &AllGlyphs[70], &AllGlyphs[86], &AllGlyphs[82], &AllGlyphs[20], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(103), &AllGlyphs[21], &AllGlyphs[72], &AllGlyphs[71], &AllGlyphs[72], &AllGlyphs[84], &AllGlyphs[21], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(104), &AllGlyphs[84], &AllGlyphs[71], &AllGlyphs[72], &AllGlyphs[71], &AllGlyphs[21], &AllGlyphs[84], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(105), &AllGlyphs[77], &AllGlyphs[23], &AllGlyphs[24], &AllGlyphs[85], &AllGlyphs[74], &AllGlyphs[12], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(106), &AllGlyphs[24], &AllGlyphs[12], &AllGlyphs[77], &AllGlyphs[75], &AllGlyphs[73], &AllGlyphs[23], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(107), &AllGlyphs[73], &AllGlyphs[77], &AllGlyphs[12], &AllGlyphs[74], &AllGlyphs[24], &AllGlyphs[85], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(108), &AllGlyphs[16], &AllGlyphs[15], &AllGlyphs[14], &AllGlyphs[27], &AllGlyphs[80], &AllGlyphs[25], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(109), &AllGlyphs[23], &AllGlyphs[75], &AllGlyphs[74], &AllGlyphs[12], &AllGlyphs[85], &AllGlyphs[24], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(110), &AllGlyphs[89], &AllGlyphs[66], &AllGlyphs[78], &AllGlyphs[66], &AllGlyphs[22], &AllGlyphs[89], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(111), &AllGlyphs[76], &AllGlyphs[16], &AllGlyphs[25], &AllGlyphs[80], &AllGlyphs[14], &AllGlyphs[27], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(112), &AllGlyphs[14], &AllGlyphs[25], &AllGlyphs[16], &AllGlyphs[79], &AllGlyphs[76], &AllGlyphs[15], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(113), &AllGlyphs[69], &AllGlyphs[67], &AllGlyphs[90], &AllGlyphs[69], &AllGlyphs[67], &AllGlyphs[81], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(114), &AllGlyphs[70], &AllGlyphs[20], &AllGlyphs[20], &AllGlyphs[82], &AllGlyphs[70], &AllGlyphs[86], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(115), &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 1, 1, 1, 1, 1, 1 },
	{ wxUniChar(116), &AllGlyphs[71], &AllGlyphs[21], &AllGlyphs[21], &AllGlyphs[84], &AllGlyphs[71], &AllGlyphs[72], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(117), &AllGlyphs[74], &AllGlyphs[24], &AllGlyphs[23], &AllGlyphs[73], &AllGlyphs[77], &AllGlyphs[75], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(118), &AllGlyphs[82], &AllGlyphs[70], &AllGlyphs[86], &AllGlyphs[70], &AllGlyphs[20], &AllGlyphs[82], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(119), &AllGlyphs[68], &AllGlyphs[88], &AllGlyphs[88], &AllGlyphs[87], &AllGlyphs[68], &AllGlyphs[65], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(120), &AllGlyphs[65], &AllGlyphs[87], &AllGlyphs[87], &AllGlyphs[88], &AllGlyphs[65], &AllGlyphs[68], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(121), &AllGlyphs[66], &AllGlyphs[22], &AllGlyphs[22], &AllGlyphs[89], &AllGlyphs[66], &AllGlyphs[78], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(122), &AllGlyphs[81], &AllGlyphs[69], &AllGlyphs[81], &AllGlyphs[67], &AllGlyphs[90], &AllGlyphs[69], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(123), &AllGlyphs[63], &AllGlyphs[11], &AllGlyphs[63], &AllGlyphs[93], &AllGlyphs[91], &AllGlyphs[11], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(124), &AllGlyphs[60], &AllGlyphs[92], &AllGlyphs[92], &AllGlyphs[92], &AllGlyphs[60], &AllGlyphs[60], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(125), &AllGlyphs[91], &AllGlyphs[63], &AllGlyphs[11], &AllGlyphs[91], &AllGlyphs[63], &AllGlyphs[93], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(126), &AllGlyphs[1], &AllGlyphs[94], &AllGlyphs[94], &AllGlyphs[94], &AllGlyphs[1], &AllGlyphs[1], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(161), &AllGlyphs[98], &AllGlyphs[96], &AllGlyphs[95], &AllGlyphs[96], &AllGlyphs[97], &AllGlyphs[98], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(162), &AllGlyphs[97], &AllGlyphs[95], &AllGlyphs[96], &AllGlyphs[95], &AllGlyphs[98], &AllGlyphs[97], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(163), &AllGlyphs[95], &AllGlyphs[98], &AllGlyphs[98], &AllGlyphs[97], &AllGlyphs[95], &AllGlyphs[96], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(164), &AllGlyphs[96], &AllGlyphs[97], &AllGlyphs[97], &AllGlyphs[98], &AllGlyphs[96], &AllGlyphs[95], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(165), &AllGlyphs[100], &AllGlyphs[101], &AllGlyphs[102], &AllGlyphs[100], &AllGlyphs[101], &AllGlyphs[99], Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(166), &AllGlyphs[101], &AllGlyphs[102], &AllGlyphs[101], &AllGlyphs[99], &AllGlyphs[100], &AllGlyphs[102], Connection::DIAG_BACK, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_FRONT, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(167), &AllGlyphs[102], &AllGlyphs[99], &AllGlyphs[100], &AllGlyphs[102], &AllGlyphs[99], &AllGlyphs[101], Connection::DIAG_BOTH, Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::DIAG_BACK, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(168), &AllGlyphs[99], &AllGlyphs[100], &AllGlyphs[99], &AllGlyphs[101], &AllGlyphs[102], &AllGlyphs[100], Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::DIAG_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(169), &AllGlyphs[104], &AllGlyphs[105], &AllGlyphs[105], &AllGlyphs[103], &AllGlyphs[104], &AllGlyphs[106], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(170), &AllGlyphs[105], &AllGlyphs[106], &AllGlyphs[104], &AllGlyphs[106], &AllGlyphs[103], &AllGlyphs[105], Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(171), &AllGlyphs[106], &AllGlyphs[103], &AllGlyphs[103], &AllGlyphs[105], &AllGlyphs[106], &AllGlyphs[104], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(172), &AllGlyphs[103], &AllGlyphs[104], &AllGlyphs[106], &AllGlyphs[104], &AllGlyphs[105], &AllGlyphs[103], Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(174), &AllGlyphs[108], &AllGlyphs[109], &AllGlyphs[108], &AllGlyphs[110], &AllGlyphs[107], &AllGlyphs[109], Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(175), &AllGlyphs[109], &AllGlyphs[110], &AllGlyphs[107], &AllGlyphs[109], &AllGlyphs[110], &AllGlyphs[108], Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_FRONT, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(176), &AllGlyphs[110], &AllGlyphs[107], &AllGlyphs[110], &AllGlyphs[108], &AllGlyphs[109], &AllGlyphs[107], Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(177), &AllGlyphs[107], &AllGlyphs[108], &AllGlyphs[109], &AllGlyphs[107], &AllGlyphs[108], &AllGlyphs[110], Connection::EMPTY, Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_FRONT, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(178), &AllGlyphs[112], &AllGlyphs[113], &AllGlyphs[113], &AllGlyphs[111], &AllGlyphs[112], &AllGlyphs[114], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(179), &AllGlyphs[113], &AllGlyphs[114], &AllGlyphs[112], &AllGlyphs[114], &AllGlyphs[111], &AllGlyphs[113], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(180), &AllGlyphs[114], &AllGlyphs[111], &AllGlyphs[111], &AllGlyphs[113], &AllGlyphs[114], &AllGlyphs[112], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(181), &AllGlyphs[111], &AllGlyphs[112], &AllGlyphs[114], &AllGlyphs[112], &AllGlyphs[113], &AllGlyphs[111], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(182), &AllGlyphs[116], &AllGlyphs[117], &AllGlyphs[116], &AllGlyphs[118], &AllGlyphs[115], &AllGlyphs[117], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(184), &AllGlyphs[117], &AllGlyphs[118], &AllGlyphs[115], &AllGlyphs[117], &AllGlyphs[118], &AllGlyphs[116], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(185), &AllGlyphs[118], &AllGlyphs[115], &AllGlyphs[118], &AllGlyphs[116], &AllGlyphs[117], &AllGlyphs[115], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(186), &AllGlyphs[115], &AllGlyphs[116], &AllGlyphs[117], &AllGlyphs[115], &AllGlyphs[116], &AllGlyphs[118], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(187), &AllGlyphs[122], &AllGlyphs[120], &AllGlyphs[119], &AllGlyphs[120], &AllGlyphs[121], &AllGlyphs[122], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(188), &AllGlyphs[121], &AllGlyphs[119], &AllGlyphs[120], &AllGlyphs[119], &AllGlyphs[122], &AllGlyphs[121], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(189), &AllGlyphs[119], &AllGlyphs[122], &AllGlyphs[122], &AllGlyphs[121], &AllGlyphs[119], &AllGlyphs[120], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(190), &AllGlyphs[120], &AllGlyphs[121], &AllGlyphs[121], &AllGlyphs[122], &AllGlyphs[120], &AllGlyphs[119], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(191), &AllGlyphs[126], &AllGlyphs[124], &AllGlyphs[123], &AllGlyphs[124], &AllGlyphs[125], &AllGlyphs[126], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(192), &AllGlyphs[125], &AllGlyphs[123], &AllGlyphs[124], &AllGlyphs[123], &AllGlyphs[126], &AllGlyphs[125], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(193), &AllGlyphs[123], &AllGlyphs[126], &AllGlyphs[126], &AllGlyphs[125], &AllGlyphs[123], &AllGlyphs[124], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(194), &AllGlyphs[124], &AllGlyphs[125], &AllGlyphs[125], &AllGlyphs[126], &AllGlyphs[124], &AllGlyphs[123], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(195), &AllGlyphs[128], &AllGlyphs[129], &AllGlyphs[128], &AllGlyphs[130], &AllGlyphs[127], &AllGlyphs[129], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(196), &AllGlyphs[129], &AllGlyphs[130], &AllGlyphs[127], &AllGlyphs[129], &AllGlyphs[130], &AllGlyphs[128], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(197), &AllGlyphs[130], &AllGlyphs[127], &AllGlyphs[130], &AllGlyphs[128], &AllGlyphs[129], &AllGlyphs[127], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(198), &AllGlyphs[127], &AllGlyphs[128], &AllGlyphs[129], &AllGlyphs[127], &AllGlyphs[128], &AllGlyphs[130], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(199), &AllGlyphs[134], &AllGlyphs[132], &AllGlyphs[132], &AllGlyphs[131], &AllGlyphs[134], &AllGlyphs[133], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(200), &AllGlyphs[133], &AllGlyphs[131], &AllGlyphs[131], &AllGlyphs[132], &AllGlyphs[133], &AllGlyphs[134], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(201), &AllGlyphs[131], &AllGlyphs[134], &AllGlyphs[133], &AllGlyphs[134], &AllGlyphs[132], &AllGlyphs[131], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(202), &AllGlyphs[132], &AllGlyphs[133], &AllGlyphs[134], &AllGlyphs[133], &AllGlyphs[131], &AllGlyphs[132], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(203), &AllGlyphs[136], &AllGlyphs[138], &AllGlyphs[137], &AllGlyphs[136], &AllGlyphs[138], &AllGlyphs[135], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(204), &AllGlyphs[138], &AllGlyphs[137], &AllGlyphs[138], &AllGlyphs[135], &AllGlyphs[136], &AllGlyphs[137], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(205), &AllGlyphs[135], &AllGlyphs[136], &AllGlyphs[135], &AllGlyphs[138], &AllGlyphs[137], &AllGlyphs[136], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(206), &AllGlyphs[137], &AllGlyphs[135], &AllGlyphs[136], &AllGlyphs[137], &AllGlyphs[135], &AllGlyphs[138], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(207), &AllGlyphs[140], &AllGlyphs[142], &AllGlyphs[141], &AllGlyphs[140], &AllGlyphs[142], &AllGlyphs[139], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(208), &AllGlyphs[142], &AllGlyphs[141], &AllGlyphs[142], &AllGlyphs[139], &AllGlyphs[140], &AllGlyphs[141], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(209), &AllGlyphs[139], &AllGlyphs[140], &AllGlyphs[139], &AllGlyphs[142], &AllGlyphs[141], &AllGlyphs[140], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(210), &AllGlyphs[141], &AllGlyphs[139], &AllGlyphs[140], &AllGlyphs[141], &AllGlyphs[139], &AllGlyphs[142], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(211), &AllGlyphs[145], &AllGlyphs[143], &AllGlyphs[144], &AllGlyphs[144], &AllGlyphs[146], &AllGlyphs[146], Connection::ORTHO_LEFT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, 0, 1, 0, 0, 0, 0 },
	{ wxUniChar(212), &AllGlyphs[146], &AllGlyphs[144], &AllGlyphs[143], &AllGlyphs[143], &AllGlyphs[145], &AllGlyphs[145], Connection::ORTHO_RIGHT, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, 0, 1, 0, 0, 0, 0 },
	{ wxUniChar(213), &AllGlyphs[143], &AllGlyphs[145], &AllGlyphs[146], &AllGlyphs[146], &AllGlyphs[144], &AllGlyphs[144], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::ORTHO_UP, 0, 1, 0, 0, 0, 0 },
	{ wxUniChar(214), &AllGlyphs[144], &AllGlyphs[146], &AllGlyphs[145], &AllGlyphs[145], &AllGlyphs[143], &AllGlyphs[143], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_DOWN, 0, 1, 0, 0, 0, 0 },
	{ wxUniChar(215), &AllGlyphs[148], &AllGlyphs[149], &AllGlyphs[149], &AllGlyphs[147], &AllGlyphs[148], &AllGlyphs[150], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(216), &AllGlyphs[149], &AllGlyphs[150], &AllGlyphs[148], &AllGlyphs[150], &AllGlyphs[147], &AllGlyphs[149], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(217), &AllGlyphs[150], &AllGlyphs[147], &AllGlyphs[147], &AllGlyphs[149], &AllGlyphs[150], &AllGlyphs[148], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(218), &AllGlyphs[147], &AllGlyphs[148], &AllGlyphs[150], &AllGlyphs[148], &AllGlyphs[149], &AllGlyphs[147], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(219), &AllGlyphs[152], &AllGlyphs[153], &AllGlyphs[153], &AllGlyphs[151], &AllGlyphs[152], &AllGlyphs[154], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(220), &AllGlyphs[153], &AllGlyphs[154], &AllGlyphs[152], &AllGlyphs[154], &AllGlyphs[151], &AllGlyphs[153], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(221), &AllGlyphs[154], &AllGlyphs[151], &AllGlyphs[151], &AllGlyphs[153], &AllGlyphs[154], &AllGlyphs[152], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(222), &AllGlyphs[151], &AllGlyphs[152], &AllGlyphs[154], &AllGlyphs[152], &AllGlyphs[153], &AllGlyphs[151], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(223), &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1, 1, 1 },
	{ wxUniChar(224), &AllGlyphs[157], &AllGlyphs[158], &AllGlyphs[159], &AllGlyphs[157], &AllGlyphs[158], &AllGlyphs[156], Connection::ORTHO_RIGHT, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(225), &AllGlyphs[158], &AllGlyphs[159], &AllGlyphs[158], &AllGlyphs[156], &AllGlyphs[157], &AllGlyphs[159], Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(226), &AllGlyphs[159], &AllGlyphs[156], &AllGlyphs[157], &AllGlyphs[159], &AllGlyphs[156], &AllGlyphs[158], Connection::ORTHO_BOTH, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_UP, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(227), &AllGlyphs[156], &AllGlyphs[157], &AllGlyphs[156], &AllGlyphs[158], &AllGlyphs[159], &AllGlyphs[157], Connection::ORTHO_BOTH, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(240), &AllGlyphs[163], &AllGlyphs[161], &AllGlyphs[161], &AllGlyphs[160], &AllGlyphs[163], &AllGlyphs[162], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(241), &AllGlyphs[162], &AllGlyphs[160], &AllGlyphs[160], &AllGlyphs[161], &AllGlyphs[162], &AllGlyphs[163], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, Connection::DIAG_FRONT, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(242), &AllGlyphs[160], &AllGlyphs[163], &AllGlyphs[162], &AllGlyphs[163], &AllGlyphs[161], &AllGlyphs[160], Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(243), &AllGlyphs[161], &AllGlyphs[162], &AllGlyphs[163], &AllGlyphs[162], &AllGlyphs[160], &AllGlyphs[161], Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(244), &AllGlyphs[169], &AllGlyphs[167], &AllGlyphs[166], &AllGlyphs[165], &AllGlyphs[171], &AllGlyphs[168], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(245), &AllGlyphs[171], &AllGlyphs[166], &AllGlyphs[167], &AllGlyphs[164], &AllGlyphs[169], &AllGlyphs[170], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(246), &AllGlyphs[168], &AllGlyphs[165], &AllGlyphs[164], &AllGlyphs[167], &AllGlyphs[170], &AllGlyphs[169], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(247), &AllGlyphs[170], &AllGlyphs[164], &AllGlyphs[165], &AllGlyphs[166], &AllGlyphs[168], &AllGlyphs[171], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(248), &AllGlyphs[165], &AllGlyphs[171], &AllGlyphs[170], &AllGlyphs[169], &AllGlyphs[167], &AllGlyphs[164], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(249), &AllGlyphs[167], &AllGlyphs[170], &AllGlyphs[171], &AllGlyphs[168], &AllGlyphs[165], &AllGlyphs[166], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(251), &AllGlyphs[164], &AllGlyphs[169], &AllGlyphs[168], &AllGlyphs[171], &AllGlyphs[166], &AllGlyphs[165], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(252), &AllGlyphs[166], &AllGlyphs[168], &AllGlyphs[169], &AllGlyphs[170], &AllGlyphs[164], &AllGlyphs[167], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(291), &AllGlyphs[173], &AllGlyphs[174], &AllGlyphs[178], &AllGlyphs[176], &AllGlyphs[177], &AllGlyphs[179], Connection::EMPTY, Connection::DIAG_FRONT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(292), &AllGlyphs[174], &AllGlyphs[175], &AllGlyphs[177], &AllGlyphs[179], &AllGlyphs[176], &AllGlyphs[178], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::DIAG_FRONT, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(293), &AllGlyphs[175], &AllGlyphs[172], &AllGlyphs[176], &AllGlyphs[178], &AllGlyphs[179], &AllGlyphs[177], Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(294), &AllGlyphs[172], &AllGlyphs[173], &AllGlyphs[179], &AllGlyphs[177], &AllGlyphs[178], &AllGlyphs[176], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::DIAG_FRONT, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(295), &AllGlyphs[177], &AllGlyphs[178], &AllGlyphs[174], &AllGlyphs[172], &AllGlyphs[173], &AllGlyphs[175], Connection::EMPTY, Connection::DIAG_BACK, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(296), &AllGlyphs[178], &AllGlyphs[179], &AllGlyphs[173], &AllGlyphs[175], &AllGlyphs[172], &AllGlyphs[174], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::DIAG_BACK, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(297), &AllGlyphs[179], &AllGlyphs[176], &AllGlyphs[172], &AllGlyphs[174], &AllGlyphs[175], &AllGlyphs[173], Connection::DIAG_BACK, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(298), &AllGlyphs[176], &AllGlyphs[177], &AllGlyphs[175], &AllGlyphs[173], &AllGlyphs[174], &AllGlyphs[172], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, 0, 0, 0, 0, 0, 0 },
	{ wxUniChar(338), &AllGlyphs[183], &AllGlyphs[180], &AllGlyphs[180], &AllGlyphs[180], &AllGlyphs[183], &AllGlyphs[183], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(352), &AllGlyphs[182], &AllGlyphs[189], &AllGlyphs[181], &AllGlyphs[189], &AllGlyphs[188], &AllGlyphs[182], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
	{ wxUniChar(710), &AllGlyphs[189], &AllGlyphs[188], &AllGlyphs[188], &AllGlyphs[182], &AllGlyphs[189], &AllGlyphs[181], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(8218), &AllGlyphs[180], &AllGlyphs[183], &AllGlyphs[183], &AllGlyphs[183], &AllGlyphs[180], &AllGlyphs[180], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
	{ wxUniChar(8222), &AllGlyphs[187], &AllGlyphs[186], &AllGlyphs[185], &AllGlyphs[187], &AllGlyphs[186], &AllGlyphs[184], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(8224), &AllGlyphs[184], &AllGlyphs[187], &AllGlyphs[184], &AllGlyphs[186], &AllGlyphs[185], &AllGlyphs[187], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(8225), &AllGlyphs[185], &AllGlyphs[184], &AllGlyphs[187], &AllGlyphs[185], &AllGlyphs[184], &AllGlyphs[186], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
	{ wxUniChar(8230), &AllGlyphs[186], &AllGlyphs[185], &AllGlyphs[186], &AllGlyphs[184], &AllGlyphs[187], &AllGlyphs[185], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
	{ wxUniChar(8240), &AllGlyphs[181], &AllGlyphs[182], &AllGlyphs[182], &AllGlyphs[188], &AllGlyphs[181], &AllGlyphs[189], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
	{ wxUniChar(8249), &AllGlyphs[188], &AllGlyphs[181], &AllGlyphs[189], &AllGlyphs[181], &AllGlyphs[182], &AllGlyphs[188], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
} };
/// The mapping from the unicode character to the Glyph that uses that character, used for reading knots.
const inline std::map<wxUniChar, const Glyph*> UnicharToGlyph = {
	{ wxUniChar(32), &AllGlyphs[0] },
	{ wxUniChar(33), &AllGlyphs[1] },
	{ wxUniChar(34), &AllGlyphs[2] },
	{ wxUniChar(35), &AllGlyphs[3] },
	{ wxUniChar(36), &AllGlyphs[4] },
	{ wxUniChar(37), &AllGlyphs[5] },
	{ wxUniChar(38), &AllGlyphs[6] },
	{ wxUniChar(39), &AllGlyphs[7] },
	{ wxUniChar(40), &AllGlyphs[8] },
	{ wxUniChar(41), &AllGlyphs[9] },
	{ wxUniChar(42), &AllGlyphs[10] },
	{ wxUniChar(43), &AllGlyphs[11] },
	{ wxUniChar(44), &AllGlyphs[12] },
	{ wxUniChar(45), &AllGlyphs[13] },
	{ wxUniChar(46), &AllGlyphs[14] },
	{ wxUniChar(47), &AllGlyphs[15] },
	{ wxUniChar(48), &AllGlyphs[16] },
	{ wxUniChar(49), &AllGlyphs[17] },
	{ wxUniChar(50), &AllGlyphs[18] },
	{ wxUniChar(51), &AllGlyphs[19] },
	{ wxUniChar(52), &AllGlyphs[20] },
	{ wxUniChar(53), &AllGlyphs[21] },
	{ wxUniChar(54), &AllGlyphs[22] },
	{ wxUniChar(55), &AllGlyphs[23] },
	{ wxUniChar(56), &AllGlyphs[24] },
	{ wxUniChar(57), &AllGlyphs[25] },
	{ wxUniChar(58), &AllGlyphs[26] },
	{ wxUniChar(59), &AllGlyphs[27] },
	{ wxUniChar(60), &AllGlyphs[28] },
	{ wxUniChar(61), &AllGlyphs[29] },
	{ wxUniChar(62), &AllGlyphs[30] },
	{ wxUniChar(63), &AllGlyphs[31] },
	{ wxUniChar(64), &AllGlyphs[32] },
	{ wxUniChar(65), &AllGlyphs[33] },
	{ wxUniChar(66), &AllGlyphs[34] },
	{ wxUniChar(67), &AllGlyphs[35] },
	{ wxUniChar(68), &AllGlyphs[36] },
	{ wxUniChar(69), &AllGlyphs[37] },
	{ wxUniChar(70), &AllGlyphs[38] },
	{ wxUniChar(71), &AllGlyphs[39] },
	{ wxUniChar(72), &AllGlyphs[40] },
	{ wxUniChar(73), &AllGlyphs[41] },
	{ wxUniChar(74), &AllGlyphs[42] },
	{ wxUniChar(75), &AllGlyphs[43] },
	{ wxUniChar(76), &AllGlyphs[44] },
	{ wxUniChar(77), &AllGlyphs[45] },
	{ wxUniChar(78), &AllGlyphs[46] },
	{ wxUniChar(79), &AllGlyphs[47] },
	{ wxUniChar(80), &AllGlyphs[48] },
	{ wxUniChar(81), &AllGlyphs[49] },
	{ wxUniChar(82), &AllGlyphs[50] },
	{ wxUniChar(83), &AllGlyphs[51] },
	{ wxUniChar(84), &AllGlyphs[52] },
	{ wxUniChar(85), &AllGlyphs[53] },
	{ wxUniChar(86), &AllGlyphs[54] },
	{ wxUniChar(87), &AllGlyphs[55] },
	{ wxUniChar(88), &AllGlyphs[56] },
	{ wxUniChar(89), &AllGlyphs[57] },
	{ wxUniChar(90), &AllGlyphs[58] },
	{ wxUniChar(91), &AllGlyphs[59] },
	{ wxUniChar(92), &AllGlyphs[60] },
	{ wxUniChar(93), &AllGlyphs[61] },
	{ wxUniChar(94), &AllGlyphs[62] },
	{ wxUniChar(95), &AllGlyphs[63] },
	{ wxUniChar(96), &AllGlyphs[64] },
	{ wxUniChar(97), &AllGlyphs[65] },
	{ wxUniChar(98), &AllGlyphs[66] },
	{ wxUniChar(99), &AllGlyphs[67] },
	{ wxUniChar(100), &AllGlyphs[68] },
	{ wxUniChar(101), &AllGlyphs[69] },
	{ wxUniChar(102), &AllGlyphs[70] },
	{ wxUniChar(103), &AllGlyphs[71] },
	{ wxUniChar(104), &AllGlyphs[72] },
	{ wxUniChar(105), &AllGlyphs[73] },
	{ wxUniChar(106), &AllGlyphs[74] },
	{ wxUniChar(107), &AllGlyphs[75] },
	{ wxUniChar(108), &AllGlyphs[76] },
	{ wxUniChar(109), &AllGlyphs[77] },
	{ wxUniChar(110), &AllGlyphs[78] },
	{ wxUniChar(111), &AllGlyphs[79] },
	{ wxUniChar(112), &AllGlyphs[80] },
	{ wxUniChar(113), &AllGlyphs[81] },
	{ wxUniChar(114), &AllGlyphs[82] },
	{ wxUniChar(115), &AllGlyphs[83] },
	{ wxUniChar(116), &AllGlyphs[84] },
	{ wxUniChar(117), &AllGlyphs[85] },
	{ wxUniChar(118), &AllGlyphs[86] },
	{ wxUniChar(119), &AllGlyphs[87] },
	{ wxUniChar(120), &AllGlyphs[88] },
	{ wxUniChar(121), &AllGlyphs[89] },
	{ wxUniChar(122), &AllGlyphs[90] },
	{ wxUniChar(123), &AllGlyphs[91] },
	{ wxUniChar(124), &AllGlyphs[92] },
	{ wxUniChar(125), &AllGlyphs[93] },
	{ wxUniChar(126), &AllGlyphs[94] },
	{ wxUniChar(161), &AllGlyphs[95] },
	{ wxUniChar(162), &AllGlyphs[96] },
	{ wxUniChar(163), &AllGlyphs[97] },
	{ wxUniChar(164), &AllGlyphs[98] },
	{ wxUniChar(165), &AllGlyphs[99] },
	{ wxUniChar(166), &AllGlyphs[100] },
	{ wxUniChar(167), &AllGlyphs[101] },
	{ wxUniChar(168), &AllGlyphs[102] },
	{ wxUniChar(169), &AllGlyphs[103] },
	{ wxUniChar(170), &AllGlyphs[104] },
	{ wxUniChar(171), &AllGlyphs[105] },
	{ wxUniChar(172), &AllGlyphs[106] },
	{ wxUniChar(174), &AllGlyphs[107] },
	{ wxUniChar(175), &AllGlyphs[108] },
	{ wxUniChar(176), &AllGlyphs[109] },
	{ wxUniChar(177), &AllGlyphs[110] },
	{ wxUniChar(178), &AllGlyphs[111] },
	{ wxUniChar(179), &AllGlyphs[112] },
	{ wxUniChar(180), &AllGlyphs[113] },
	{ wxUniChar(181), &AllGlyphs[114] },
	{ wxUniChar(182), &AllGlyphs[115] },
	{ wxUniChar(184), &AllGlyphs[116] },
	{ wxUniChar(185), &AllGlyphs[117] },
	{ wxUniChar(186), &AllGlyphs[118] },
	{ wxUniChar(187), &AllGlyphs[119] },
	{ wxUniChar(188), &AllGlyphs[120] },
	{ wxUniChar(189), &AllGlyphs[121] },
	{ wxUniChar(190), &AllGlyphs[122] },
	{ wxUniChar(191), &AllGlyphs[123] },
	{ wxUniChar(192), &AllGlyphs[124] },
	{ wxUniChar(193), &AllGlyphs[125] },
	{ wxUniChar(194), &AllGlyphs[126] },
	{ wxUniChar(195), &AllGlyphs[127] },
	{ wxUniChar(196), &AllGlyphs[128] },
	{ wxUniChar(197), &AllGlyphs[129] },
	{ wxUniChar(198), &AllGlyphs[130] },
	{ wxUniChar(199), &AllGlyphs[131] },
	{ wxUniChar(200), &AllGlyphs[132] },
	{ wxUniChar(201), &AllGlyphs[133] },
	{ wxUniChar(202), &AllGlyphs[134] },
	{ wxUniChar(203), &AllGlyphs[135] },
	{ wxUniChar(204), &AllGlyphs[136] },
	{ wxUniChar(205), &AllGlyphs[137] },
	{ wxUniChar(206), &AllGlyphs[138] },
	{ wxUniChar(207), &AllGlyphs[139] },
	{ wxUniChar(208), &AllGlyphs[140] },
	{ wxUniChar(209), &AllGlyphs[141] },
	{ wxUniChar(210), &AllGlyphs[142] },
	{ wxUniChar(211), &AllGlyphs[143] },
	{ wxUniChar(212), &AllGlyphs[144] },
	{ wxUniChar(213), &AllGlyphs[145] },
	{ wxUniChar(214), &AllGlyphs[146] },
	{ wxUniChar(215), &AllGlyphs[147] },
	{ wxUniChar(216), &AllGlyphs[148] },
	{ wxUniChar(217), &AllGlyphs[149] },
	{ wxUniChar(218), &AllGlyphs[150] },
	{ wxUniChar(219), &AllGlyphs[151] },
	{ wxUniChar(220), &AllGlyphs[152] },
	{ wxUniChar(221), &AllGlyphs[153] },
	{ wxUniChar(222), &AllGlyphs[154] },
	{ wxUniChar(223), &AllGlyphs[155] },
	{ wxUniChar(224), &AllGlyphs[156] },
	{ wxUniChar(225), &AllGlyphs[157] },
	{ wxUniChar(226), &AllGlyphs[158] },
	{ wxUniChar(227), &AllGlyphs[159] },
	{ wxUniChar(240), &AllGlyphs[160] },
	{ wxUniChar(241), &AllGlyphs[161] },
	{ wxUniChar(242), &AllGlyphs[162] },
	{ wxUniChar(243), &AllGlyphs[163] },
	{ wxUniChar(244), &AllGlyphs[164] },
	{ wxUniChar(245), &AllGlyphs[165] },
	{ wxUniChar(246), &AllGlyphs[166] },
	{ wxUniChar(247), &AllGlyphs[167] },
	{ wxUniChar(248), &AllGlyphs[168] },
	{ wxUniChar(249), &AllGlyphs[169] },
	{ wxUniChar(251), &AllGlyphs[170] },
	{ wxUniChar(252), &AllGlyphs[171] },
	{ wxUniChar(291), &AllGlyphs[172] },
	{ wxUniChar(292), &AllGlyphs[173] },
	{ wxUniChar(293), &AllGlyphs[174] },
	{ wxUniChar(294), &AllGlyphs[175] },
	{ wxUniChar(295), &AllGlyphs[176] },
	{ wxUniChar(296), &AllGlyphs[177] },
	{ wxUniChar(297), &AllGlyphs[178] },
	{ wxUniChar(298), &AllGlyphs[179] },
	{ wxUniChar(338), &AllGlyphs[180] },
	{ wxUniChar(352), &AllGlyphs[181] },
	{ wxUniChar(710), &AllGlyphs[182] },
	{ wxUniChar(8218), &AllGlyphs[183] },
	{ wxUniChar(8222), &AllGlyphs[184] },
	{ wxUniChar(8224), &AllGlyphs[185] },
	{ wxUniChar(8225), &AllGlyphs[186] },
	{ wxUniChar(8230), &AllGlyphs[187] },
	{ wxUniChar(8240), &AllGlyphs[188] },
	{ wxUniChar(8249), &AllGlyphs[189] }
};
/// The default Glyph to fill the Knot upon initialization, which is set as the \c space character, \c \x20
const Glyph* const DefaultGlyph = &AllGlyphs[0];

inline bool compatible(Connections known, Connections checking)
{
	return (known.up    == Connection::DO_NOT_CARE || known.up    == checking.up)
	    && (known.down  == Connection::DO_NOT_CARE || known.down  == checking.down)
	    && (known.left  == Connection::DO_NOT_CARE || known.left  == checking.left)
	    && (known.right == Connection::DO_NOT_CARE || known.right == checking.right);
}

inline const Glyph* RandomGlyph(const Connection up, const Connection down, const Connection left, const Connection right, const GlyphFlag boolFlags)
/// This function takes in the desired flags and outputs the vector of all glyphs which meet the criteria.
{
	/// \param up The \c Connection desired for the upper side. If this does not matter, then pass \c Connection::DO_NOT_CARE
	/// \param down The \c Connection desired for the lower side. If this does not matter, then pass \c Connection::DO_NOT_CARE
	/// \param left The \c Connection desired for the left side. If this does not matter, then pass \c Connection::DO_NOT_CARE
	/// \param right The \c Connection desired for the right side. If this does not matter, then pass \c Connection::DO_NOT_CARE
	/// \param boolFlag The other condition flags to check for the glyphs, passed by using \c operator| on \c GlyphFlag values. Any bits with a value of \c 0 are ignored

	/// \b Method

	/// This function allows all properties of the Glyphs to be checked at once, instead of checking multiple properties in order.
	/// Using this function allows for high speed Glyph selection, since all attributes are assessed simultaneously.

	/// First a bit mask is created. If the \c up Connection is anything other than \c Connection::DO_NOT_CARE, then use \c GlyphFlag::UP in the mask.
	/// Do this same process for the other 3 Connection values. As well, add the \c boolFlags to the mask with the \c GlyphFlag::COND_MASK applied.
	/// It is assumed that if a flag is nonzero, it is in use. If it is zero, it is ignored.
	const GlyphFlag bool_mask = (boolFlags & GlyphFlag::COND_MASK); // All the other flags are only 1 bit long, so this is acceptable

	/// Next, determine which value needs to be checked against. For the 4 sides, use the toFlag() function.
	/// Add the boolFlags, with the \c GlyphFlag::COND_MASK applied.
	const GlyphFlag bool_toCheck = (boolFlags & GlyphFlag::COND_MASK);

	/// Once all that is done, loop through \c AllGlyphs. If the \c flags member of the Glyph with the mask applied from above
	/// is equal to the value which needs to be checked, then add the pointer to this Glyph to the output vector.
	GlyphVec1 glyphList;
	for (const Glyph& glyph : AllGlyphs)
		if (compatible({up, down, left, right}, {glyph.up, glyph.down, glyph.left, glyph.right}) && (glyph.flags & bool_mask) == bool_toCheck)
			glyphList.push_back(&glyph);

	if (glyphList.size() == 0)
		return nullptr;
	else
		return glyphList[rand() % glyphList.size()];
}
