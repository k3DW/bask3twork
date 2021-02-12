#pragma once
#include "Constants.h"
/// \file

/// The side of a glyph, as a bitfield enum, which can undergo the \c operator| and \c operator&
enum class Side : unsigned int {
	NONE	= 0, ///< No side
	UP		= 1, ///< Upper side
	DOWN	= 2, ///< Lower side
	LEFT	= 4, ///< Left side
	RIGHT	= 8	 ///< Right side
};
constexpr inline Side operator|(Side side1, Side side2)
/// Allows Side values to have \c operator| used on them, to generate new Side values
{ 
	return static_cast<Side>(static_cast<unsigned int>(side1) | static_cast<unsigned int>(side2));
}
constexpr inline Side operator&(Side side1, Side side2)
/// Allows Side values to have \c operator& used on them, to generate new Side values
{
	return static_cast<Side>(static_cast<unsigned int>(side1) & static_cast<unsigned int>(side2));
} 

/// The connection on one side of a glyph, as an incremental enum, which can undergo the \c operator| and \c operator&
enum class Connection : unsigned int {
		DO_NOT_CARE = 0, ///< The connection is irrelevant (no Glyph objects should ever be assigned this value, it is for usage in functions)
		EMPTY		= 1, ///< The empty connection, where nothing is passing through this edge
		DIAG_BOTH	= 2, ///< There are 2 strands, which pass over each other diagonally, at the the midpoint of the edge
		ORTHO_BOTH	= 3, ///< There are 2 strands, which are parallel to each other and are orthogonal to the edge
		DIAG_FRONT	= 4, ///< Only the strand from \c DIAG_BOTH that is visually "on top" or "in front"
		DIAG_BACK	= 5, ///< Only the strand from \c DIAG_BOTH that is visually "on the bottom" or "behind"
		ORTHO_UP	= 6, ///< Only the strand from \c ORTHO_BOTH that is running horizontally, on the upper half
		ORTHO_DOWN	= 7, ///< Only the strand from \c ORTHO_BOTH that is running horizontally, on the lower half
		ORTHO_LEFT	= 8, ///< Only the strand from \c ORTHO_BOTH that is running vertical, on the left half
		ORTHO_RIGHT = 9  ///< Only the strand from \c ORTHO_BOTH that is running vertical, on the right half
};
constexpr inline Connection rot4Connection(Connection input)
/// This function takes a Connection to its counterpart after a clockwise rotation of 90 degrees
{
	return std::array<Connection, 10>{ Connection::DO_NOT_CARE, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::ORTHO_RIGHT, Connection::ORTHO_LEFT, Connection::ORTHO_UP, Connection::ORTHO_DOWN } [static_cast<unsigned int>(input)];
}
constexpr inline Connection rot2Connection(Connection input)
/// This function takes a Connection to its counterpart after a rotation of 180 degrees
{
	return std::array<Connection, 10>{ Connection::DO_NOT_CARE, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::ORTHO_DOWN, Connection::ORTHO_UP, Connection::ORTHO_RIGHT, Connection::ORTHO_LEFT } [static_cast<unsigned int>(input)];
}
constexpr inline Connection mirXConnection(Connection input)
/// This function takes a Connection to its counterpart after being mirrored across the horizontal line
{
	return std::array<Connection, 10>{ Connection::DO_NOT_CARE, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::ORTHO_DOWN, Connection::ORTHO_UP, Connection::ORTHO_LEFT, Connection::ORTHO_RIGHT } [static_cast<unsigned int>(input)];
}
constexpr inline Connection mirYConnection(Connection input)
/// This function takes a Connection to its counterpart after being mirrored across the vertical line
{
	return std::array<Connection, 10>{ Connection::DO_NOT_CARE, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::ORTHO_UP, Connection::ORTHO_DOWN, Connection::ORTHO_RIGHT, Connection::ORTHO_LEFT } [static_cast<unsigned int>(input)];
}
constexpr inline Connection mirFDConnection(Connection input)
/// This function takes a Connection to its counterpart after being mirrored across the forward diagonal line
{
	return std::array<Connection, 10>{ Connection::DO_NOT_CARE, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::ORTHO_RIGHT, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::ORTHO_UP } [static_cast<unsigned int>(input)];
}
constexpr inline Connection mirBDConnection(Connection input)
/// This function takes a Connection to its counterpart after being mirrored across the backward diagonal line
{
	return std::array<Connection, 10>{ Connection::DO_NOT_CARE, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::ORTHO_LEFT, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_DOWN } [static_cast<unsigned int>(input)];
}

/// The bit flag for each of the properties of a Glyph object
enum class GlyphFlag : ull {
	NONE	= 0,			///< No flag
	UP		= 0b1111 <<  0,	///< The upper side 4-bit mask
	DOWN	= 0b1111 <<  4, ///< The lower side 4-bit mask
	LEFT	= 0b1111 <<  8, ///< The left side 4-bit mask
	RIGHT	= 0b1111 << 12, ///< The right side 4-bit mask
	SA_ROT4	 = 1 << 16,		///< Is this Glyph the same after rotating by 90 degrees
	SA_ROT2  = 1 << 17,		///< Is this Glyph the same after rotating by 180 degrees
	SA_MIRX  = 1 << 18,		///< Is this Glyph the same after mirroring across the horizontal line
	SA_MIRY  = 1 << 19,		///< Is this Glyph the same after mirroring across the vertical line
	CT_ROT4U = 1 << 20,		///< Can this Glyph connect to its 90-degree-rotated counterpart on its own upper side
	CT_ROT4D = 1 << 21,		///< Can this Glyph connect to its 90-degree-rotated counterpart on its own lower side
	CT_ROT4L = 1 << 22,		///< Can this Glyph connect to its 90-degree-rotated counterpart on its own left side
	CT_ROT4R = 1 << 23,		///< Can this Glyph connect to its 90-degree-rotated counterpart on its own right side
	CT_ROT2U = 1 << 24,		///< Can this Glyph connect to its 180-degree-rotated counterpart on its own upper side
	CT_ROT2D = 1 << 25,		///< Can this Glyph connect to its 180-degree-rotated counterpart on its own lower side
	CT_ROT2L = 1 << 26,		///< Can this Glyph connect to its 180-degree-rotated counterpart on its own left side
	CT_ROT2R = 1 << 27,		///< Can this Glyph connect to its 180-degree-rotated counterpart on its own right side
	CT_MIRU  = 1 << 28,		///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its upper side
	CT_MIRD  = 1 << 29,		///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its lower side
	CT_MIRL  = 1 << 30,		///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its left side
	CT_MIRR  = 1LL << 31,	///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its right side
	SA_MIRFD = 1LL << 32,	///< Is this Glyph the same after mirroring across the forward diagonal
	SA_MIRBD = 1LL << 33,	///< Is this Glyph the same after mirroring across the backward diagonal

	COND_MASK = 0b111111111111111111LL << 16, ///< The mask of all the non-Side flags
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

	union {
		struct {
			Connection up : 4;						///< The connection on the upper side of the Glyph
			Connection down : 4;					///< The connection on the lower side of the Glyph
			Connection left : 4;					///< The connection on the left side of the Glyph
			Connection right : 4;					///< The connection on the right side of the Glyph
			unsigned int sameAfterRotate4 : 1;		///< Is this Glyph the same after rotating by 90 degrees
			unsigned int sameAfterRotate2 : 1;		///< Is this Glyph the same after rotating by 180 degrees
			unsigned int sameAfterMirrorX : 1;		///< Is this Glyph the same after mirroring across the horizontal line
			unsigned int sameAfterMirrorY : 1;		///< Is this Glyph the same after mirroring across the vertical line
			unsigned int connectToRotate4Up : 1;	///< Can this Glyph connect to its 90-degree-rotated counterpart on its own upper side
			unsigned int connectToRotate4Down : 1;	///< Can this Glyph connect to its 90-degree-rotated counterpart on its own lower side
			unsigned int connectToRotate4Left : 1;	///< Can this Glyph connect to its 90-degree-rotated counterpart on its own left side
			unsigned int connectToRotate4Right : 1;	///< Can this Glyph connect to its 90-degree-rotated counterpart on its own right side
			unsigned int connectToRotate2Up : 1;	///< Can this Glyph connect to its 180-degree-rotated counterpart on its own upper side
			unsigned int connectToRotate2Down : 1;	///< Can this Glyph connect to its 180-degree-rotated counterpart on its own lower side
			unsigned int connectToRotate2Left : 1;	///< Can this Glyph connect to its 180-degree-rotated counterpart on its own left side
			unsigned int connectToRotate2Right : 1;	///< Can this Glyph connect to its 180-degree-rotated counterpart on its own right side
			unsigned int connectToMirrorUp : 1;		///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its upper side
			unsigned int connectToMirrorDown : 1;	///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its lower side
			unsigned int connectToMirrorLeft : 1;	///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its left side
			unsigned int connectToMirrorRight : 1;	///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its right side
			unsigned int sameAfterMirrorFwdDiag : 1;	///< Is this Glyph the same after mirroring across the horizontal line
			unsigned int sameAfterMirrorBackDiag : 1;	///< Is this Glyph the same after mirroring across the vertical line
		};
		GlyphFlag flags;	///< The total signature of this glyph, in a union with all the other flags to access the individual flags simultaneously
	};

	/// All the \c connectTo____ flags are determined from the other parameters, but the other parameters are given explicitly
	Glyph(wxString chr, const Glyph* rotated4, const Glyph* rotated2, const Glyph* mirroredX, const Glyph* mirroredY, const Glyph* mirroredFD, const Glyph* mirroredBD,
	const Connection up, const Connection down, const Connection left, const Connection right,
	unsigned int sameAfterRotate4, unsigned int sameAfterRotate2, unsigned int sameAfterMirrorX, unsigned int sameAfterMirrorY, unsigned int sameAfterMirrorFwdDiag, unsigned int sameAfterMirrorBackDiag) :
		chr{ chr }, rotated4{ rotated4 }, rotated2{ rotated2 }, mirroredX{ mirroredX }, mirroredY{ mirroredY }, mirroredFD{ mirroredFD }, mirroredBD{ mirroredBD },
		up{ up }, down{ down }, left{ left }, right{ right },
		sameAfterRotate4{ sameAfterRotate4 }, sameAfterRotate2{ sameAfterRotate2 },
		sameAfterMirrorX{ sameAfterMirrorX }, sameAfterMirrorY{ sameAfterMirrorY },
		connectToRotate4Up		{up		== rot4Connection(right)},
		connectToRotate4Down	{down	== rot4Connection(left)},
		connectToRotate4Left	{left	== rot4Connection(up)},
		connectToRotate4Right	{right	== rot4Connection(down)},
		connectToRotate2Up		{up		== rot2Connection(up)},
		connectToRotate2Down	{down	== rot2Connection(down)},
		connectToRotate2Left	{left	== rot2Connection(left)},
		connectToRotate2Right	{right	== rot2Connection(right)},
		connectToMirrorUp		{up		== mirXConnection(up)},
		connectToMirrorDown		{down	== mirXConnection(down)},
		connectToMirrorLeft		{left	== mirYConnection(left)},
		connectToMirrorRight	{right	== mirYConnection(right)},
		sameAfterMirrorFwdDiag{ sameAfterMirrorFwdDiag }, sameAfterMirrorBackDiag{ sameAfterMirrorBackDiag } {}

	static constexpr int TOTAL = 190; ///< The total number of glyphs used
};
using GlyphVec1 = std::vector<const Glyph*>;	///< Syntactic sugar for a 1D vector of pointers to \c const \c Glyph
using GlyphVec2 = std::vector<GlyphVec1>;		///< Syntactic sugar for a 2D vector of pointers to \c const \c Glyph

/// The array of every Glyph in the program (generated in Excel), the only place where a Glyph object is initialized;
/// every other place a Glyph is referenced in the whole codebase is actually a pointer to one of the Glyphs in AllGlyphs.
const inline std::array<Glyph, Glyph::TOTAL> AllGlyphs{ {
    { wxString::FromUTF8("\x20"), &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 1, 1, 1, 1, 1, 1 },
    { wxString::FromUTF8("\x21"), &AllGlyphs[94], &AllGlyphs[1], &AllGlyphs[1], &AllGlyphs[1], &AllGlyphs[94], &AllGlyphs[94], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x22"), &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1, 1, 1 },
    { wxString::FromUTF8("\x23"), &AllGlyphs[32], &AllGlyphs[3], &AllGlyphs[3], &AllGlyphs[3], &AllGlyphs[32], &AllGlyphs[32], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x24"), &AllGlyphs[54], &AllGlyphs[50], &AllGlyphs[50], &AllGlyphs[4], &AllGlyphs[54], &AllGlyphs[38], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x25"), &AllGlyphs[40], &AllGlyphs[52], &AllGlyphs[52], &AllGlyphs[5], &AllGlyphs[40], &AllGlyphs[39], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x26"), &AllGlyphs[43], &AllGlyphs[41], &AllGlyphs[53], &AllGlyphs[10], &AllGlyphs[28], &AllGlyphs[42], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x27"), &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 1, 1, 1, 1, 1, 1 },
    { wxString::FromUTF8("\x28"), &AllGlyphs[9], &AllGlyphs[48], &AllGlyphs[47], &AllGlyphs[9], &AllGlyphs[48], &AllGlyphs[8], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x29"), &AllGlyphs[48], &AllGlyphs[47], &AllGlyphs[48], &AllGlyphs[8], &AllGlyphs[9], &AllGlyphs[47], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x2A"), &AllGlyphs[28], &AllGlyphs[53], &AllGlyphs[41], &AllGlyphs[6], &AllGlyphs[43], &AllGlyphs[45], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x2B"), &AllGlyphs[93], &AllGlyphs[91], &AllGlyphs[93], &AllGlyphs[63], &AllGlyphs[11], &AllGlyphs[91], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x2C"), &AllGlyphs[85], &AllGlyphs[74], &AllGlyphs[75], &AllGlyphs[77], &AllGlyphs[23], &AllGlyphs[73], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x2D"), &AllGlyphs[61], &AllGlyphs[29], &AllGlyphs[29], &AllGlyphs[13], &AllGlyphs[61], &AllGlyphs[59], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x2E"), &AllGlyphs[25], &AllGlyphs[27], &AllGlyphs[76], &AllGlyphs[15], &AllGlyphs[79], &AllGlyphs[16], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x2F"), &AllGlyphs[79], &AllGlyphs[76], &AllGlyphs[27], &AllGlyphs[14], &AllGlyphs[25], &AllGlyphs[80], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x30"), &AllGlyphs[15], &AllGlyphs[79], &AllGlyphs[80], &AllGlyphs[25], &AllGlyphs[27], &AllGlyphs[14], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x31"), &AllGlyphs[64], &AllGlyphs[17], &AllGlyphs[17], &AllGlyphs[17], &AllGlyphs[64], &AllGlyphs[64], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x32"), &AllGlyphs[19], &AllGlyphs[18], &AllGlyphs[18], &AllGlyphs[18], &AllGlyphs[19], &AllGlyphs[19], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x33"), &AllGlyphs[18], &AllGlyphs[19], &AllGlyphs[19], &AllGlyphs[19], &AllGlyphs[18], &AllGlyphs[18], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x34"), &AllGlyphs[86], &AllGlyphs[82], &AllGlyphs[82], &AllGlyphs[20], &AllGlyphs[86], &AllGlyphs[70], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x35"), &AllGlyphs[72], &AllGlyphs[84], &AllGlyphs[84], &AllGlyphs[21], &AllGlyphs[72], &AllGlyphs[71], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x36"), &AllGlyphs[78], &AllGlyphs[89], &AllGlyphs[89], &AllGlyphs[22], &AllGlyphs[78], &AllGlyphs[66], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x37"), &AllGlyphs[75], &AllGlyphs[73], &AllGlyphs[85], &AllGlyphs[24], &AllGlyphs[12], &AllGlyphs[74], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x38"), &AllGlyphs[12], &AllGlyphs[85], &AllGlyphs[73], &AllGlyphs[23], &AllGlyphs[75], &AllGlyphs[77], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x39"), &AllGlyphs[27], &AllGlyphs[80], &AllGlyphs[79], &AllGlyphs[16], &AllGlyphs[15], &AllGlyphs[76], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x3A"), &AllGlyphs[31], &AllGlyphs[30], &AllGlyphs[31], &AllGlyphs[44], &AllGlyphs[26], &AllGlyphs[30], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x3B"), &AllGlyphs[80], &AllGlyphs[14], &AllGlyphs[15], &AllGlyphs[76], &AllGlyphs[16], &AllGlyphs[79], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x3C"), &AllGlyphs[53], &AllGlyphs[42], &AllGlyphs[43], &AllGlyphs[45], &AllGlyphs[6], &AllGlyphs[41], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x3D"), &AllGlyphs[59], &AllGlyphs[13], &AllGlyphs[13], &AllGlyphs[29], &AllGlyphs[59], &AllGlyphs[61], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x3E"), &AllGlyphs[44], &AllGlyphs[26], &AllGlyphs[44], &AllGlyphs[31], &AllGlyphs[30], &AllGlyphs[26], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x3F"), &AllGlyphs[30], &AllGlyphs[44], &AllGlyphs[26], &AllGlyphs[30], &AllGlyphs[44], &AllGlyphs[31], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x40"), &AllGlyphs[3], &AllGlyphs[32], &AllGlyphs[32], &AllGlyphs[32], &AllGlyphs[3], &AllGlyphs[3], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x41"), &AllGlyphs[55], &AllGlyphs[36], &AllGlyphs[33], &AllGlyphs[36], &AllGlyphs[56], &AllGlyphs[55], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x42"), &AllGlyphs[62], &AllGlyphs[46], &AllGlyphs[34], &AllGlyphs[46], &AllGlyphs[57], &AllGlyphs[62], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x43"), &AllGlyphs[58], &AllGlyphs[49], &AllGlyphs[37], &AllGlyphs[58], &AllGlyphs[49], &AllGlyphs[35], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x44"), &AllGlyphs[56], &AllGlyphs[33], &AllGlyphs[36], &AllGlyphs[33], &AllGlyphs[55], &AllGlyphs[56], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x45"), &AllGlyphs[35], &AllGlyphs[58], &AllGlyphs[35], &AllGlyphs[49], &AllGlyphs[37], &AllGlyphs[58], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x46"), &AllGlyphs[4], &AllGlyphs[54], &AllGlyphs[38], &AllGlyphs[54], &AllGlyphs[50], &AllGlyphs[4], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x47"), &AllGlyphs[5], &AllGlyphs[40], &AllGlyphs[39], &AllGlyphs[40], &AllGlyphs[52], &AllGlyphs[5], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x48"), &AllGlyphs[52], &AllGlyphs[39], &AllGlyphs[40], &AllGlyphs[39], &AllGlyphs[5], &AllGlyphs[52], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x49"), &AllGlyphs[45], &AllGlyphs[6], &AllGlyphs[10], &AllGlyphs[53], &AllGlyphs[42], &AllGlyphs[28], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x4A"), &AllGlyphs[10], &AllGlyphs[28], &AllGlyphs[45], &AllGlyphs[43], &AllGlyphs[41], &AllGlyphs[6], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x4B"), &AllGlyphs[41], &AllGlyphs[45], &AllGlyphs[28], &AllGlyphs[42], &AllGlyphs[10], &AllGlyphs[53], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x4C"), &AllGlyphs[26], &AllGlyphs[31], &AllGlyphs[30], &AllGlyphs[26], &AllGlyphs[31], &AllGlyphs[44], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x4D"), &AllGlyphs[6], &AllGlyphs[43], &AllGlyphs[42], &AllGlyphs[28], &AllGlyphs[53], &AllGlyphs[10], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x4E"), &AllGlyphs[57], &AllGlyphs[34], &AllGlyphs[46], &AllGlyphs[34], &AllGlyphs[62], &AllGlyphs[57], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x4F"), &AllGlyphs[8], &AllGlyphs[9], &AllGlyphs[8], &AllGlyphs[48], &AllGlyphs[47], &AllGlyphs[9], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x50"), &AllGlyphs[47], &AllGlyphs[8], &AllGlyphs[9], &AllGlyphs[47], &AllGlyphs[8], &AllGlyphs[48], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x51"), &AllGlyphs[37], &AllGlyphs[35], &AllGlyphs[58], &AllGlyphs[37], &AllGlyphs[35], &AllGlyphs[49], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x52"), &AllGlyphs[38], &AllGlyphs[4], &AllGlyphs[4], &AllGlyphs[50], &AllGlyphs[38], &AllGlyphs[54], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x53"), &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1, 1, 1 },
    { wxString::FromUTF8("\x54"), &AllGlyphs[39], &AllGlyphs[5], &AllGlyphs[5], &AllGlyphs[52], &AllGlyphs[39], &AllGlyphs[40], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x55"), &AllGlyphs[42], &AllGlyphs[10], &AllGlyphs[6], &AllGlyphs[41], &AllGlyphs[45], &AllGlyphs[43], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x56"), &AllGlyphs[50], &AllGlyphs[38], &AllGlyphs[54], &AllGlyphs[38], &AllGlyphs[4], &AllGlyphs[50], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x57"), &AllGlyphs[36], &AllGlyphs[56], &AllGlyphs[56], &AllGlyphs[55], &AllGlyphs[36], &AllGlyphs[33], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x58"), &AllGlyphs[33], &AllGlyphs[55], &AllGlyphs[55], &AllGlyphs[56], &AllGlyphs[33], &AllGlyphs[36], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x59"), &AllGlyphs[34], &AllGlyphs[62], &AllGlyphs[62], &AllGlyphs[57], &AllGlyphs[34], &AllGlyphs[46], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x5A"), &AllGlyphs[49], &AllGlyphs[37], &AllGlyphs[49], &AllGlyphs[35], &AllGlyphs[58], &AllGlyphs[37], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x5B"), &AllGlyphs[13], &AllGlyphs[61], &AllGlyphs[59], &AllGlyphs[61], &AllGlyphs[29], &AllGlyphs[13], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x5C"), &AllGlyphs[92], &AllGlyphs[60], &AllGlyphs[60], &AllGlyphs[60], &AllGlyphs[92], &AllGlyphs[92], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x5D"), &AllGlyphs[29], &AllGlyphs[59], &AllGlyphs[61], &AllGlyphs[59], &AllGlyphs[13], &AllGlyphs[29], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x5E"), &AllGlyphs[46], &AllGlyphs[57], &AllGlyphs[57], &AllGlyphs[62], &AllGlyphs[46], &AllGlyphs[34], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x5F"), &AllGlyphs[11], &AllGlyphs[93], &AllGlyphs[91], &AllGlyphs[11], &AllGlyphs[93], &AllGlyphs[63], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x60"), &AllGlyphs[17], &AllGlyphs[64], &AllGlyphs[64], &AllGlyphs[64], &AllGlyphs[17], &AllGlyphs[17], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x61"), &AllGlyphs[87], &AllGlyphs[68], &AllGlyphs[65], &AllGlyphs[68], &AllGlyphs[88], &AllGlyphs[87], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x62"), &AllGlyphs[22], &AllGlyphs[78], &AllGlyphs[66], &AllGlyphs[78], &AllGlyphs[89], &AllGlyphs[22], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x63"), &AllGlyphs[90], &AllGlyphs[81], &AllGlyphs[69], &AllGlyphs[90], &AllGlyphs[81], &AllGlyphs[67], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x64"), &AllGlyphs[88], &AllGlyphs[65], &AllGlyphs[68], &AllGlyphs[65], &AllGlyphs[87], &AllGlyphs[88], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x65"), &AllGlyphs[67], &AllGlyphs[90], &AllGlyphs[67], &AllGlyphs[81], &AllGlyphs[69], &AllGlyphs[90], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x66"), &AllGlyphs[20], &AllGlyphs[86], &AllGlyphs[70], &AllGlyphs[86], &AllGlyphs[82], &AllGlyphs[20], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x67"), &AllGlyphs[21], &AllGlyphs[72], &AllGlyphs[71], &AllGlyphs[72], &AllGlyphs[84], &AllGlyphs[21], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x68"), &AllGlyphs[84], &AllGlyphs[71], &AllGlyphs[72], &AllGlyphs[71], &AllGlyphs[21], &AllGlyphs[84], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x69"), &AllGlyphs[77], &AllGlyphs[23], &AllGlyphs[24], &AllGlyphs[85], &AllGlyphs[74], &AllGlyphs[12], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x6A"), &AllGlyphs[24], &AllGlyphs[12], &AllGlyphs[77], &AllGlyphs[75], &AllGlyphs[73], &AllGlyphs[23], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x6B"), &AllGlyphs[73], &AllGlyphs[77], &AllGlyphs[12], &AllGlyphs[74], &AllGlyphs[24], &AllGlyphs[85], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x6C"), &AllGlyphs[16], &AllGlyphs[15], &AllGlyphs[14], &AllGlyphs[27], &AllGlyphs[80], &AllGlyphs[25], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x6D"), &AllGlyphs[23], &AllGlyphs[75], &AllGlyphs[74], &AllGlyphs[12], &AllGlyphs[85], &AllGlyphs[24], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x6E"), &AllGlyphs[89], &AllGlyphs[66], &AllGlyphs[78], &AllGlyphs[66], &AllGlyphs[22], &AllGlyphs[89], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x6F"), &AllGlyphs[76], &AllGlyphs[16], &AllGlyphs[25], &AllGlyphs[80], &AllGlyphs[14], &AllGlyphs[27], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x70"), &AllGlyphs[14], &AllGlyphs[25], &AllGlyphs[16], &AllGlyphs[79], &AllGlyphs[76], &AllGlyphs[15], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x71"), &AllGlyphs[69], &AllGlyphs[67], &AllGlyphs[90], &AllGlyphs[69], &AllGlyphs[67], &AllGlyphs[81], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x72"), &AllGlyphs[70], &AllGlyphs[20], &AllGlyphs[20], &AllGlyphs[82], &AllGlyphs[70], &AllGlyphs[86], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x73"), &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 1, 1, 1, 1, 1, 1 },
    { wxString::FromUTF8("\x74"), &AllGlyphs[71], &AllGlyphs[21], &AllGlyphs[21], &AllGlyphs[84], &AllGlyphs[71], &AllGlyphs[72], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x75"), &AllGlyphs[74], &AllGlyphs[24], &AllGlyphs[23], &AllGlyphs[73], &AllGlyphs[77], &AllGlyphs[75], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\x76"), &AllGlyphs[82], &AllGlyphs[70], &AllGlyphs[86], &AllGlyphs[70], &AllGlyphs[20], &AllGlyphs[82], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\x77"), &AllGlyphs[68], &AllGlyphs[88], &AllGlyphs[88], &AllGlyphs[87], &AllGlyphs[68], &AllGlyphs[65], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x78"), &AllGlyphs[65], &AllGlyphs[87], &AllGlyphs[87], &AllGlyphs[88], &AllGlyphs[65], &AllGlyphs[68], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x79"), &AllGlyphs[66], &AllGlyphs[22], &AllGlyphs[22], &AllGlyphs[89], &AllGlyphs[66], &AllGlyphs[78], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\x7A"), &AllGlyphs[81], &AllGlyphs[69], &AllGlyphs[81], &AllGlyphs[67], &AllGlyphs[90], &AllGlyphs[69], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x7B"), &AllGlyphs[63], &AllGlyphs[11], &AllGlyphs[63], &AllGlyphs[93], &AllGlyphs[91], &AllGlyphs[11], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\x7C"), &AllGlyphs[60], &AllGlyphs[92], &AllGlyphs[92], &AllGlyphs[92], &AllGlyphs[60], &AllGlyphs[60], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\x7D"), &AllGlyphs[91], &AllGlyphs[63], &AllGlyphs[11], &AllGlyphs[91], &AllGlyphs[63], &AllGlyphs[93], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\x7E"), &AllGlyphs[1], &AllGlyphs[94], &AllGlyphs[94], &AllGlyphs[94], &AllGlyphs[1], &AllGlyphs[1], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xA1"), &AllGlyphs[98], &AllGlyphs[96], &AllGlyphs[95], &AllGlyphs[96], &AllGlyphs[97], &AllGlyphs[98], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC2\xA2"), &AllGlyphs[97], &AllGlyphs[95], &AllGlyphs[96], &AllGlyphs[95], &AllGlyphs[98], &AllGlyphs[97], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC2\xA3"), &AllGlyphs[95], &AllGlyphs[98], &AllGlyphs[98], &AllGlyphs[97], &AllGlyphs[95], &AllGlyphs[96], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xA4"), &AllGlyphs[96], &AllGlyphs[97], &AllGlyphs[97], &AllGlyphs[98], &AllGlyphs[96], &AllGlyphs[95], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xA5"), &AllGlyphs[100], &AllGlyphs[101], &AllGlyphs[102], &AllGlyphs[100], &AllGlyphs[101], &AllGlyphs[99], Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC2\xA6"), &AllGlyphs[101], &AllGlyphs[102], &AllGlyphs[101], &AllGlyphs[99], &AllGlyphs[100], &AllGlyphs[102], Connection::DIAG_BACK, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_FRONT, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC2\xA7"), &AllGlyphs[102], &AllGlyphs[99], &AllGlyphs[100], &AllGlyphs[102], &AllGlyphs[99], &AllGlyphs[101], Connection::DIAG_BOTH, Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::DIAG_BACK, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC2\xA8"), &AllGlyphs[99], &AllGlyphs[100], &AllGlyphs[99], &AllGlyphs[101], &AllGlyphs[102], &AllGlyphs[100], Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::DIAG_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC2\xA9"), &AllGlyphs[104], &AllGlyphs[105], &AllGlyphs[105], &AllGlyphs[103], &AllGlyphs[104], &AllGlyphs[106], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xAA"), &AllGlyphs[105], &AllGlyphs[106], &AllGlyphs[104], &AllGlyphs[106], &AllGlyphs[103], &AllGlyphs[105], Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC2\xAB"), &AllGlyphs[106], &AllGlyphs[103], &AllGlyphs[103], &AllGlyphs[105], &AllGlyphs[106], &AllGlyphs[104], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xAC"), &AllGlyphs[103], &AllGlyphs[104], &AllGlyphs[106], &AllGlyphs[104], &AllGlyphs[105], &AllGlyphs[103], Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC2\xAE"), &AllGlyphs[108], &AllGlyphs[109], &AllGlyphs[108], &AllGlyphs[110], &AllGlyphs[107], &AllGlyphs[109], Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC2\xAF"), &AllGlyphs[109], &AllGlyphs[110], &AllGlyphs[107], &AllGlyphs[109], &AllGlyphs[110], &AllGlyphs[108], Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_FRONT, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC2\xB0"), &AllGlyphs[110], &AllGlyphs[107], &AllGlyphs[110], &AllGlyphs[108], &AllGlyphs[109], &AllGlyphs[107], Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC2\xB1"), &AllGlyphs[107], &AllGlyphs[108], &AllGlyphs[109], &AllGlyphs[107], &AllGlyphs[108], &AllGlyphs[110], Connection::EMPTY, Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_FRONT, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC2\xB2"), &AllGlyphs[112], &AllGlyphs[113], &AllGlyphs[113], &AllGlyphs[111], &AllGlyphs[112], &AllGlyphs[114], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xB3"), &AllGlyphs[113], &AllGlyphs[114], &AllGlyphs[112], &AllGlyphs[114], &AllGlyphs[111], &AllGlyphs[113], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC2\xB4"), &AllGlyphs[114], &AllGlyphs[111], &AllGlyphs[111], &AllGlyphs[113], &AllGlyphs[114], &AllGlyphs[112], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xB5"), &AllGlyphs[111], &AllGlyphs[112], &AllGlyphs[114], &AllGlyphs[112], &AllGlyphs[113], &AllGlyphs[111], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC2\xB6"), &AllGlyphs[116], &AllGlyphs[117], &AllGlyphs[116], &AllGlyphs[118], &AllGlyphs[115], &AllGlyphs[117], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC2\xB8"), &AllGlyphs[117], &AllGlyphs[118], &AllGlyphs[115], &AllGlyphs[117], &AllGlyphs[118], &AllGlyphs[116], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC2\xB9"), &AllGlyphs[118], &AllGlyphs[115], &AllGlyphs[118], &AllGlyphs[116], &AllGlyphs[117], &AllGlyphs[115], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC2\xBA"), &AllGlyphs[115], &AllGlyphs[116], &AllGlyphs[117], &AllGlyphs[115], &AllGlyphs[116], &AllGlyphs[118], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC2\xBB"), &AllGlyphs[122], &AllGlyphs[120], &AllGlyphs[119], &AllGlyphs[120], &AllGlyphs[121], &AllGlyphs[122], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC2\xBC"), &AllGlyphs[121], &AllGlyphs[119], &AllGlyphs[120], &AllGlyphs[119], &AllGlyphs[122], &AllGlyphs[121], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC2\xBD"), &AllGlyphs[119], &AllGlyphs[122], &AllGlyphs[122], &AllGlyphs[121], &AllGlyphs[119], &AllGlyphs[120], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xBE"), &AllGlyphs[120], &AllGlyphs[121], &AllGlyphs[121], &AllGlyphs[122], &AllGlyphs[120], &AllGlyphs[119], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC2\xBF"), &AllGlyphs[126], &AllGlyphs[124], &AllGlyphs[123], &AllGlyphs[124], &AllGlyphs[125], &AllGlyphs[126], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x80"), &AllGlyphs[125], &AllGlyphs[123], &AllGlyphs[124], &AllGlyphs[123], &AllGlyphs[126], &AllGlyphs[125], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x81"), &AllGlyphs[123], &AllGlyphs[126], &AllGlyphs[126], &AllGlyphs[125], &AllGlyphs[123], &AllGlyphs[124], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\x82"), &AllGlyphs[124], &AllGlyphs[125], &AllGlyphs[125], &AllGlyphs[126], &AllGlyphs[124], &AllGlyphs[123], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\x83"), &AllGlyphs[128], &AllGlyphs[129], &AllGlyphs[128], &AllGlyphs[130], &AllGlyphs[127], &AllGlyphs[129], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC3\x84"), &AllGlyphs[129], &AllGlyphs[130], &AllGlyphs[127], &AllGlyphs[129], &AllGlyphs[130], &AllGlyphs[128], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC3\x85"), &AllGlyphs[130], &AllGlyphs[127], &AllGlyphs[130], &AllGlyphs[128], &AllGlyphs[129], &AllGlyphs[127], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC3\x86"), &AllGlyphs[127], &AllGlyphs[128], &AllGlyphs[129], &AllGlyphs[127], &AllGlyphs[128], &AllGlyphs[130], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC3\x87"), &AllGlyphs[134], &AllGlyphs[132], &AllGlyphs[132], &AllGlyphs[131], &AllGlyphs[134], &AllGlyphs[133], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\x88"), &AllGlyphs[133], &AllGlyphs[131], &AllGlyphs[131], &AllGlyphs[132], &AllGlyphs[133], &AllGlyphs[134], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\x89"), &AllGlyphs[131], &AllGlyphs[134], &AllGlyphs[133], &AllGlyphs[134], &AllGlyphs[132], &AllGlyphs[131], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x8A"), &AllGlyphs[132], &AllGlyphs[133], &AllGlyphs[134], &AllGlyphs[133], &AllGlyphs[131], &AllGlyphs[132], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x8B"), &AllGlyphs[136], &AllGlyphs[138], &AllGlyphs[137], &AllGlyphs[136], &AllGlyphs[138], &AllGlyphs[135], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC3\x8C"), &AllGlyphs[138], &AllGlyphs[137], &AllGlyphs[138], &AllGlyphs[135], &AllGlyphs[136], &AllGlyphs[137], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC3\x8D"), &AllGlyphs[135], &AllGlyphs[136], &AllGlyphs[135], &AllGlyphs[138], &AllGlyphs[137], &AllGlyphs[136], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC3\x8E"), &AllGlyphs[137], &AllGlyphs[135], &AllGlyphs[136], &AllGlyphs[137], &AllGlyphs[135], &AllGlyphs[138], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC3\x8F"), &AllGlyphs[140], &AllGlyphs[142], &AllGlyphs[141], &AllGlyphs[140], &AllGlyphs[142], &AllGlyphs[139], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC3\x90"), &AllGlyphs[142], &AllGlyphs[141], &AllGlyphs[142], &AllGlyphs[139], &AllGlyphs[140], &AllGlyphs[141], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC3\x91"), &AllGlyphs[139], &AllGlyphs[140], &AllGlyphs[139], &AllGlyphs[142], &AllGlyphs[141], &AllGlyphs[140], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC3\x92"), &AllGlyphs[141], &AllGlyphs[139], &AllGlyphs[140], &AllGlyphs[141], &AllGlyphs[139], &AllGlyphs[142], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC3\x93"), &AllGlyphs[145], &AllGlyphs[143], &AllGlyphs[144], &AllGlyphs[144], &AllGlyphs[146], &AllGlyphs[146], Connection::ORTHO_LEFT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, 0, 1, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x94"), &AllGlyphs[146], &AllGlyphs[144], &AllGlyphs[143], &AllGlyphs[143], &AllGlyphs[145], &AllGlyphs[145], Connection::ORTHO_RIGHT, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, 0, 1, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x95"), &AllGlyphs[143], &AllGlyphs[145], &AllGlyphs[146], &AllGlyphs[146], &AllGlyphs[144], &AllGlyphs[144], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::ORTHO_UP, 0, 1, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x96"), &AllGlyphs[144], &AllGlyphs[146], &AllGlyphs[145], &AllGlyphs[145], &AllGlyphs[143], &AllGlyphs[143], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_DOWN, 0, 1, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x97"), &AllGlyphs[148], &AllGlyphs[149], &AllGlyphs[149], &AllGlyphs[147], &AllGlyphs[148], &AllGlyphs[150], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\x98"), &AllGlyphs[149], &AllGlyphs[150], &AllGlyphs[148], &AllGlyphs[150], &AllGlyphs[147], &AllGlyphs[149], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x99"), &AllGlyphs[150], &AllGlyphs[147], &AllGlyphs[147], &AllGlyphs[149], &AllGlyphs[150], &AllGlyphs[148], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\x9A"), &AllGlyphs[147], &AllGlyphs[148], &AllGlyphs[150], &AllGlyphs[148], &AllGlyphs[149], &AllGlyphs[147], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x9B"), &AllGlyphs[152], &AllGlyphs[153], &AllGlyphs[153], &AllGlyphs[151], &AllGlyphs[152], &AllGlyphs[154], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\x9C"), &AllGlyphs[153], &AllGlyphs[154], &AllGlyphs[152], &AllGlyphs[154], &AllGlyphs[151], &AllGlyphs[153], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x9D"), &AllGlyphs[154], &AllGlyphs[151], &AllGlyphs[151], &AllGlyphs[153], &AllGlyphs[154], &AllGlyphs[152], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\x9E"), &AllGlyphs[151], &AllGlyphs[152], &AllGlyphs[154], &AllGlyphs[152], &AllGlyphs[153], &AllGlyphs[151], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\x9F"), &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1, 1, 1 },
    { wxString::FromUTF8("\xC3\xA0"), &AllGlyphs[157], &AllGlyphs[158], &AllGlyphs[159], &AllGlyphs[157], &AllGlyphs[158], &AllGlyphs[156], Connection::ORTHO_RIGHT, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC3\xA1"), &AllGlyphs[158], &AllGlyphs[159], &AllGlyphs[158], &AllGlyphs[156], &AllGlyphs[157], &AllGlyphs[159], Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC3\xA2"), &AllGlyphs[159], &AllGlyphs[156], &AllGlyphs[157], &AllGlyphs[159], &AllGlyphs[156], &AllGlyphs[158], Connection::ORTHO_BOTH, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_UP, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xC3\xA3"), &AllGlyphs[156], &AllGlyphs[157], &AllGlyphs[156], &AllGlyphs[158], &AllGlyphs[159], &AllGlyphs[157], Connection::ORTHO_BOTH, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xC3\xB0"), &AllGlyphs[163], &AllGlyphs[161], &AllGlyphs[161], &AllGlyphs[160], &AllGlyphs[163], &AllGlyphs[162], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\xB1"), &AllGlyphs[162], &AllGlyphs[160], &AllGlyphs[160], &AllGlyphs[161], &AllGlyphs[162], &AllGlyphs[163], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, Connection::DIAG_FRONT, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xC3\xB2"), &AllGlyphs[160], &AllGlyphs[163], &AllGlyphs[162], &AllGlyphs[163], &AllGlyphs[161], &AllGlyphs[160], Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xB3"), &AllGlyphs[161], &AllGlyphs[162], &AllGlyphs[163], &AllGlyphs[162], &AllGlyphs[160], &AllGlyphs[161], Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xB4"), &AllGlyphs[169], &AllGlyphs[167], &AllGlyphs[166], &AllGlyphs[165], &AllGlyphs[171], &AllGlyphs[168], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xB5"), &AllGlyphs[171], &AllGlyphs[166], &AllGlyphs[167], &AllGlyphs[164], &AllGlyphs[169], &AllGlyphs[170], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xB6"), &AllGlyphs[168], &AllGlyphs[165], &AllGlyphs[164], &AllGlyphs[167], &AllGlyphs[170], &AllGlyphs[169], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xB7"), &AllGlyphs[170], &AllGlyphs[164], &AllGlyphs[165], &AllGlyphs[166], &AllGlyphs[168], &AllGlyphs[171], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xB8"), &AllGlyphs[165], &AllGlyphs[171], &AllGlyphs[170], &AllGlyphs[169], &AllGlyphs[167], &AllGlyphs[164], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xB9"), &AllGlyphs[167], &AllGlyphs[170], &AllGlyphs[171], &AllGlyphs[168], &AllGlyphs[165], &AllGlyphs[166], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xBB"), &AllGlyphs[164], &AllGlyphs[169], &AllGlyphs[168], &AllGlyphs[171], &AllGlyphs[166], &AllGlyphs[165], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC3\xBC"), &AllGlyphs[166], &AllGlyphs[168], &AllGlyphs[169], &AllGlyphs[170], &AllGlyphs[164], &AllGlyphs[167], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC4\xA3"), &AllGlyphs[173], &AllGlyphs[174], &AllGlyphs[178], &AllGlyphs[176], &AllGlyphs[177], &AllGlyphs[179], Connection::EMPTY, Connection::DIAG_FRONT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC4\xA4"), &AllGlyphs[174], &AllGlyphs[175], &AllGlyphs[177], &AllGlyphs[179], &AllGlyphs[176], &AllGlyphs[178], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::DIAG_FRONT, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC4\xA5"), &AllGlyphs[175], &AllGlyphs[172], &AllGlyphs[176], &AllGlyphs[178], &AllGlyphs[179], &AllGlyphs[177], Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC4\xA6"), &AllGlyphs[172], &AllGlyphs[173], &AllGlyphs[179], &AllGlyphs[177], &AllGlyphs[178], &AllGlyphs[176], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::DIAG_FRONT, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC4\xA7"), &AllGlyphs[177], &AllGlyphs[178], &AllGlyphs[174], &AllGlyphs[172], &AllGlyphs[173], &AllGlyphs[175], Connection::EMPTY, Connection::DIAG_BACK, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC4\xA8"), &AllGlyphs[178], &AllGlyphs[179], &AllGlyphs[173], &AllGlyphs[175], &AllGlyphs[172], &AllGlyphs[174], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::DIAG_BACK, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC4\xA9"), &AllGlyphs[179], &AllGlyphs[176], &AllGlyphs[172], &AllGlyphs[174], &AllGlyphs[175], &AllGlyphs[173], Connection::DIAG_BACK, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC4\xAA"), &AllGlyphs[176], &AllGlyphs[177], &AllGlyphs[175], &AllGlyphs[173], &AllGlyphs[174], &AllGlyphs[172], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, 0, 0, 0, 0, 0, 0 },
    { wxString::FromUTF8("\xC5\x92"), &AllGlyphs[183], &AllGlyphs[180], &AllGlyphs[180], &AllGlyphs[180], &AllGlyphs[183], &AllGlyphs[183], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\xC5\xA0"), &AllGlyphs[182], &AllGlyphs[189], &AllGlyphs[181], &AllGlyphs[189], &AllGlyphs[188], &AllGlyphs[182], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0, 0, 0 },
    { wxString::FromUTF8("\xCB\x86"), &AllGlyphs[189], &AllGlyphs[188], &AllGlyphs[188], &AllGlyphs[182], &AllGlyphs[189], &AllGlyphs[181], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xE2\x80\x9A"), &AllGlyphs[180], &AllGlyphs[183], &AllGlyphs[183], &AllGlyphs[183], &AllGlyphs[180], &AllGlyphs[180], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1, 0, 0 },
    { wxString::FromUTF8("\xE2\x80\x9E"), &AllGlyphs[187], &AllGlyphs[186], &AllGlyphs[185], &AllGlyphs[187], &AllGlyphs[186], &AllGlyphs[184], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xE2\x80\xA0"), &AllGlyphs[184], &AllGlyphs[187], &AllGlyphs[184], &AllGlyphs[186], &AllGlyphs[185], &AllGlyphs[187], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xE2\x80\xA1"), &AllGlyphs[185], &AllGlyphs[184], &AllGlyphs[187], &AllGlyphs[185], &AllGlyphs[184], &AllGlyphs[186], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 0, 1 },
    { wxString::FromUTF8("\xE2\x80\xA6"), &AllGlyphs[186], &AllGlyphs[185], &AllGlyphs[186], &AllGlyphs[184], &AllGlyphs[187], &AllGlyphs[185], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0, 1, 0 },
    { wxString::FromUTF8("\xE2\x80\xB0"), &AllGlyphs[181], &AllGlyphs[182], &AllGlyphs[182], &AllGlyphs[188], &AllGlyphs[181], &AllGlyphs[189], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1, 0, 0 },
    { wxString::FromUTF8("\xE2\x80\xB9"), &AllGlyphs[188], &AllGlyphs[181], &AllGlyphs[189], &AllGlyphs[181], &AllGlyphs[182], &AllGlyphs[188], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0, 0, 0 },
} };
/// The default Glyph to fill the Knot upon initialization, which is set as the \c space character, \c \x20
const Glyph* const DefaultGlyph = &AllGlyphs[0];

inline GlyphVec1 PossibleGlyphs(const Connection up, const Connection down, const Connection left, const Connection right, const GlyphFlag boolFlags)
/// This function takes in the desired flags and outputs the vector of all glyphs which meet the criteria.
{
	/// \param up The Connection desired for the upper side. If this does not matter, then pass Connection::DO_NOT_CARE
	/// \param down The Connection desired for the lower side. If this does not matter, then pass Connection::DO_NOT_CARE
	/// \param left The Connection desired for the left side. If this does not matter, then pass Connection::DO_NOT_CARE
	/// \param right The Connection desired for the right side. If this does not matter, then pass Connection::DO_NOT_CARE
	/// \param boolFlag The other condition flags to check for the glyphs, passed by using \c operator| on GlyphFlag values. Any bits with a value of \c 0 are ignored
	
	/// \b Method

	/// This function allows all properties of the Glyphs to be checked at once, instead of checking multiple properties in order.
	/// Using this function, as opposed to the previous methods, sped up the knot generating by a large factor.

	/// First a bit mask is created. If the \c up Connection is anything other than Connection::DO_NOT_CARE, then use GlyphFlag::UP in the mask.
	/// Do this same process for the other 3 Connection values. As well, add the \c boolFlags to the mask with the GlyphFlag::COND_MASK applied.
	/// It is assumed that if a flag is nonzero, it is in use. If it is zero, it is ignored.
	const GlyphFlag mask = (up != Connection::DO_NOT_CARE	? GlyphFlag::UP		: GlyphFlag::NONE)	// If the UP side is used, add 0x000F to the mask. If not used, it will be GlyphFlag::NONE (== 0)
						 | (down != Connection::DO_NOT_CARE	? GlyphFlag::DOWN	: GlyphFlag::NONE)	// If the DOWN side is used, add 0x00F0 to the mask. If not used, it will be GlyphFlag::NONE (== 0)
						 | (left != Connection::DO_NOT_CARE	? GlyphFlag::LEFT	: GlyphFlag::NONE)	// etc, for LEFT
						 | (right != Connection::DO_NOT_CARE ? GlyphFlag::RIGHT	: GlyphFlag::NONE)	// etc, for RIGHT
						 | (boolFlags & GlyphFlag::COND_MASK); // All the other flags are only 1 bit long, so this is acceptable

	/// Next, determine which value needs to be checked against. For the 4 sides, use the toFlag() function.
	/// Add the boolFlags, with the GlyphFlag::COND_MASK applied.
	const GlyphFlag toCheck = toFlag(up, GlyphFlag::UP) | toFlag(down, GlyphFlag::DOWN) | toFlag(left, GlyphFlag::LEFT) | toFlag(right, GlyphFlag::RIGHT) | (boolFlags & GlyphFlag::COND_MASK);
	
	/// Once all that is done, loop through AllGlyphs. If the \c flags member of the Glyph with the mask applied from above
	/// is equal to the value which needs to be checked, then add the pointer to this Glyph to the output vector.
	GlyphVec1 glyphList;
	for (const Glyph& glyph : AllGlyphs)
		if ((glyph.flags & mask) == toCheck)
			glyphList.push_back(&glyph);
	return glyphList;
}
