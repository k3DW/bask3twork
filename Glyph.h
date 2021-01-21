#pragma once
#include "Constants.h"
//#include <array>

struct GlyphFlag {
	enum : unsigned int {
		UP		= 0b1111 <<  0,	
		DOWN	= 0b1111 <<  4,
		LEFT	= 0b1111 <<  8,
		RIGHT	= 0b1111 << 12,
		SA_ROT	= 0b0001 << 16,	// Same after rotate
		SA_MIRX = 0b0001 << 17,	// Same after mirror X
		SA_MIRY = 0b0001 << 18,	// Same after mirror Y
		CT_MIRU = 0b0001 << 19, // Can connect to itself on the up side after a relevant mirror
		CT_MIRD = 0b0001 << 20, // Can connect to itself on the down side after a relevant mirror
		CT_MIRL = 0b0001 << 21, // Can connect to itself on the left side after a relevant mirror
		CT_MIRR = 0b0001 << 22, // Can connect to itself on the right side after a relevant mirror

		SIDE_MASK = 0b00000001111111111111111,
		COND_MASK = 0b11111110000000000000000,
		FULL_MASK = 0b11111111111111111111111
	};
};

struct Connection {
	enum Type : unsigned int {
		DO_NOT_CARE = 0,
		EMPTY		= 1, 
		DIAG_BOTH	   ,
		ORTHO_BOTH	   ,
		DIAG_FRONT	   ,
		DIAG_BACK	   ,
		ORTHO_UP	   ,
		ORTHO_DOWN	   ,
		ORTHO_LEFT	   ,
		ORTHO_RIGHT	   
	};

	static constexpr Type  rotateTypes[] = { DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_RIGHT, ORTHO_LEFT, ORTHO_UP, ORTHO_DOWN };
	static constexpr Type mirrorXTypes[] = { DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_DOWN, ORTHO_UP, ORTHO_LEFT, ORTHO_RIGHT };
	static constexpr Type mirrorYTypes[] = { DO_NOT_CARE, EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_UP, ORTHO_DOWN, ORTHO_RIGHT, ORTHO_LEFT };
};

struct Glyph {
	wxString chr;

	unsigned int index : 16;
	unsigned int rotated : 16;
	unsigned int mirroredX : 16;
	unsigned int mirroredY : 16;

	union {
		struct {
			Connection::Type up : 4;
			Connection::Type down : 4;
			Connection::Type left : 4;
			Connection::Type right : 4;
			bool sameAfterRotate : 1;
			bool sameAfterMirrorX : 1;
			bool sameAfterMirrorY : 1;
			bool connectToMirrorUp : 1;
			bool connectToMirrorDown : 1;
			bool connectToMirrorLeft : 1;
			bool connectToMirrorRight : 1;
		};
		unsigned int flags;
	};

	Glyph(unsigned int index_, unsigned int rotated_, unsigned int mirroredX_, unsigned int mirroredY_, wxString chr_,
		Connection::Type up_, Connection::Type down_, Connection::Type left_, Connection::Type right_) :
		up(up_), down(down_), left(left_), right(right_), chr(chr_),
		index(index_), rotated(rotated_), mirroredX(mirroredX_), mirroredY(mirroredY_),
		sameAfterRotate(index_ == rotated_),
		sameAfterMirrorY(index_ == mirroredY_),
		sameAfterMirrorX(index_ == mirroredX_),
		connectToMirrorUp(up_ == Connection::mirrorXTypes[up_]),
		connectToMirrorDown(down_ == Connection::mirrorXTypes[down_]),
		connectToMirrorLeft(left_ == Connection::mirrorYTypes[left_]),
		connectToMirrorRight(right_ == Connection::mirrorYTypes[right_]) {}
	Glyph(const Glyph& input) :
		chr(input.chr), index(input.index), rotated(input.rotated), mirroredX(input.mirroredX), mirroredY(input.mirroredY), flags(input.flags) {}
	Glyph& operator=(const Glyph& input) {
		chr = input.chr;
		index = input.index;
		mirroredX = input.mirroredX;
		mirroredY = input.mirroredY;
		flags = input.flags;
		return *this;
	}
};

// maybe switch this to an std::array later
//const std::array<Glyph, 190> allGlyphs { { // This is generated in Excel
const std::vector<Glyph> allGlyphs { // This is generated in Excel
	{ 0, 0, 0, 0, wxString::FromUTF8("\x20"), Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY },
	{ 1, 94, 1, 1, wxString::FromUTF8("\x21"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 2, 2, 2, 2, wxString::FromUTF8("\x22"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 3, 32, 3, 3, wxString::FromUTF8("\x23"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 4, 54, 4, 50, wxString::FromUTF8("\x24"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 5, 40, 5, 52, wxString::FromUTF8("\x25"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 6, 43, 10, 53, wxString::FromUTF8("\x26"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 7, 7, 7, 7, wxString::FromUTF8("\x27"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 8, 9, 9, 47, wxString::FromUTF8("\x28"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 9, 48, 8, 48, wxString::FromUTF8("\x29"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 10, 28, 6, 41, wxString::FromUTF8("\x2A"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 11, 93, 63, 93, wxString::FromUTF8("\x2B"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 12, 85, 77, 75, wxString::FromUTF8("\x2C"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 13, 61, 13, 29, wxString::FromUTF8("\x2D"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 14, 25, 15, 76, wxString::FromUTF8("\x2E"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 15, 79, 14, 27, wxString::FromUTF8("\x2F"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 16, 15, 25, 80, wxString::FromUTF8("\x30"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 17, 64, 17, 17, wxString::FromUTF8("\x31"), Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 18, 19, 18, 18, wxString::FromUTF8("\x32"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 19, 18, 19, 19, wxString::FromUTF8("\x33"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 20, 86, 20, 82, wxString::FromUTF8("\x34"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 21, 72, 21, 84, wxString::FromUTF8("\x35"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 22, 78, 22, 89, wxString::FromUTF8("\x36"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 23, 75, 24, 85, wxString::FromUTF8("\x37"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 24, 12, 23, 73, wxString::FromUTF8("\x38"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 25, 27, 16, 79, wxString::FromUTF8("\x39"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 26, 31, 44, 31, wxString::FromUTF8("\x3A"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 27, 80, 76, 15, wxString::FromUTF8("\x3B"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 28, 53, 45, 43, wxString::FromUTF8("\x3C"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 29, 59, 29, 13, wxString::FromUTF8("\x3D"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 30, 44, 31, 44, wxString::FromUTF8("\x3E"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 31, 30, 30, 26, wxString::FromUTF8("\x3F"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 32, 3, 32, 32, wxString::FromUTF8("\x40"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 33, 55, 36, 33, wxString::FromUTF8("\x41"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 34, 62, 46, 34, wxString::FromUTF8("\x42"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 35, 58, 58, 37, wxString::FromUTF8("\x43"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 36, 56, 33, 36, wxString::FromUTF8("\x44"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 37, 35, 49, 35, wxString::FromUTF8("\x45"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 38, 4, 54, 38, wxString::FromUTF8("\x46"), Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 39, 5, 40, 39, wxString::FromUTF8("\x47"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 40, 52, 39, 40, wxString::FromUTF8("\x48"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 41, 45, 53, 10, wxString::FromUTF8("\x49"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 42, 10, 43, 45, wxString::FromUTF8("\x4A"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 43, 41, 42, 28, wxString::FromUTF8("\x4B"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 44, 26, 26, 30, wxString::FromUTF8("\x4C"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 45, 6, 28, 42, wxString::FromUTF8("\x4D"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 46, 57, 34, 46, wxString::FromUTF8("\x4E"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 47, 8, 48, 8, wxString::FromUTF8("\x4F"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 48, 47, 47, 9, wxString::FromUTF8("\x50"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 49, 37, 37, 58, wxString::FromUTF8("\x51"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 50, 38, 50, 4, wxString::FromUTF8("\x52"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY },
	{ 51, 51, 51, 51, wxString::FromUTF8("\x53"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 52, 39, 52, 5, wxString::FromUTF8("\x54"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 53, 42, 41, 6, wxString::FromUTF8("\x55"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 54, 50, 38, 54, wxString::FromUTF8("\x56"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 55, 36, 55, 56, wxString::FromUTF8("\x57"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 56, 33, 56, 55, wxString::FromUTF8("\x58"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 57, 34, 57, 62, wxString::FromUTF8("\x59"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 58, 49, 35, 49, wxString::FromUTF8("\x5A"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 59, 13, 61, 59, wxString::FromUTF8("\x5B"), Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 60, 92, 60, 60, wxString::FromUTF8("\x5C"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 61, 29, 59, 61, wxString::FromUTF8("\x5D"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 62, 46, 62, 57, wxString::FromUTF8("\x5E"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 63, 11, 11, 91, wxString::FromUTF8("\x5F"), Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 64, 17, 64, 64, wxString::FromUTF8("\x60"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 65, 87, 68, 65, wxString::FromUTF8("\x61"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 66, 22, 78, 66, wxString::FromUTF8("\x62"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 67, 90, 90, 69, wxString::FromUTF8("\x63"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 68, 88, 65, 68, wxString::FromUTF8("\x64"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 69, 67, 81, 67, wxString::FromUTF8("\x65"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 70, 20, 86, 70, wxString::FromUTF8("\x66"), Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 71, 21, 72, 71, wxString::FromUTF8("\x67"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 72, 84, 71, 72, wxString::FromUTF8("\x68"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 73, 77, 85, 24, wxString::FromUTF8("\x69"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 74, 24, 75, 77, wxString::FromUTF8("\x6A"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 75, 73, 74, 12, wxString::FromUTF8("\x6B"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 76, 16, 27, 14, wxString::FromUTF8("\x6C"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 77, 23, 12, 74, wxString::FromUTF8("\x6D"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 78, 89, 66, 78, wxString::FromUTF8("\x6E"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 79, 76, 80, 25, wxString::FromUTF8("\x6F"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 80, 14, 79, 16, wxString::FromUTF8("\x70"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 81, 69, 69, 90, wxString::FromUTF8("\x71"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 82, 70, 82, 20, wxString::FromUTF8("\x72"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY },
	{ 83, 83, 83, 83, wxString::FromUTF8("\x73"), Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 84, 71, 84, 21, wxString::FromUTF8("\x74"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 85, 74, 73, 23, wxString::FromUTF8("\x75"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 86, 82, 70, 86, wxString::FromUTF8("\x76"), Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 87, 68, 87, 88, wxString::FromUTF8("\x77"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 88, 65, 88, 87, wxString::FromUTF8("\x78"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 89, 66, 89, 22, wxString::FromUTF8("\x79"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH },
	{ 90, 81, 67, 81, wxString::FromUTF8("\x7A"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 91, 63, 93, 63, wxString::FromUTF8("\x7B"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH },
	{ 92, 60, 92, 92, wxString::FromUTF8("\x7C"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 93, 91, 91, 11, wxString::FromUTF8("\x7D"), Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::DIAG_BOTH },
	{ 94, 1, 94, 94, wxString::FromUTF8("\x7E"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 95, 98, 96, 95, wxString::FromUTF8("\xC2\xA1"), Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY },
	{ 96, 97, 95, 96, wxString::FromUTF8("\xC2\xA2"), Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY },
	{ 97, 95, 97, 98, wxString::FromUTF8("\xC2\xA3"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP },
	{ 98, 96, 98, 97, wxString::FromUTF8("\xC2\xA4"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN },
	{ 99, 100, 100, 102, wxString::FromUTF8("\xC2\xA5"), Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_BOTH },
	{ 100, 101, 99, 101, wxString::FromUTF8("\xC2\xA6"), Connection::DIAG_BACK, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::DIAG_FRONT },
	{ 101, 102, 102, 100, wxString::FromUTF8("\xC2\xA7"), Connection::DIAG_BOTH, Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::DIAG_BACK },
	{ 102, 99, 101, 99, wxString::FromUTF8("\xC2\xA8"), Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::DIAG_BOTH },
	{ 103, 104, 103, 105, wxString::FromUTF8("\xC2\xA9"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BACK, Connection::DIAG_FRONT },
	{ 104, 105, 106, 104, wxString::FromUTF8("\xC2\xAA"), Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 105, 106, 105, 103, wxString::FromUTF8("\xC2\xAB"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK },
	{ 106, 103, 104, 106, wxString::FromUTF8("\xC2\xAC"), Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 107, 108, 110, 108, wxString::FromUTF8("\xC2\xAE"), Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY },
	{ 108, 109, 109, 107, wxString::FromUTF8("\xC2\xAF"), Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_FRONT, Connection::EMPTY },
	{ 109, 110, 108, 110, wxString::FromUTF8("\xC2\xB0"), Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK },
	{ 110, 107, 107, 109, wxString::FromUTF8("\xC2\xB1"), Connection::EMPTY, Connection::DIAG_BACK, Connection::EMPTY, Connection::DIAG_FRONT },
	{ 111, 112, 111, 113, wxString::FromUTF8("\xC2\xB2"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN },
	{ 112, 113, 114, 112, wxString::FromUTF8("\xC2\xB3"), Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 113, 114, 113, 111, wxString::FromUTF8("\xC2\xB4"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_UP, Connection::ORTHO_UP },
	{ 114, 111, 112, 114, wxString::FromUTF8("\xC2\xB5"), Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 115, 116, 118, 116, wxString::FromUTF8("\xC2\xB6"), Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN },
	{ 116, 117, 117, 115, wxString::FromUTF8("\xC2\xB8"), Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_UP, Connection::ORTHO_UP },
	{ 117, 118, 116, 118, wxString::FromUTF8("\xC2\xB9"), Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_UP },
	{ 118, 115, 115, 117, wxString::FromUTF8("\xC2\xBA"), Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN },
	{ 119, 122, 120, 119, wxString::FromUTF8("\xC2\xBB"), Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 120, 121, 119, 120, wxString::FromUTF8("\xC2\xBC"), Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 121, 119, 121, 122, wxString::FromUTF8("\xC2\xBD"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP },
	{ 122, 120, 122, 121, wxString::FromUTF8("\xC2\xBE"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN },
	{ 123, 126, 124, 123, wxString::FromUTF8("\xC2\xBF"), Connection::ORTHO_RIGHT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 124, 125, 123, 124, wxString::FromUTF8("\xC3\x80"), Connection::ORTHO_LEFT, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 125, 123, 125, 126, wxString::FromUTF8("\xC3\x81"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_UP },
	{ 126, 124, 126, 125, wxString::FromUTF8("\xC3\x82"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_DOWN },
	{ 127, 128, 130, 128, wxString::FromUTF8("\xC3\x83"), Connection::EMPTY, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::EMPTY },
	{ 128, 129, 129, 127, wxString::FromUTF8("\xC3\x84"), Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY },
	{ 129, 130, 128, 130, wxString::FromUTF8("\xC3\x85"), Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP },
	{ 130, 127, 127, 129, wxString::FromUTF8("\xC3\x86"), Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN },
	{ 131, 134, 131, 132, wxString::FromUTF8("\xC3\x87"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY },
	{ 132, 133, 132, 131, wxString::FromUTF8("\xC3\x88"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 133, 131, 134, 133, wxString::FromUTF8("\xC3\x89"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 134, 132, 133, 134, wxString::FromUTF8("\xC3\x8A"), Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 135, 136, 136, 137, wxString::FromUTF8("\xC3\x8B"), Connection::EMPTY, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP },
	{ 136, 138, 135, 138, wxString::FromUTF8("\xC3\x8C"), Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::EMPTY },
	{ 137, 135, 138, 135, wxString::FromUTF8("\xC3\x8D"), Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN },
	{ 138, 137, 137, 136, wxString::FromUTF8("\xC3\x8E"), Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::EMPTY },
	{ 139, 140, 140, 141, wxString::FromUTF8("\xC3\x8F"), Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::ORTHO_DOWN },
	{ 140, 142, 139, 142, wxString::FromUTF8("\xC3\x90"), Connection::EMPTY, Connection::ORTHO_LEFT, Connection::ORTHO_DOWN, Connection::EMPTY },
	{ 141, 139, 142, 139, wxString::FromUTF8("\xC3\x91"), Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP },
	{ 142, 141, 141, 140, wxString::FromUTF8("\xC3\x92"), Connection::ORTHO_LEFT, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY },
	{ 143, 145, 144, 144, wxString::FromUTF8("\xC3\x93"), Connection::ORTHO_LEFT, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY },
	{ 144, 146, 143, 143, wxString::FromUTF8("\xC3\x94"), Connection::ORTHO_RIGHT, Connection::ORTHO_LEFT, Connection::EMPTY, Connection::EMPTY },
	{ 145, 143, 146, 146, wxString::FromUTF8("\xC3\x95"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_DOWN, Connection::ORTHO_UP },
	{ 146, 144, 145, 145, wxString::FromUTF8("\xC3\x96"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP, Connection::ORTHO_DOWN },
	{ 147, 148, 147, 149, wxString::FromUTF8("\xC3\x97"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 148, 149, 150, 148, wxString::FromUTF8("\xC3\x98"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 149, 150, 149, 147, wxString::FromUTF8("\xC3\x99"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 150, 147, 148, 150, wxString::FromUTF8("\xC3\x9A"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 151, 152, 151, 153, wxString::FromUTF8("\xC3\x9B"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 152, 153, 154, 152, wxString::FromUTF8("\xC3\x9C"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 153, 154, 153, 151, wxString::FromUTF8("\xC3\x9D"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 154, 151, 152, 154, wxString::FromUTF8("\xC3\x9E"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 155, 155, 155, 155, wxString::FromUTF8("\xC3\x9F"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 156, 157, 157, 159, wxString::FromUTF8("\xC3\xA0"), Connection::ORTHO_RIGHT, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN, Connection::ORTHO_BOTH },
	{ 157, 158, 156, 158, wxString::FromUTF8("\xC3\xA1"), Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::ORTHO_DOWN },
	{ 158, 159, 159, 157, wxString::FromUTF8("\xC3\xA2"), Connection::ORTHO_BOTH, Connection::ORTHO_LEFT, Connection::ORTHO_BOTH, Connection::ORTHO_UP },
	{ 159, 156, 158, 156, wxString::FromUTF8("\xC3\xA3"), Connection::ORTHO_BOTH, Connection::ORTHO_RIGHT, Connection::ORTHO_UP, Connection::ORTHO_BOTH },
	{ 160, 163, 160, 161, wxString::FromUTF8("\xC3\xB0"), Connection::EMPTY, Connection::EMPTY, Connection::DIAG_FRONT, Connection::DIAG_BACK },
	{ 161, 162, 161, 160, wxString::FromUTF8("\xC3\xB1"), Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK, Connection::DIAG_FRONT },
	{ 162, 160, 163, 162, wxString::FromUTF8("\xC3\xB2"), Connection::DIAG_BACK, Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY },
	{ 163, 161, 162, 163, wxString::FromUTF8("\xC3\xB3"), Connection::DIAG_FRONT, Connection::DIAG_BACK, Connection::EMPTY, Connection::EMPTY },
	{ 164, 169, 165, 166, wxString::FromUTF8("\xC3\xB4"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 165, 171, 164, 167, wxString::FromUTF8("\xC3\xB5"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 166, 168, 167, 164, wxString::FromUTF8("\xC3\xB6"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH },
	{ 167, 170, 166, 165, wxString::FromUTF8("\xC3\xB7"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY },
	{ 168, 165, 169, 170, wxString::FromUTF8("\xC3\xB8"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 169, 167, 168, 171, wxString::FromUTF8("\xC3\xB9"), Connection::EMPTY, Connection::ORTHO_BOTH, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 170, 164, 171, 168, wxString::FromUTF8("\xC3\xBB"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 171, 166, 170, 169, wxString::FromUTF8("\xC3\xBC"), Connection::ORTHO_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 172, 173, 176, 178, wxString::FromUTF8("\xC4\xA3"), Connection::EMPTY, Connection::DIAG_FRONT, Connection::ORTHO_DOWN, Connection::EMPTY },
	{ 173, 174, 179, 177, wxString::FromUTF8("\xC4\xA4"), Connection::ORTHO_LEFT, Connection::EMPTY, Connection::DIAG_FRONT, Connection::EMPTY },
	{ 174, 175, 178, 176, wxString::FromUTF8("\xC4\xA5"), Connection::DIAG_FRONT, Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_UP },
	{ 175, 172, 177, 179, wxString::FromUTF8("\xC4\xA6"), Connection::EMPTY, Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::DIAG_FRONT },
	{ 176, 177, 172, 174, wxString::FromUTF8("\xC4\xA7"), Connection::EMPTY, Connection::DIAG_BACK, Connection::EMPTY, Connection::ORTHO_DOWN },
	{ 177, 178, 175, 173, wxString::FromUTF8("\xC4\xA8"), Connection::EMPTY, Connection::ORTHO_LEFT, Connection::DIAG_BACK, Connection::EMPTY },
	{ 178, 179, 174, 172, wxString::FromUTF8("\xC4\xA9"), Connection::DIAG_BACK, Connection::EMPTY, Connection::ORTHO_UP, Connection::EMPTY },
	{ 179, 176, 173, 175, wxString::FromUTF8("\xC4\xAA"), Connection::ORTHO_RIGHT, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BACK },
	{ 180, 183, 180, 180, wxString::FromUTF8("\xC5\x92"), Connection::EMPTY, Connection::EMPTY, Connection::ORTHO_BOTH, Connection::ORTHO_BOTH },
	{ 181, 182, 189, 181, wxString::FromUTF8("\xC5\xA0"), Connection::EMPTY, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 182, 189, 182, 188, wxString::FromUTF8("\xCB\x86"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 183, 180, 183, 183, wxString::FromUTF8("\xE2\x80\x9A"), Connection::ORTHO_BOTH, Connection::ORTHO_BOTH, Connection::EMPTY, Connection::EMPTY },
	{ 184, 187, 187, 185, wxString::FromUTF8("\xE2\x80\x9E"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 185, 184, 186, 184, wxString::FromUTF8("\xE2\x80\xA0"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH },
	{ 186, 185, 185, 187, wxString::FromUTF8("\xE2\x80\xA1"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 187, 186, 184, 186, wxString::FromUTF8("\xE2\x80\xA6"), Connection::EMPTY, Connection::DIAG_BOTH, Connection::DIAG_BOTH, Connection::EMPTY },
	{ 188, 181, 188, 182, wxString::FromUTF8("\xE2\x80\xB0"), Connection::DIAG_BOTH, Connection::EMPTY, Connection::EMPTY, Connection::EMPTY },
	{ 189, 188, 181, 189, wxString::FromUTF8("\xE2\x80\xB9"), Connection::EMPTY, Connection::EMPTY, Connection::DIAG_BOTH, Connection::EMPTY }
//} 
};

/*
const std::vector<Glyph> allGlyphs{
	{ 0, 0, 0, 0, wxString::FromUTF8("\x20"), EMPTY, EMPTY, EMPTY, EMPTY },
	{ 1, 94, 1, 1, wxString::FromUTF8("\x21"), EMPTY, EMPTY, ORTHO_BOTH, ORTHO_BOTH },
	{ 2, 2, 2, 2, wxString::FromUTF8("\x22"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 3, 32, 3, 3, wxString::FromUTF8("\x23"), EMPTY, EMPTY, ORTHO_BOTH, ORTHO_BOTH },
	{ 4, 54, 4, 50, wxString::FromUTF8("\x24"), EMPTY, ORTHO_BOTH, EMPTY, EMPTY },
	{ 5, 40, 5, 52, wxString::FromUTF8("\x25"), EMPTY, DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 6, 43, 10, 53, wxString::FromUTF8("\x26"), DIAG_BOTH, ORTHO_BOTH, EMPTY, ORTHO_BOTH },
	{ 7, 7, 7, 7, wxString::FromUTF8("\x27"), DIAG_BOTH, DIAG_BOTH, DIAG_BOTH, DIAG_BOTH },
	{ 8, 9, 9, 47, wxString::FromUTF8("\x28"), EMPTY, ORTHO_BOTH, EMPTY, ORTHO_BOTH },
	{ 9, 48, 8, 48, wxString::FromUTF8("\x29"), EMPTY, ORTHO_BOTH, ORTHO_BOTH, EMPTY },
	{ 10, 28, 6, 41, wxString::FromUTF8("\x2A"), DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH, EMPTY },
	{ 11, 93, 63, 93, wxString::FromUTF8("\x2B"), DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH },
	{ 12, 85, 77, 75, wxString::FromUTF8("\x2C"), DIAG_BOTH, EMPTY, ORTHO_BOTH, DIAG_BOTH },
	{ 13, 61, 13, 29, wxString::FromUTF8("\x2D"), DIAG_BOTH, ORTHO_BOTH, EMPTY, EMPTY },
	{ 14, 25, 15, 76, wxString::FromUTF8("\x2E"), DIAG_BOTH, EMPTY, EMPTY, ORTHO_BOTH },
	{ 15, 79, 14, 27, wxString::FromUTF8("\x2F"), DIAG_BOTH, EMPTY, ORTHO_BOTH, EMPTY },
	{ 16, 15, 25, 80, wxString::FromUTF8("\x30"), EMPTY, ORTHO_BOTH, DIAG_BOTH, EMPTY },
	{ 17, 64, 17, 17, wxString::FromUTF8("\x31"), EMPTY, EMPTY, DIAG_BOTH, DIAG_BOTH },
	{ 18, 19, 18, 18, wxString::FromUTF8("\x32"), ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH },
	{ 19, 18, 19, 19, wxString::FromUTF8("\x33"), DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 20, 86, 20, 82, wxString::FromUTF8("\x34"), EMPTY, DIAG_BOTH, EMPTY, EMPTY },
	{ 21, 72, 21, 84, wxString::FromUTF8("\x35"), DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH },
	{ 22, 78, 22, 89, wxString::FromUTF8("\x36"), EMPTY, ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH },
	{ 23, 75, 24, 85, wxString::FromUTF8("\x37"), DIAG_BOTH, ORTHO_BOTH, EMPTY, DIAG_BOTH },
	{ 24, 12, 23, 73, wxString::FromUTF8("\x38"), DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH, EMPTY },
	{ 25, 27, 16, 79, wxString::FromUTF8("\x39"), EMPTY, ORTHO_BOTH, EMPTY, DIAG_BOTH },
	{ 26, 31, 44, 31, wxString::FromUTF8("\x3A"), EMPTY, ORTHO_BOTH, ORTHO_BOTH, EMPTY },
	{ 27, 80, 76, 15, wxString::FromUTF8("\x3B"), EMPTY, DIAG_BOTH, ORTHO_BOTH, EMPTY },
	{ 28, 53, 45, 43, wxString::FromUTF8("\x3C"), ORTHO_BOTH, EMPTY, ORTHO_BOTH, DIAG_BOTH },
	{ 29, 59, 29, 13, wxString::FromUTF8("\x3D"), ORTHO_BOTH, DIAG_BOTH, EMPTY, EMPTY },
	{ 30, 44, 31, 44, wxString::FromUTF8("\x3E"), ORTHO_BOTH, EMPTY, EMPTY, ORTHO_BOTH },
	{ 31, 30, 30, 26, wxString::FromUTF8("\x3F"), ORTHO_BOTH, EMPTY, ORTHO_BOTH, EMPTY },
	{ 32, 3, 32, 32, wxString::FromUTF8("\x40"), ORTHO_BOTH, ORTHO_BOTH, EMPTY, EMPTY },
	{ 33, 55, 36, 33, wxString::FromUTF8("\x41"), ORTHO_BOTH, ORTHO_BOTH, EMPTY, ORTHO_BOTH },
	{ 34, 62, 46, 34, wxString::FromUTF8("\x42"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH },
	{ 35, 58, 58, 37, wxString::FromUTF8("\x43"), ORTHO_BOTH, EMPTY, ORTHO_BOTH, EMPTY },
	{ 36, 56, 33, 36, wxString::FromUTF8("\x44"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, EMPTY },
	{ 37, 35, 49, 35, wxString::FromUTF8("\x45"), EMPTY, ORTHO_BOTH, ORTHO_BOTH, EMPTY },
	{ 38, 4, 54, 38, wxString::FromUTF8("\x46"), EMPTY, EMPTY, EMPTY, ORTHO_BOTH },
	{ 39, 5, 40, 39, wxString::FromUTF8("\x47"), ORTHO_BOTH, ORTHO_BOTH, EMPTY, DIAG_BOTH },
	{ 40, 52, 39, 40, wxString::FromUTF8("\x48"), ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH, EMPTY },
	{ 41, 45, 53, 10, wxString::FromUTF8("\x49"), ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH, EMPTY },
	{ 42, 10, 43, 45, wxString::FromUTF8("\x4A"), EMPTY, ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH },
	{ 43, 41, 42, 28, wxString::FromUTF8("\x4B"), EMPTY, ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH },
	{ 44, 26, 26, 30, wxString::FromUTF8("\x4C"), EMPTY, ORTHO_BOTH, EMPTY, ORTHO_BOTH },
	{ 45, 6, 28, 42, wxString::FromUTF8("\x4D"), ORTHO_BOTH, EMPTY, DIAG_BOTH, ORTHO_BOTH },
	{ 46, 57, 34, 46, wxString::FromUTF8("\x4E"), ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH },
	{ 47, 8, 48, 8, wxString::FromUTF8("\x4F"), ORTHO_BOTH, EMPTY, EMPTY, ORTHO_BOTH },
	{ 48, 47, 47, 9, wxString::FromUTF8("\x50"), ORTHO_BOTH, EMPTY, ORTHO_BOTH, EMPTY },
	{ 49, 37, 37, 58, wxString::FromUTF8("\x51"), EMPTY, ORTHO_BOTH, EMPTY, ORTHO_BOTH },
	{ 50, 38, 50, 4, wxString::FromUTF8("\x52"), ORTHO_BOTH, EMPTY, EMPTY, EMPTY },
	{ 51, 51, 51, 51, wxString::FromUTF8("\x53"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 52, 39, 52, 5, wxString::FromUTF8("\x54"), DIAG_BOTH, EMPTY, ORTHO_BOTH, ORTHO_BOTH },
	{ 53, 42, 41, 6, wxString::FromUTF8("\x55"), ORTHO_BOTH, DIAG_BOTH, EMPTY, ORTHO_BOTH },
	{ 54, 50, 38, 54, wxString::FromUTF8("\x56"), EMPTY, EMPTY, ORTHO_BOTH, EMPTY },
	{ 55, 36, 55, 56, wxString::FromUTF8("\x57"), EMPTY, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 56, 33, 56, 55, wxString::FromUTF8("\x58"), ORTHO_BOTH, EMPTY, ORTHO_BOTH, ORTHO_BOTH },
	{ 57, 34, 57, 62, wxString::FromUTF8("\x59"), DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 58, 49, 35, 49, wxString::FromUTF8("\x5A"), ORTHO_BOTH, EMPTY, EMPTY, ORTHO_BOTH },
	{ 59, 13, 61, 59, wxString::FromUTF8("\x5B"), EMPTY, EMPTY, DIAG_BOTH, ORTHO_BOTH },
	{ 60, 92, 60, 60, wxString::FromUTF8("\x5C"), ORTHO_BOTH, ORTHO_BOTH, EMPTY, EMPTY },
	{ 61, 29, 59, 61, wxString::FromUTF8("\x5D"), EMPTY, EMPTY, ORTHO_BOTH, DIAG_BOTH },
	{ 62, 46, 62, 57, wxString::FromUTF8("\x5E"), ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 63, 11, 11, 91, wxString::FromUTF8("\x5F"), DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH },
	{ 64, 17, 64, 64, wxString::FromUTF8("\x60"), DIAG_BOTH, DIAG_BOTH, EMPTY, EMPTY },
	{ 65, 87, 68, 65, wxString::FromUTF8("\x61"), DIAG_BOTH, DIAG_BOTH, EMPTY, DIAG_BOTH },
	{ 66, 22, 78, 66, wxString::FromUTF8("\x62"), DIAG_BOTH, DIAG_BOTH, EMPTY, ORTHO_BOTH },
	{ 67, 90, 90, 69, wxString::FromUTF8("\x63"), DIAG_BOTH, EMPTY, DIAG_BOTH, EMPTY },
	{ 68, 88, 65, 68, wxString::FromUTF8("\x64"), DIAG_BOTH, DIAG_BOTH, DIAG_BOTH, EMPTY },
	{ 69, 67, 81, 67, wxString::FromUTF8("\x65"), EMPTY, DIAG_BOTH, DIAG_BOTH, EMPTY },
	{ 70, 20, 86, 70, wxString::FromUTF8("\x66"), EMPTY, EMPTY, EMPTY, DIAG_BOTH },
	{ 71, 21, 72, 71, wxString::FromUTF8("\x67"), DIAG_BOTH, DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH },
	{ 72, 84, 71, 72, wxString::FromUTF8("\x68"), DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH },
	{ 73, 77, 85, 24, wxString::FromUTF8("\x69"), ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH, EMPTY },
	{ 74, 24, 75, 77, wxString::FromUTF8("\x6A"), EMPTY, DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH },
	{ 75, 73, 74, 12, wxString::FromUTF8("\x6B"), EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH },
	{ 76, 16, 27, 14, wxString::FromUTF8("\x6C"), EMPTY, DIAG_BOTH, EMPTY, ORTHO_BOTH },
	{ 77, 23, 12, 74, wxString::FromUTF8("\x6D"), DIAG_BOTH, EMPTY, DIAG_BOTH, ORTHO_BOTH },
	{ 78, 89, 66, 78, wxString::FromUTF8("\x6E"), DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH, EMPTY },
	{ 79, 76, 80, 25, wxString::FromUTF8("\x6F"), ORTHO_BOTH, EMPTY, EMPTY, DIAG_BOTH },
	{ 80, 14, 79, 16, wxString::FromUTF8("\x70"), ORTHO_BOTH, EMPTY, DIAG_BOTH, EMPTY },
	{ 81, 69, 69, 90, wxString::FromUTF8("\x71"), EMPTY, DIAG_BOTH, EMPTY, DIAG_BOTH },
	{ 82, 70, 82, 20, wxString::FromUTF8("\x72"), DIAG_BOTH, EMPTY, EMPTY, EMPTY },
	{ 83, 83, 83, 83, wxString::FromUTF8("\x73"), DIAG_BOTH, DIAG_BOTH, DIAG_BOTH, DIAG_BOTH },
	{ 84, 71, 84, 21, wxString::FromUTF8("\x74"), ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH, DIAG_BOTH },
	{ 85, 74, 73, 23, wxString::FromUTF8("\x75"), ORTHO_BOTH, DIAG_BOTH, EMPTY, DIAG_BOTH },
	{ 86, 82, 70, 86, wxString::FromUTF8("\x76"), EMPTY, EMPTY, DIAG_BOTH, EMPTY },
	{ 87, 68, 87, 88, wxString::FromUTF8("\x77"), EMPTY, DIAG_BOTH, DIAG_BOTH, DIAG_BOTH },
	{ 88, 65, 88, 87, wxString::FromUTF8("\x78"), DIAG_BOTH, EMPTY, DIAG_BOTH, DIAG_BOTH },
	{ 89, 66, 89, 22, wxString::FromUTF8("\x79"), ORTHO_BOTH, EMPTY, DIAG_BOTH, DIAG_BOTH },
	{ 90, 81, 67, 81, wxString::FromUTF8("\x7A"), DIAG_BOTH, EMPTY, EMPTY, DIAG_BOTH },
	{ 91, 63, 93, 63, wxString::FromUTF8("\x7B"), ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH },
	{ 92, 60, 92, 92, wxString::FromUTF8("\x7C"), EMPTY, EMPTY, ORTHO_BOTH, ORTHO_BOTH },
	{ 93, 91, 91, 11, wxString::FromUTF8("\x7D"), ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH },
	{ 94, 1, 94, 94, wxString::FromUTF8("\x7E"), ORTHO_BOTH, ORTHO_BOTH, EMPTY, EMPTY },
	{ 95, 98, 96, 95, wxString::FromUTF8("\xC2\xA1"), ORTHO_RIGHT, ORTHO_RIGHT, EMPTY, EMPTY },
	{ 96, 97, 95, 96, wxString::FromUTF8("\xC2\xA2"), ORTHO_LEFT, ORTHO_LEFT, EMPTY, EMPTY },
	{ 97, 95, 97, 98, wxString::FromUTF8("\xC2\xA3"), EMPTY, EMPTY, ORTHO_UP, ORTHO_UP },
	{ 98, 96, 98, 97, wxString::FromUTF8("\xC2\xA4"), EMPTY, EMPTY, ORTHO_DOWN, ORTHO_DOWN },
	{ 99, 100, 100, 102, wxString::FromUTF8("\xC2\xA5"), DIAG_FRONT, DIAG_BOTH, DIAG_BACK, DIAG_BOTH },
	{ 100, 101, 99, 101, wxString::FromUTF8("\xC2\xA6"), DIAG_BACK, DIAG_BOTH, DIAG_BOTH, DIAG_FRONT },
	{ 101, 102, 102, 100, wxString::FromUTF8("\xC2\xA7"), DIAG_BOTH, DIAG_FRONT, DIAG_BOTH, DIAG_BACK },
	{ 102, 99, 101, 99, wxString::FromUTF8("\xC2\xA8"), DIAG_BOTH, DIAG_BACK, DIAG_FRONT, DIAG_BOTH },
	{ 103, 104, 103, 105, wxString::FromUTF8("\xC2\xA9"), EMPTY, DIAG_BOTH, DIAG_BACK, DIAG_FRONT },
	{ 104, 105, 106, 104, wxString::FromUTF8("\xC2\xAA"), DIAG_BACK, DIAG_FRONT, DIAG_BOTH, EMPTY },
	{ 105, 106, 105, 103, wxString::FromUTF8("\xC2\xAB"), DIAG_BOTH, EMPTY, DIAG_FRONT, DIAG_BACK },
	{ 106, 103, 104, 106, wxString::FromUTF8("\xC2\xAC"), DIAG_FRONT, DIAG_BACK, EMPTY, DIAG_BOTH },
	{ 107, 108, 110, 108, wxString::FromUTF8("\xC2\xAE"), EMPTY, DIAG_FRONT, DIAG_BACK, EMPTY },
	{ 108, 109, 109, 107, wxString::FromUTF8("\xC2\xAF"), DIAG_BACK, EMPTY, DIAG_FRONT, EMPTY },
	{ 109, 110, 108, 110, wxString::FromUTF8("\xC2\xB0"), DIAG_FRONT, EMPTY, EMPTY, DIAG_BACK },
	{ 110, 107, 107, 109, wxString::FromUTF8("\xC2\xB1"), EMPTY, DIAG_BACK, EMPTY, DIAG_FRONT },
	{ 111, 112, 111, 113, wxString::FromUTF8("\xC2\xB2"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_DOWN, ORTHO_DOWN },
	{ 112, 113, 114, 112, wxString::FromUTF8("\xC2\xB3"), ORTHO_LEFT, ORTHO_LEFT, ORTHO_BOTH, ORTHO_BOTH },
	{ 113, 114, 113, 111, wxString::FromUTF8("\xC2\xB4"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_UP, ORTHO_UP },
	{ 114, 111, 112, 114, wxString::FromUTF8("\xC2\xB5"), ORTHO_RIGHT, ORTHO_RIGHT, ORTHO_BOTH, ORTHO_BOTH },
	{ 115, 116, 118, 116, wxString::FromUTF8("\xC2\xB6"), ORTHO_LEFT, ORTHO_LEFT, ORTHO_DOWN, ORTHO_DOWN },
	{ 116, 117, 117, 115, wxString::FromUTF8("\xC2\xB8"), ORTHO_LEFT, ORTHO_LEFT, ORTHO_UP, ORTHO_UP },
	{ 117, 118, 116, 118, wxString::FromUTF8("\xC2\xB9"), ORTHO_RIGHT, ORTHO_RIGHT, ORTHO_UP, ORTHO_UP },
	{ 118, 115, 115, 117, wxString::FromUTF8("\xC2\xBA"), ORTHO_RIGHT, ORTHO_RIGHT, ORTHO_DOWN, ORTHO_DOWN },
	{ 119, 122, 120, 119, wxString::FromUTF8("\xC2\xBB"), ORTHO_RIGHT, ORTHO_RIGHT, EMPTY, DIAG_BOTH },
	{ 120, 121, 119, 120, wxString::FromUTF8("\xC2\xBC"), ORTHO_LEFT, ORTHO_LEFT, DIAG_BOTH, EMPTY },
	{ 121, 119, 121, 122, wxString::FromUTF8("\xC2\xBD"), DIAG_BOTH, EMPTY, ORTHO_UP, ORTHO_UP },
	{ 122, 120, 122, 121, wxString::FromUTF8("\xC2\xBE"), EMPTY, DIAG_BOTH, ORTHO_DOWN, ORTHO_DOWN },
	{ 123, 126, 124, 123, wxString::FromUTF8("\xC2\xBF"), ORTHO_RIGHT, ORTHO_RIGHT, EMPTY, ORTHO_BOTH },
	{ 124, 125, 123, 124, wxString::FromUTF8("\xC3\x80"), ORTHO_LEFT, ORTHO_LEFT, ORTHO_BOTH, EMPTY },
	{ 125, 123, 125, 126, wxString::FromUTF8("\xC3\x81"), ORTHO_BOTH, EMPTY, ORTHO_UP, ORTHO_UP },
	{ 126, 124, 126, 125, wxString::FromUTF8("\xC3\x82"), EMPTY, ORTHO_BOTH, ORTHO_DOWN, ORTHO_DOWN },
	{ 127, 128, 130, 128, wxString::FromUTF8("\xC3\x83"), EMPTY, ORTHO_LEFT, ORTHO_DOWN, EMPTY },
	{ 128, 129, 129, 127, wxString::FromUTF8("\xC3\x84"), ORTHO_LEFT, EMPTY, ORTHO_UP, EMPTY },
	{ 129, 130, 128, 130, wxString::FromUTF8("\xC3\x85"), ORTHO_RIGHT, EMPTY, EMPTY, ORTHO_UP },
	{ 130, 127, 127, 129, wxString::FromUTF8("\xC3\x86"), EMPTY, ORTHO_RIGHT, EMPTY, ORTHO_DOWN },
	{ 131, 134, 131, 132, wxString::FromUTF8("\xC3\x87"), ORTHO_BOTH, EMPTY, EMPTY, EMPTY },
	{ 132, 133, 132, 131, wxString::FromUTF8("\xC3\x88"), EMPTY, ORTHO_BOTH, EMPTY, EMPTY },
	{ 133, 131, 134, 133, wxString::FromUTF8("\xC3\x89"), EMPTY, EMPTY, ORTHO_BOTH, EMPTY },
	{ 134, 132, 133, 134, wxString::FromUTF8("\xC3\x8A"), EMPTY, EMPTY, EMPTY, ORTHO_BOTH },
	{ 135, 136, 136, 137, wxString::FromUTF8("\xC3\x8B"), EMPTY, ORTHO_LEFT, EMPTY, ORTHO_UP },
	{ 136, 138, 135, 138, wxString::FromUTF8("\xC3\x8C"), EMPTY, ORTHO_RIGHT, ORTHO_UP, EMPTY },
	{ 137, 135, 138, 135, wxString::FromUTF8("\xC3\x8D"), ORTHO_LEFT, EMPTY, EMPTY, ORTHO_DOWN },
	{ 138, 137, 137, 136, wxString::FromUTF8("\xC3\x8E"), ORTHO_RIGHT, EMPTY, ORTHO_DOWN, EMPTY },
	{ 139, 140, 140, 141, wxString::FromUTF8("\xC3\x8F"), EMPTY, ORTHO_RIGHT, EMPTY, ORTHO_DOWN },
	{ 140, 142, 139, 142, wxString::FromUTF8("\xC3\x90"), EMPTY, ORTHO_LEFT, ORTHO_DOWN, EMPTY },
	{ 141, 139, 142, 139, wxString::FromUTF8("\xC3\x91"), ORTHO_RIGHT, EMPTY, EMPTY, ORTHO_UP },
	{ 142, 141, 141, 140, wxString::FromUTF8("\xC3\x92"), ORTHO_LEFT, EMPTY, ORTHO_UP, EMPTY },
	{ 143, 145, 144, 144, wxString::FromUTF8("\xC3\x93"), ORTHO_LEFT, ORTHO_RIGHT, EMPTY, EMPTY },
	{ 144, 146, 143, 143, wxString::FromUTF8("\xC3\x94"), ORTHO_RIGHT, ORTHO_LEFT, EMPTY, EMPTY },
	{ 145, 143, 146, 146, wxString::FromUTF8("\xC3\x95"), EMPTY, EMPTY, ORTHO_DOWN, ORTHO_UP },
	{ 146, 144, 145, 145, wxString::FromUTF8("\xC3\x96"), EMPTY, EMPTY, ORTHO_UP, ORTHO_DOWN },
	{ 147, 148, 147, 149, wxString::FromUTF8("\xC3\x97"), EMPTY, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 148, 149, 150, 148, wxString::FromUTF8("\xC3\x98"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, EMPTY },
	{ 149, 150, 149, 147, wxString::FromUTF8("\xC3\x99"), ORTHO_BOTH, EMPTY, ORTHO_BOTH, ORTHO_BOTH },
	{ 150, 147, 148, 150, wxString::FromUTF8("\xC3\x9A"), ORTHO_BOTH, ORTHO_BOTH, EMPTY, ORTHO_BOTH },
	{ 151, 152, 151, 153, wxString::FromUTF8("\xC3\x9B"), EMPTY, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 152, 153, 154, 152, wxString::FromUTF8("\xC3\x9C"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, EMPTY },
	{ 153, 154, 153, 151, wxString::FromUTF8("\xC3\x9D"), ORTHO_BOTH, EMPTY, ORTHO_BOTH, ORTHO_BOTH },
	{ 154, 151, 152, 154, wxString::FromUTF8("\xC3\x9E"), ORTHO_BOTH, ORTHO_BOTH, EMPTY, ORTHO_BOTH },
	{ 155, 155, 155, 155, wxString::FromUTF8("\xC3\x9F"), ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH },
	{ 156, 157, 157, 159, wxString::FromUTF8("\xC3\xA0"), ORTHO_RIGHT, ORTHO_BOTH, ORTHO_DOWN, ORTHO_BOTH },
	{ 157, 158, 156, 158, wxString::FromUTF8("\xC3\xA1"), ORTHO_LEFT, ORTHO_BOTH, ORTHO_BOTH, ORTHO_DOWN },
	{ 158, 159, 159, 157, wxString::FromUTF8("\xC3\xA2"), ORTHO_BOTH, ORTHO_LEFT, ORTHO_BOTH, ORTHO_UP },
	{ 159, 156, 158, 156, wxString::FromUTF8("\xC3\xA3"), ORTHO_BOTH, ORTHO_RIGHT, ORTHO_UP, ORTHO_BOTH },
	{ 160, 163, 160, 161, wxString::FromUTF8("\xC3\xB0"), EMPTY, EMPTY, DIAG_FRONT, DIAG_BACK },
	{ 161, 162, 161, 160, wxString::FromUTF8("\xC3\xB1"), EMPTY, EMPTY, DIAG_BACK, DIAG_FRONT },
	{ 162, 160, 163, 162, wxString::FromUTF8("\xC3\xB2"), DIAG_BACK, DIAG_FRONT, EMPTY, EMPTY },
	{ 163, 161, 162, 163, wxString::FromUTF8("\xC3\xB3"), DIAG_FRONT, DIAG_BACK, EMPTY, EMPTY },
	{ 164, 169, 165, 166, wxString::FromUTF8("\xC3\xB4"), EMPTY, DIAG_BOTH, EMPTY, ORTHO_BOTH },
	{ 165, 171, 164, 167, wxString::FromUTF8("\xC3\xB5"), EMPTY, DIAG_BOTH, ORTHO_BOTH, EMPTY },
	{ 166, 168, 167, 164, wxString::FromUTF8("\xC3\xB6"), DIAG_BOTH, EMPTY, EMPTY, ORTHO_BOTH },
	{ 167, 170, 166, 165, wxString::FromUTF8("\xC3\xB7"), DIAG_BOTH, EMPTY, ORTHO_BOTH, EMPTY },
	{ 168, 165, 169, 170, wxString::FromUTF8("\xC3\xB8"), EMPTY, ORTHO_BOTH, EMPTY, DIAG_BOTH },
	{ 169, 167, 168, 171, wxString::FromUTF8("\xC3\xB9"), EMPTY, ORTHO_BOTH, DIAG_BOTH, EMPTY },
	{ 170, 164, 171, 168, wxString::FromUTF8("\xC3\xBB"), ORTHO_BOTH, EMPTY, EMPTY, DIAG_BOTH },
	{ 171, 166, 170, 169, wxString::FromUTF8("\xC3\xBC"), ORTHO_BOTH, EMPTY, DIAG_BOTH, EMPTY },
	{ 172, 173, 176, 178, wxString::FromUTF8("\xC4\xA3"), EMPTY, DIAG_FRONT, ORTHO_DOWN, EMPTY },
	{ 173, 174, 179, 177, wxString::FromUTF8("\xC4\xA4"), ORTHO_LEFT, EMPTY, DIAG_FRONT, EMPTY },
	{ 174, 175, 178, 176, wxString::FromUTF8("\xC4\xA5"), DIAG_FRONT, EMPTY, EMPTY, ORTHO_UP },
	{ 175, 172, 177, 179, wxString::FromUTF8("\xC4\xA6"), EMPTY, ORTHO_RIGHT, EMPTY, DIAG_FRONT },
	{ 176, 177, 172, 174, wxString::FromUTF8("\xC4\xA7"), EMPTY, DIAG_BACK, EMPTY, ORTHO_DOWN },
	{ 177, 178, 175, 173, wxString::FromUTF8("\xC4\xA8"), EMPTY, ORTHO_LEFT, DIAG_BACK, EMPTY },
	{ 178, 179, 174, 172, wxString::FromUTF8("\xC4\xA9"), DIAG_BACK, EMPTY, ORTHO_UP, EMPTY },
	{ 179, 176, 173, 175, wxString::FromUTF8("\xC4\xAA"), ORTHO_RIGHT, EMPTY, EMPTY, DIAG_BACK },
	{ 180, 183, 180, 180, wxString::FromUTF8("\xC5\x92"), EMPTY, EMPTY, ORTHO_BOTH, ORTHO_BOTH },
	{ 181, 182, 189, 181, wxString::FromUTF8("\xC5\xA0"), EMPTY, EMPTY, EMPTY, DIAG_BOTH },
	{ 182, 189, 182, 188, wxString::FromUTF8("\xCB\x86"), EMPTY, DIAG_BOTH, EMPTY, EMPTY },
	{ 183, 180, 183, 183, wxString::FromUTF8("\xE2\x80\x9A"), ORTHO_BOTH, ORTHO_BOTH, EMPTY, EMPTY },
	{ 184, 187, 187, 185, wxString::FromUTF8("\xE2\x80\x9E"), EMPTY, DIAG_BOTH, EMPTY, DIAG_BOTH },
	{ 185, 184, 186, 184, wxString::FromUTF8("\xE2\x80\xA0"), DIAG_BOTH, EMPTY, EMPTY, DIAG_BOTH },
	{ 186, 185, 185, 187, wxString::FromUTF8("\xE2\x80\xA1"), DIAG_BOTH, EMPTY, DIAG_BOTH, EMPTY },
	{ 187, 186, 184, 186, wxString::FromUTF8("\xE2\x80\xA6"), EMPTY, DIAG_BOTH, DIAG_BOTH, EMPTY },
	{ 188, 181, 188, 182, wxString::FromUTF8("\xE2\x80\xB0"), DIAG_BOTH, EMPTY, EMPTY, EMPTY },
	{ 189, 188, 181, 189, wxString::FromUTF8("\xE2\x80\xB9"), EMPTY, EMPTY, DIAG_BOTH, EMPTY }
};
//*/

/*
struct GlyphSuperPos {
	std::vector<Glyph> glyphList;
	std::array< std::array<unsigned int, NUM_SIDES>, NUM_TYPES> connectionCount{};
	bool determined = false;
	bool inUse = false;

	//unsigned int i; // location in the i direction
	//unsigned int j; // location in the j direction

	GlyphSuperPos(const std::vector<Glyph>& glyphList_) : glyphList(glyphList_) {
		for (const Glyph& glyph : glyphList) {
			connectionCount[glyph.up][0]++;
			connectionCount[glyph.down][1]++;
			connectionCount[glyph.left][2]++;
			connectionCount[glyph.right][3]++;
		}
	}
	GlyphSuperPos(const GlyphSuperPos& input) :
		glyphList(input.glyphList), connectionCount(input.connectionCount) {}

	GlyphSuperPos(const Glyph& glyph) {
		glyphList.push_back(glyph);
		determined = true;
		connectionCount[glyph.up][0]++;
		connectionCount[glyph.down][1]++;
		connectionCount[glyph.left][2]++;
		connectionCount[glyph.right][3]++;
	}

	void remove(int index) {
		glyphList.erase(glyphList.begin() + index);
	}

	Glyph collapse() {
		/* Choose a random Glyph from the options, and set that as the only Glyph. * /
		Glyph glyph = glyphList[rand() % glyphList.size()]; // Grab a random Glyph
		*this = { glyph };									// Reset the GlyphSuperPosition so it is determined
		return glyph;										// Return the Glyph;
	}

	operator Glyph() const {
		return glyphList[0];
	}
};
//*/



//const GlyphSuperPos allGlyphSuperPos{ allGlyphs };




