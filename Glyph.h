#pragma once
#include "Constants.h"

struct GlyphFlag {
	enum : unsigned int {
		UP		= 0b1111 <<  0,	
		DOWN	= 0b1111 <<  4,
		LEFT	= 0b1111 <<  8,
		RIGHT	= 0b1111 << 12,
		SIDE_MASK = UP + DOWN + LEFT + RIGHT,

		SA_ROT4	= 0b0001 << 16,	// Same after rotation of 90 degrees
		SA_ROT2 = 0b0001 << 17, // Same after rotation of 180 degrees
		SA_MIRX = 0b0001 << 18,	// Same after mirror X
		SA_MIRY = 0b0001 << 19,	// Same after mirror Y
		CT_ROT4U = 0b0001 << 20, // Can connect to itself on the up side after a 90 degree rotation
		CT_ROT4D = 0b0001 << 21, // Can connect to itself on the down side after a 90 degree rotation
		CT_ROT4R = 0b0001 << 22, // Can connect to itself on the left side after a 90 degree rotation
		CT_ROT4L = 0b0001 << 23, // Can connect to itself on the right side after a 90 degree rotation
		CT_ROT2U = 0b0001 << 24, // Can connect to itself on the up side after a 180 degree rotation
		CT_ROT2D = 0b0001 << 25, // Can connect to itself on the down side after a 180 degree rotation
		CT_ROT2L = 0b0001 << 26, // Can connect to itself on the left side after a 180 degree rotation
		CT_ROT2R = 0b0001 << 27, // Can connect to itself on the right side after a 180 degree rotation
		CT_MIRU = 0b0001 << 28, // Can connect to itself on the up side after a relevant mirror
		CT_MIRD = 0b0001 << 29, // Can connect to itself on the down side after a relevant mirror
		CT_MIRL = 0b0001 << 30, // Can connect to itself on the left side after a relevant mirror
		CT_MIRR = 0b0001 << 31, // Can connect to itself on the right side after a relevant mirror
		COND_MASK = SA_ROT4 + SA_ROT2 + SA_MIRX + SA_MIRY + CT_ROT4U + CT_ROT4D + CT_ROT4L + CT_ROT4R + 
					CT_ROT2U + CT_ROT2D + CT_ROT2L + CT_ROT2R + CT_MIRU + CT_MIRD + CT_MIRL + CT_MIRR,
		
		FULL_MASK = SIDE_MASK + COND_MASK
	};
};

struct Connection {
	enum Type : unsigned int {
		DO_NOT_CARE = 0,
		EMPTY		= 1, 
		DIAG_BOTH	= 2,
		ORTHO_BOTH	= 3,
		DIAG_FRONT	= 4,
		DIAG_BACK	= 5,
		ORTHO_UP	= 6,
		ORTHO_DOWN	= 7,
		ORTHO_LEFT	= 8,
		ORTHO_RIGHT = 9,
	};

	static constexpr Type rotate4Types[] = { DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_RIGHT, ORTHO_LEFT, ORTHO_UP, ORTHO_DOWN };
	static constexpr Type rotate2Types[] = { DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_DOWN, ORTHO_UP, ORTHO_RIGHT, ORTHO_LEFT };
	static constexpr Type mirrorXTypes[] = { DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_DOWN, ORTHO_UP, ORTHO_LEFT, ORTHO_RIGHT };
	static constexpr Type mirrorYTypes[] = { DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_UP, ORTHO_DOWN, ORTHO_RIGHT, ORTHO_LEFT };
};

struct Side {
	enum : unsigned int {
		NONE	= 0,
		UP		= 1,
		DOWN	= 2,
		LEFT	= 4,
		RIGHT	= 8
	};
};

struct Glyph {
	const wxString chr;

	const Glyph* const rotated4;
	const Glyph* const rotated2;
	const Glyph* const mirroredX;
	const Glyph* const mirroredY;

	union {
		struct {
			Connection::Type up : 4;
			Connection::Type down : 4;
			Connection::Type left : 4;
			Connection::Type right : 4;
			unsigned int sameAfterRotate4 : 1;
			unsigned int sameAfterRotate2 : 1;
			unsigned int sameAfterMirrorX : 1;
			unsigned int sameAfterMirrorY : 1;
			unsigned int connectToRotate4Up : 1;
			unsigned int connectToRotate4Down : 1;
			unsigned int connectToRotate4Left : 1;
			unsigned int connectToRotate4Right : 1;
			unsigned int connectToRotate2Up : 1;
			unsigned int connectToRotate2Down : 1;
			unsigned int connectToRotate2Left : 1;
			unsigned int connectToRotate2Right : 1;
			unsigned int connectToMirrorUp : 1;
			unsigned int connectToMirrorDown : 1;
			unsigned int connectToMirrorLeft : 1;
			unsigned int connectToMirrorRight : 1;
		};
		unsigned int flags;
	};

	Glyph(wxString chr, const Glyph* rotated4, const Glyph* rotated2, const Glyph* mirroredX, const Glyph* mirroredY,
	Connection::Type up, Connection::Type down, Connection::Type left, Connection::Type right,
	unsigned int sameAfterRotate4, unsigned int sameAfterRotate2, unsigned int sameAfterMirrorX, unsigned int sameAfterMirrorY) :
		chr(chr), rotated4(rotated4), rotated2(rotated2), mirroredX(mirroredX), mirroredY(mirroredY),
		up(up), down(down), left(left), right(right),
		sameAfterRotate4(sameAfterRotate4), sameAfterRotate2(sameAfterRotate2),
		sameAfterMirrorX(sameAfterMirrorX), sameAfterMirrorY(sameAfterMirrorY),
		connectToRotate4Up		(up		== Connection::rotate4Types[up]),
		connectToRotate4Down	(down	== Connection::rotate4Types[down]),
		connectToRotate4Left	(left	== Connection::rotate4Types[left]),
		connectToRotate4Right	(right	== Connection::rotate4Types[right]),
		connectToRotate2Up		(up		== Connection::rotate2Types[up]),
		connectToRotate2Down	(down	== Connection::rotate2Types[down]),
		connectToRotate2Left	(left	== Connection::rotate2Types[left]),
		connectToRotate2Right	(right	== Connection::rotate2Types[right]),
		connectToMirrorUp		(up		== Connection::mirrorXTypes[up]),
		connectToMirrorDown		(down	== Connection::mirrorXTypes[down]),
		connectToMirrorLeft		(left	== Connection::mirrorYTypes[left]),
		connectToMirrorRight	(right	== Connection::mirrorYTypes[right]) {}

	static constexpr int TOTAL = 190;
};

using GlyphVec1 = std::vector<const Glyph*>;
using GlyphVec2 = std::vector<GlyphVec1>;

const inline std::array<Glyph, Glyph::TOTAL> AllGlyphs{ {
	{ wxString::FromUTF8("\x20"), &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], &AllGlyphs[0], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 1, 1, 1, 1 },
	{ wxString::FromUTF8("\x21"), &AllGlyphs[94], &AllGlyphs[1], &AllGlyphs[1], &AllGlyphs[1], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x22"), &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], &AllGlyphs[2], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1 },
	{ wxString::FromUTF8("\x23"), &AllGlyphs[32], &AllGlyphs[3], &AllGlyphs[3], &AllGlyphs[3], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x24"), &AllGlyphs[54], &AllGlyphs[50], &AllGlyphs[50], &AllGlyphs[4], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x25"), &AllGlyphs[40], &AllGlyphs[52], &AllGlyphs[52], &AllGlyphs[5], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x26"), &AllGlyphs[43], &AllGlyphs[41], &AllGlyphs[53], &AllGlyphs[10], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x27"), &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], &AllGlyphs[7], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 1, 1, 1, 1 },
	{ wxString::FromUTF8("\x28"), &AllGlyphs[9], &AllGlyphs[48], &AllGlyphs[47], &AllGlyphs[9], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x29"), &AllGlyphs[48], &AllGlyphs[47], &AllGlyphs[48], &AllGlyphs[8], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x2A"), &AllGlyphs[28], &AllGlyphs[53], &AllGlyphs[41], &AllGlyphs[6], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x2B"), &AllGlyphs[93], &AllGlyphs[91], &AllGlyphs[93], &AllGlyphs[63], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x2C"), &AllGlyphs[85], &AllGlyphs[74], &AllGlyphs[75], &AllGlyphs[77], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x2D"), &AllGlyphs[61], &AllGlyphs[29], &AllGlyphs[29], &AllGlyphs[13], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x2E"), &AllGlyphs[25], &AllGlyphs[27], &AllGlyphs[76], &AllGlyphs[15], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x2F"), &AllGlyphs[79], &AllGlyphs[76], &AllGlyphs[27], &AllGlyphs[14], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x30"), &AllGlyphs[15], &AllGlyphs[79], &AllGlyphs[80], &AllGlyphs[25], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x31"), &AllGlyphs[64], &AllGlyphs[17], &AllGlyphs[17], &AllGlyphs[17], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x32"), &AllGlyphs[19], &AllGlyphs[18], &AllGlyphs[18], &AllGlyphs[18], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x33"), &AllGlyphs[18], &AllGlyphs[19], &AllGlyphs[19], &AllGlyphs[19], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x34"), &AllGlyphs[86], &AllGlyphs[82], &AllGlyphs[82], &AllGlyphs[20], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x35"), &AllGlyphs[72], &AllGlyphs[84], &AllGlyphs[84], &AllGlyphs[21], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x36"), &AllGlyphs[78], &AllGlyphs[89], &AllGlyphs[89], &AllGlyphs[22], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x37"), &AllGlyphs[75], &AllGlyphs[73], &AllGlyphs[85], &AllGlyphs[24], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x38"), &AllGlyphs[12], &AllGlyphs[85], &AllGlyphs[73], &AllGlyphs[23], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x39"), &AllGlyphs[27], &AllGlyphs[80], &AllGlyphs[79], &AllGlyphs[16], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x3A"), &AllGlyphs[31], &AllGlyphs[30], &AllGlyphs[31], &AllGlyphs[44], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x3B"), &AllGlyphs[80], &AllGlyphs[14], &AllGlyphs[15], &AllGlyphs[76], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x3C"), &AllGlyphs[53], &AllGlyphs[42], &AllGlyphs[43], &AllGlyphs[45], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x3D"), &AllGlyphs[59], &AllGlyphs[13], &AllGlyphs[13], &AllGlyphs[29], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x3E"), &AllGlyphs[44], &AllGlyphs[26], &AllGlyphs[44], &AllGlyphs[31], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x3F"), &AllGlyphs[30], &AllGlyphs[44], &AllGlyphs[26], &AllGlyphs[30], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x40"), &AllGlyphs[3], &AllGlyphs[32], &AllGlyphs[32], &AllGlyphs[32], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x41"), &AllGlyphs[55], &AllGlyphs[36], &AllGlyphs[33], &AllGlyphs[36], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x42"), &AllGlyphs[62], &AllGlyphs[46], &AllGlyphs[34], &AllGlyphs[46], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x43"), &AllGlyphs[58], &AllGlyphs[49], &AllGlyphs[37], &AllGlyphs[58], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x44"), &AllGlyphs[56], &AllGlyphs[33], &AllGlyphs[36], &AllGlyphs[33], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x45"), &AllGlyphs[35], &AllGlyphs[58], &AllGlyphs[35], &AllGlyphs[49], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x46"), &AllGlyphs[4], &AllGlyphs[54], &AllGlyphs[38], &AllGlyphs[54], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x47"), &AllGlyphs[5], &AllGlyphs[40], &AllGlyphs[39], &AllGlyphs[40], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x48"), &AllGlyphs[52], &AllGlyphs[39], &AllGlyphs[40], &AllGlyphs[39], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x49"), &AllGlyphs[45], &AllGlyphs[6], &AllGlyphs[10], &AllGlyphs[53], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x4A"), &AllGlyphs[10], &AllGlyphs[28], &AllGlyphs[45], &AllGlyphs[43], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x4B"), &AllGlyphs[41], &AllGlyphs[45], &AllGlyphs[28], &AllGlyphs[42], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x4C"), &AllGlyphs[26], &AllGlyphs[31], &AllGlyphs[30], &AllGlyphs[26], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x4D"), &AllGlyphs[6], &AllGlyphs[43], &AllGlyphs[42], &AllGlyphs[28], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x4E"), &AllGlyphs[57], &AllGlyphs[34], &AllGlyphs[46], &AllGlyphs[34], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x4F"), &AllGlyphs[8], &AllGlyphs[9], &AllGlyphs[8], &AllGlyphs[48], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x50"), &AllGlyphs[47], &AllGlyphs[8], &AllGlyphs[9], &AllGlyphs[47], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x51"), &AllGlyphs[37], &AllGlyphs[35], &AllGlyphs[58], &AllGlyphs[37], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x52"), &AllGlyphs[38], &AllGlyphs[4], &AllGlyphs[4], &AllGlyphs[50], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x53"), &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], &AllGlyphs[51], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1 },
	{ wxString::FromUTF8("\x54"), &AllGlyphs[39], &AllGlyphs[5], &AllGlyphs[5], &AllGlyphs[52], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x55"), &AllGlyphs[42], &AllGlyphs[10], &AllGlyphs[6], &AllGlyphs[41], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x56"), &AllGlyphs[50], &AllGlyphs[38], &AllGlyphs[54], &AllGlyphs[38], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x57"), &AllGlyphs[36], &AllGlyphs[56], &AllGlyphs[56], &AllGlyphs[55], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x58"), &AllGlyphs[33], &AllGlyphs[55], &AllGlyphs[55], &AllGlyphs[56], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x59"), &AllGlyphs[34], &AllGlyphs[62], &AllGlyphs[62], &AllGlyphs[57], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x5A"), &AllGlyphs[49], &AllGlyphs[37], &AllGlyphs[49], &AllGlyphs[35], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x5B"), &AllGlyphs[13], &AllGlyphs[61], &AllGlyphs[59], &AllGlyphs[61], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x5C"), &AllGlyphs[92], &AllGlyphs[60], &AllGlyphs[60], &AllGlyphs[60], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x5D"), &AllGlyphs[29], &AllGlyphs[59], &AllGlyphs[61], &AllGlyphs[59], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x5E"), &AllGlyphs[46], &AllGlyphs[57], &AllGlyphs[57], &AllGlyphs[62], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x5F"), &AllGlyphs[11], &AllGlyphs[93], &AllGlyphs[91], &AllGlyphs[11], Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x60"), &AllGlyphs[17], &AllGlyphs[64], &AllGlyphs[64], &AllGlyphs[64], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x61"), &AllGlyphs[87], &AllGlyphs[68], &AllGlyphs[65], &AllGlyphs[68], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x62"), &AllGlyphs[22], &AllGlyphs[78], &AllGlyphs[66], &AllGlyphs[78], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x63"), &AllGlyphs[90], &AllGlyphs[81], &AllGlyphs[69], &AllGlyphs[90], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x64"), &AllGlyphs[88], &AllGlyphs[65], &AllGlyphs[68], &AllGlyphs[65], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x65"), &AllGlyphs[67], &AllGlyphs[90], &AllGlyphs[67], &AllGlyphs[81], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x66"), &AllGlyphs[20], &AllGlyphs[86], &AllGlyphs[70], &AllGlyphs[86], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x67"), &AllGlyphs[21], &AllGlyphs[72], &AllGlyphs[71], &AllGlyphs[72], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x68"), &AllGlyphs[84], &AllGlyphs[71], &AllGlyphs[72], &AllGlyphs[71], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x69"), &AllGlyphs[77], &AllGlyphs[23], &AllGlyphs[24], &AllGlyphs[85], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x6A"), &AllGlyphs[24], &AllGlyphs[12], &AllGlyphs[77], &AllGlyphs[75], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x6B"), &AllGlyphs[73], &AllGlyphs[77], &AllGlyphs[12], &AllGlyphs[74], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x6C"), &AllGlyphs[16], &AllGlyphs[15], &AllGlyphs[14], &AllGlyphs[27], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x6D"), &AllGlyphs[23], &AllGlyphs[75], &AllGlyphs[74], &AllGlyphs[12], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x6E"), &AllGlyphs[89], &AllGlyphs[66], &AllGlyphs[78], &AllGlyphs[66], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x6F"), &AllGlyphs[76], &AllGlyphs[16], &AllGlyphs[25], &AllGlyphs[80], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x70"), &AllGlyphs[14], &AllGlyphs[25], &AllGlyphs[16], &AllGlyphs[79], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x71"), &AllGlyphs[69], &AllGlyphs[67], &AllGlyphs[90], &AllGlyphs[69], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x72"), &AllGlyphs[70], &AllGlyphs[20], &AllGlyphs[20], &AllGlyphs[82], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x73"), &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], &AllGlyphs[83], Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 1, 1, 1, 1 },
	{ wxString::FromUTF8("\x74"), &AllGlyphs[71], &AllGlyphs[21], &AllGlyphs[21], &AllGlyphs[84], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x75"), &AllGlyphs[74], &AllGlyphs[24], &AllGlyphs[23], &AllGlyphs[73], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x76"), &AllGlyphs[82], &AllGlyphs[70], &AllGlyphs[86], &AllGlyphs[70], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\x77"), &AllGlyphs[68], &AllGlyphs[88], &AllGlyphs[88], &AllGlyphs[87], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x78"), &AllGlyphs[65], &AllGlyphs[87], &AllGlyphs[87], &AllGlyphs[88], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x79"), &AllGlyphs[66], &AllGlyphs[22], &AllGlyphs[22], &AllGlyphs[89], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\x7A"), &AllGlyphs[81], &AllGlyphs[69], &AllGlyphs[81], &AllGlyphs[67], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x7B"), &AllGlyphs[63], &AllGlyphs[11], &AllGlyphs[63], &AllGlyphs[93], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x7C"), &AllGlyphs[60], &AllGlyphs[92], &AllGlyphs[92], &AllGlyphs[92], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\x7D"), &AllGlyphs[91], &AllGlyphs[63], &AllGlyphs[11], &AllGlyphs[91], Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\x7E"), &AllGlyphs[1], &AllGlyphs[94], &AllGlyphs[94], &AllGlyphs[94], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\xC2\xA1"), &AllGlyphs[98], &AllGlyphs[96], &AllGlyphs[95], &AllGlyphs[96], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC2\xA2"), &AllGlyphs[97], &AllGlyphs[95], &AllGlyphs[96], &AllGlyphs[95], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC2\xA3"), &AllGlyphs[95], &AllGlyphs[98], &AllGlyphs[98], &AllGlyphs[97], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC2\xA4"), &AllGlyphs[96], &AllGlyphs[97], &AllGlyphs[97], &AllGlyphs[98], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC2\xA5"), &AllGlyphs[100], &AllGlyphs[101], &AllGlyphs[102], &AllGlyphs[100], Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xA6"), &AllGlyphs[101], &AllGlyphs[102], &AllGlyphs[101], &AllGlyphs[99], Connection::DIAG_BACK, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_FRONT, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xA7"), &AllGlyphs[102], &AllGlyphs[99], &AllGlyphs[100], &AllGlyphs[102], Connection::DIAG_BOTH, Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::DIAG_BACK, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xA8"), &AllGlyphs[99], &AllGlyphs[100], &AllGlyphs[99], &AllGlyphs[101], Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xA9"), &AllGlyphs[104], &AllGlyphs[105], &AllGlyphs[105], &AllGlyphs[103], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC2\xAA"), &AllGlyphs[105], &AllGlyphs[106], &AllGlyphs[104], &AllGlyphs[106], Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC2\xAB"), &AllGlyphs[106], &AllGlyphs[103], &AllGlyphs[103], &AllGlyphs[105], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC2\xAC"), &AllGlyphs[103], &AllGlyphs[104], &AllGlyphs[106], &AllGlyphs[104], Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC2\xAE"), &AllGlyphs[108], &AllGlyphs[109], &AllGlyphs[108], &AllGlyphs[110], Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xAF"), &AllGlyphs[109], &AllGlyphs[110], &AllGlyphs[107], &AllGlyphs[109], Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_FRONT, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xB0"), &AllGlyphs[110], &AllGlyphs[107], &AllGlyphs[110], &AllGlyphs[108], Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xB1"), &AllGlyphs[107], &AllGlyphs[108], &AllGlyphs[109], &AllGlyphs[107], Connection::EMPTY, Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_FRONT, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xB2"), &AllGlyphs[112], &AllGlyphs[113], &AllGlyphs[113], &AllGlyphs[111], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC2\xB3"), &AllGlyphs[113], &AllGlyphs[114], &AllGlyphs[112], &AllGlyphs[114], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC2\xB4"), &AllGlyphs[114], &AllGlyphs[111], &AllGlyphs[111], &AllGlyphs[113], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC2\xB5"), &AllGlyphs[111], &AllGlyphs[112], &AllGlyphs[114], &AllGlyphs[112], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC2\xB6"), &AllGlyphs[116], &AllGlyphs[117], &AllGlyphs[116], &AllGlyphs[118], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xB8"), &AllGlyphs[117], &AllGlyphs[118], &AllGlyphs[115], &AllGlyphs[117], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xB9"), &AllGlyphs[118], &AllGlyphs[115], &AllGlyphs[118], &AllGlyphs[116], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xBA"), &AllGlyphs[115], &AllGlyphs[116], &AllGlyphs[117], &AllGlyphs[115], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC2\xBB"), &AllGlyphs[122], &AllGlyphs[120], &AllGlyphs[119], &AllGlyphs[120], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC2\xBC"), &AllGlyphs[121], &AllGlyphs[119], &AllGlyphs[120], &AllGlyphs[119], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC2\xBD"), &AllGlyphs[119], &AllGlyphs[122], &AllGlyphs[122], &AllGlyphs[121], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC2\xBE"), &AllGlyphs[120], &AllGlyphs[121], &AllGlyphs[121], &AllGlyphs[122], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC2\xBF"), &AllGlyphs[126], &AllGlyphs[124], &AllGlyphs[123], &AllGlyphs[124], Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\x80"), &AllGlyphs[125], &AllGlyphs[123], &AllGlyphs[124], &AllGlyphs[123], Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\x81"), &AllGlyphs[123], &AllGlyphs[126], &AllGlyphs[126], &AllGlyphs[125], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\x82"), &AllGlyphs[124], &AllGlyphs[125], &AllGlyphs[125], &AllGlyphs[126], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\x83"), &AllGlyphs[128], &AllGlyphs[129], &AllGlyphs[128], &AllGlyphs[130], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x84"), &AllGlyphs[129], &AllGlyphs[130], &AllGlyphs[127], &AllGlyphs[129], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x85"), &AllGlyphs[130], &AllGlyphs[127], &AllGlyphs[130], &AllGlyphs[128], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x86"), &AllGlyphs[127], &AllGlyphs[128], &AllGlyphs[129], &AllGlyphs[127], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x87"), &AllGlyphs[134], &AllGlyphs[132], &AllGlyphs[132], &AllGlyphs[131], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\x88"), &AllGlyphs[133], &AllGlyphs[131], &AllGlyphs[131], &AllGlyphs[132], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\x89"), &AllGlyphs[131], &AllGlyphs[134], &AllGlyphs[133], &AllGlyphs[134], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\x8A"), &AllGlyphs[132], &AllGlyphs[133], &AllGlyphs[134], &AllGlyphs[133], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\x8B"), &AllGlyphs[136], &AllGlyphs[138], &AllGlyphs[137], &AllGlyphs[136], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x8C"), &AllGlyphs[138], &AllGlyphs[137], &AllGlyphs[138], &AllGlyphs[135], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x8D"), &AllGlyphs[135], &AllGlyphs[136], &AllGlyphs[135], &AllGlyphs[138], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x8E"), &AllGlyphs[137], &AllGlyphs[135], &AllGlyphs[136], &AllGlyphs[137], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x8F"), &AllGlyphs[140], &AllGlyphs[142], &AllGlyphs[141], &AllGlyphs[140], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x90"), &AllGlyphs[142], &AllGlyphs[141], &AllGlyphs[142], &AllGlyphs[139], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x91"), &AllGlyphs[139], &AllGlyphs[140], &AllGlyphs[139], &AllGlyphs[142], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x92"), &AllGlyphs[141], &AllGlyphs[139], &AllGlyphs[140], &AllGlyphs[141], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\x93"), &AllGlyphs[145], &AllGlyphs[143], &AllGlyphs[144], &AllGlyphs[144], Connection::ORTHO_LEFT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, 0, 1, 0, 0 },
	{ wxString::FromUTF8("\xC3\x94"), &AllGlyphs[146], &AllGlyphs[144], &AllGlyphs[143], &AllGlyphs[143], Connection::ORTHO_RIGHT, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, 0, 1, 0, 0 },
	{ wxString::FromUTF8("\xC3\x95"), &AllGlyphs[143], &AllGlyphs[145], &AllGlyphs[146], &AllGlyphs[146], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::ORTHO_UP, 0, 1, 0, 0 },
	{ wxString::FromUTF8("\xC3\x96"), &AllGlyphs[144], &AllGlyphs[146], &AllGlyphs[145], &AllGlyphs[145], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_DOWN, 0, 1, 0, 0 },
	{ wxString::FromUTF8("\xC3\x97"), &AllGlyphs[148], &AllGlyphs[149], &AllGlyphs[149], &AllGlyphs[147], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\x98"), &AllGlyphs[149], &AllGlyphs[150], &AllGlyphs[148], &AllGlyphs[150], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\x99"), &AllGlyphs[150], &AllGlyphs[147], &AllGlyphs[147], &AllGlyphs[149], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\x9A"), &AllGlyphs[147], &AllGlyphs[148], &AllGlyphs[150], &AllGlyphs[148], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\x9B"), &AllGlyphs[152], &AllGlyphs[153], &AllGlyphs[153], &AllGlyphs[151], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\x9C"), &AllGlyphs[153], &AllGlyphs[154], &AllGlyphs[152], &AllGlyphs[154], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\x9D"), &AllGlyphs[154], &AllGlyphs[151], &AllGlyphs[151], &AllGlyphs[153], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\x9E"), &AllGlyphs[151], &AllGlyphs[152], &AllGlyphs[154], &AllGlyphs[152], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\x9F"), &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], &AllGlyphs[155], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 1, 1, 1, 1 },
	{ wxString::FromUTF8("\xC3\xA0"), &AllGlyphs[157], &AllGlyphs[158], &AllGlyphs[159], &AllGlyphs[157], Connection::ORTHO_RIGHT, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xA1"), &AllGlyphs[158], &AllGlyphs[159], &AllGlyphs[158], &AllGlyphs[156], Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xA2"), &AllGlyphs[159], &AllGlyphs[156], &AllGlyphs[157], &AllGlyphs[159], Connection::ORTHO_BOTH, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_UP, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xA3"), &AllGlyphs[156], &AllGlyphs[157], &AllGlyphs[156], &AllGlyphs[158], Connection::ORTHO_BOTH, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xB0"), &AllGlyphs[163], &AllGlyphs[161], &AllGlyphs[161], &AllGlyphs[160], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\xB1"), &AllGlyphs[162], &AllGlyphs[160], &AllGlyphs[160], &AllGlyphs[161], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, Connection::DIAG_FRONT, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xC3\xB2"), &AllGlyphs[160], &AllGlyphs[163], &AllGlyphs[162], &AllGlyphs[163], Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\xB3"), &AllGlyphs[161], &AllGlyphs[162], &AllGlyphs[163], &AllGlyphs[162], Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, Connection::EMPTY, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xC3\xB4"), &AllGlyphs[169], &AllGlyphs[167], &AllGlyphs[166], &AllGlyphs[165], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xB5"), &AllGlyphs[171], &AllGlyphs[166], &AllGlyphs[167], &AllGlyphs[164], Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xB6"), &AllGlyphs[168], &AllGlyphs[165], &AllGlyphs[164], &AllGlyphs[167], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xB7"), &AllGlyphs[170], &AllGlyphs[164], &AllGlyphs[165], &AllGlyphs[166], Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xB8"), &AllGlyphs[165], &AllGlyphs[171], &AllGlyphs[170], &AllGlyphs[169], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xB9"), &AllGlyphs[167], &AllGlyphs[170], &AllGlyphs[171], &AllGlyphs[168], Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xBB"), &AllGlyphs[164], &AllGlyphs[169], &AllGlyphs[168], &AllGlyphs[171], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC3\xBC"), &AllGlyphs[166], &AllGlyphs[168], &AllGlyphs[169], &AllGlyphs[170], Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC4\xA3"), &AllGlyphs[173], &AllGlyphs[174], &AllGlyphs[178], &AllGlyphs[176], Connection::EMPTY, Connection::DIAG_FRONT, Connection::ORTHO_DOWN, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC4\xA4"), &AllGlyphs[174], &AllGlyphs[175], &AllGlyphs[177], &AllGlyphs[179], Connection::ORTHO_LEFT, Connection::EMPTY, Connection::DIAG_FRONT, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC4\xA5"), &AllGlyphs[175], &AllGlyphs[172], &AllGlyphs[176], &AllGlyphs[178], Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC4\xA6"), &AllGlyphs[172], &AllGlyphs[173], &AllGlyphs[179], &AllGlyphs[177], Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::DIAG_FRONT, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC4\xA7"), &AllGlyphs[177], &AllGlyphs[178], &AllGlyphs[174], &AllGlyphs[172], Connection::EMPTY, Connection::DIAG_BACK, Connection::EMPTY, Connection::ORTHO_DOWN, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC4\xA8"), &AllGlyphs[178], &AllGlyphs[179], &AllGlyphs[173], &AllGlyphs[175], Connection::EMPTY, Connection::ORTHO_LEFT, Connection::DIAG_BACK, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC4\xA9"), &AllGlyphs[179], &AllGlyphs[176], &AllGlyphs[172], &AllGlyphs[174], Connection::DIAG_BACK, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC4\xAA"), &AllGlyphs[176], &AllGlyphs[177], &AllGlyphs[175], &AllGlyphs[173], Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xC5\x92"), &AllGlyphs[183], &AllGlyphs[180], &AllGlyphs[180], &AllGlyphs[180], Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\xC5\xA0"), &AllGlyphs[182], &AllGlyphs[189], &AllGlyphs[181], &AllGlyphs[189], Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 1, 0 },
	{ wxString::FromUTF8("\xCB\x86"), &AllGlyphs[189], &AllGlyphs[188], &AllGlyphs[188], &AllGlyphs[182], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xE2\x80\x9A"), &AllGlyphs[180], &AllGlyphs[183], &AllGlyphs[183], &AllGlyphs[183], Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, 0, 1, 1, 1 },
	{ wxString::FromUTF8("\xE2\x80\x9E"), &AllGlyphs[187], &AllGlyphs[186], &AllGlyphs[185], &AllGlyphs[187], Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xE2\x80\xA0"), &AllGlyphs[184], &AllGlyphs[187], &AllGlyphs[184], &AllGlyphs[186], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xE2\x80\xA1"), &AllGlyphs[185], &AllGlyphs[184], &AllGlyphs[187], &AllGlyphs[185], Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xE2\x80\xA6"), &AllGlyphs[186], &AllGlyphs[185], &AllGlyphs[186], &AllGlyphs[184], Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 0, 0 },
	{ wxString::FromUTF8("\xE2\x80\xB0"), &AllGlyphs[181], &AllGlyphs[182], &AllGlyphs[182], &AllGlyphs[188], Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, 0, 0, 0, 1 },
	{ wxString::FromUTF8("\xE2\x80\xB9"), &AllGlyphs[188], &AllGlyphs[181], &AllGlyphs[189], &AllGlyphs[181], Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, 0, 0, 1, 0 },
} };

const Glyph* const DefaultGlyph = &AllGlyphs[0];

inline GlyphVec1 PossibleGlyphs(const Connection::Type up, const Connection::Type down, const Connection::Type left, const Connection::Type right, unsigned int boolFlags) {
	// It is assumed that if a flag is nonzero, it is in use. If it is zero, it is ignored.
	unsigned int mask = (up != 0	? GlyphFlag::UP		: 0)	// If the UP side is used, add 0x000F to the mask. If not used, it will be GlyphFlag::DO_NOT_CARE (== 0)
					  + (down != 0	? GlyphFlag::DOWN	: 0)	// If the DOWN side is used, add 0x00F0 to the mask. If not used, it will be GlyphFlag::DO_NOT_CARE (== 0)
					  + (left != 0	? GlyphFlag::LEFT	: 0)	// etc, for LEFT
					  + (right != 0 ? GlyphFlag::RIGHT	: 0)	// etc, for RIGHT
					  + (boolFlags); // All the other flags are only 1 bit long, so this is acceptable
	unsigned int toCheck = (up << 0) + (down << 4) + (left << 8) + (right << 12) + boolFlags;

	GlyphVec1 glyphList;
	for (const Glyph& glyph : AllGlyphs)
		if ((glyph.flags & mask) == toCheck)
			glyphList.push_back(&glyph);
	return glyphList;
}
