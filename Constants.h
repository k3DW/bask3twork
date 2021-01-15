#pragma once
#include "wx/wx.h"
#include <array>

#define intWX(x) wxString::Format(wxT("%i"),x)
#define VERSION(MAJOR, MINOR, PATCH) intWX(MAJOR) + "." + intWX(MINOR) + (PATCH == 0 ? "" : "." + intWX(PATCH))

const int SCREEN_X = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
const int SCREEN_Y = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
const wxSize MAX_SIZE(SCREEN_X, SCREEN_Y);

enum SymmetryType {
	NoSym,			// No symmetry
	VertSym,		// Mirror symmetry across the vertical axis
	HoriSym,		// Mirror symmetry across the horizontal axis
	VertHoriSym,	// Mirror symmetry across both the vertical and horizontal axes
	Rot2Sym,		// 2-way rotational symmetry
	Rot4Sym			// 4-way rotational symmetry
};

extern wxString tempString;
#define msg(x) \
	tempString = ""; \
	tempString << x; \
	wxMessageBox(tempString)
#define msgIter(list) \
	tempString = ""; \
	for (int c = 0; c < list.size(); c++) tempString << " " << list[c]; \
	wxMessageBox(tempString)

/* Begin definitions for `MainWindow` class */ #if true
	#define declareButton(buttonName) \
		void buttonName##Function(wxCommandEvent& evt); \
		wxButton* buttonName##Button
	#define initButton(buttonName, displayText) \
		buttonName##Button = new wxButton(this, wxID_ANY, displayText); \
		buttonName##Button->Bind(wxEVT_BUTTON, &MainWindow::##buttonName##Function, this)

	#define declareGenerateButton(SymType) \
		wxButton* generate##SymType##Button
	#define initGenerateButton(SymType, displayText) \
		generate##SymType##Button = new wxButton(this, SymType, displayText); \
		generate##SymType##Button->Bind(wxEVT_BUTTON, &MainWindow::generateKnot, this); \
		generateRegionSizer->Add(generate##SymType##Button)

	#define GAP_1 20
	#define GAP_2 10
	#define GAP_3 5
#endif /* End definitions for `MainWindow` class */

/* Begin definitions for `Tile` class */ #if true
	const wxColour BACKGROUND_COLOUR(240, 240, 240);
	const wxColour HL_COLOUR(150, 200, 255);
#endif /* End definitions for `Tile` class */

enum ConnectionType { EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_UP, ORTHO_DOWN, ORTHO_LEFT, ORTHO_RIGHT };

/* Begin definitions for `Knot` class */ #if true
	#define MAX_ATTEMPTS 10000
	#define ATTEMPTS_DISPLAY_INCREMENT 100
	const wxString MAX_ATTEMPTS_STR = intWX(MAX_ATTEMPTS);

	enum Side { TOP, BOTTOM, LEFT, RIGHT };
	enum SideBitfield {
		bTOP	= 1 << TOP,
		bBOTTOM	= 1 << BOTTOM,
		bLEFT	= 1 << LEFT,
		bRIGHT	= 1 << RIGHT
	};

	#define NUM_SIDES 4
	#define NUM_TYPES 9
	#define NUM_GLYPHS 190 // total number of glyphs

	#define conv(x) wxString::FromUTF8(x)
	const wxString glyphs[NUM_GLYPHS] = { // The UTF8 symbols that draw the glyphs in the CelticKnots font
		conv("\x20"), conv("\x21"), conv("\x22"), conv("\x23"), conv("\x24"), conv("\x25"), conv("\x26"),  conv("\x27"), conv("\x28"), conv("\x29"), conv("\x2A"), conv("\x2B"), conv("\x2C"), conv("\x2D"), conv("\x2E"), conv("\x2F"), conv("\x30"), conv("\x31"), conv("\x32"), conv("\x33"), conv("\x34"), conv("\x35"), conv("\x36"), conv("\x37"), conv("\x38"), conv("\x39"), conv("\x3A"), conv("\x3B"), conv("\x3C"), conv("\x3D"), conv("\x3E"), conv("\x3F"), conv("\x40"), conv("\x41"), conv("\x42"), conv("\x43"), conv("\x44"), conv("\x45"), conv("\x46"), conv("\x47"), conv("\x48"), conv("\x49"), conv("\x4A"), conv("\x4B"), conv("\x4C"), conv("\x4D"), conv("\x4E"), conv("\x4F"), conv("\x50"), conv("\x51"), conv("\x52"), conv("\x53"), conv("\x54"), conv("\x55"), conv("\x56"), conv("\x57"), conv("\x58"), conv("\x59"), conv("\x5A"), conv("\x5B"), conv("\x5C"), conv("\x5D"), conv("\x5E"), conv("\x5F"), conv("\x60"), conv("\x61"), conv("\x62"), conv("\x63"), conv("\x64"), conv("\x65"), conv("\x66"), conv("\x67"), conv("\x68"), conv("\x69"), conv("\x6A"), conv("\x6B"), conv("\x6C"), conv("\x6D"), conv("\x6E"), conv("\x6F"), conv("\x70"), conv("\x71"), conv("\x72"), conv("\x73"), conv("\x74"), conv("\x75"), conv("\x76"), conv("\x77"), conv("\x78"), conv("\x79"), conv("\x7A"), conv("\x7B"), conv("\x7C"), conv("\x7D"), conv("\x7E"), conv("\xC2\xA1"), conv("\xC2\xA2"), conv("\xC2\xA3"), conv("\xC2\xA4"), conv("\xC2\xA5"), conv("\xC2\xA6"), conv("\xC2\xA7"), conv("\xC2\xA8"), conv("\xC2\xA9"), conv("\xC2\xAA"), conv("\xC2\xAB"), conv("\xC2\xAC"), conv("\xC2\xAE"), conv("\xC2\xAF"), conv("\xC2\xB0"), conv("\xC2\xB1"), conv("\xC2\xB2"), conv("\xC2\xB3"), conv("\xC2\xB4"), conv("\xC2\xB5"), conv("\xC2\xB6"), conv("\xC2\xB8"), conv("\xC2\xB9"), conv("\xC2\xBA"), conv("\xC2\xBB"), conv("\xC2\xBC"), conv("\xC2\xBD"), conv("\xC2\xBE"), conv("\xC2\xBF"), conv("\xC3\x80"), conv("\xC3\x81"), conv("\xC3\x82"), conv("\xC3\x83"), conv("\xC3\x84"), conv("\xC3\x85"), conv("\xC3\x86"), conv("\xC3\x87"), conv("\xC3\x88"), conv("\xC3\x89"), conv("\xC3\x8A"), conv("\xC3\x8B"), conv("\xC3\x8C"), conv("\xC3\x8D"), conv("\xC3\x8E"), conv("\xC3\x8F"), conv("\xC3\x90"), conv("\xC3\x91"), conv("\xC3\x92"), conv("\xC3\x93"), conv("\xC3\x94"), conv("\xC3\x95"), conv("\xC3\x96"), conv("\xC3\x97"), conv("\xC3\x98"), conv("\xC3\x99"), conv("\xC3\x9A"), conv("\xC3\x9B"), conv("\xC3\x9C"), conv("\xC3\x9D"), conv("\xC3\x9E"), conv("\xC3\x9F"), conv("\xC3\xA0"), conv("\xC3\xA1"), conv("\xC3\xA2"), conv("\xC3\xA3"), conv("\xC3\xB0"), conv("\xC3\xB1"), conv("\xC3\xB2"), conv("\xC3\xB3"), conv("\xC3\xB4"), conv("\xC3\xB5"), conv("\xC3\xB6"), conv("\xC3\xB7"), conv("\xC3\xB8"), conv("\xC3\xB9"), conv("\xC3\xBB"), conv("\xC3\xBC"), conv("\xC4\xA3"), conv("\xC4\xA4"), conv("\xC4\xA5"), conv("\xC4\xA6"), conv("\xC4\xA7"), conv("\xC4\xA8"), conv("\xC4\xA9"), conv("\xC4\xAA"), conv("\xC5\x92"), conv("\xC5\xA0"), conv("\xCB\x86"), conv("\xE2\x80\x9A"), conv("\xE2\x80\x9E"), conv("\xE2\x80\xA0"), conv("\xE2\x80\xA1"), conv("\xE2\x80\xA6"), conv("\xE2\x80\xB0"), conv("\xE2\x80\xB9")
	};
	const int   rotateGlyphs[NUM_GLYPHS] = { // Indices such that `glyphs[rotateGlyphs[N]]` rotates `glyphs[N]` 90deg CW
		0, 94, 2, 32, 54, 40, 43, 7, 9, 48, 28, 93, 85, 61, 25, 79, 15, 64, 19, 18, 86, 72, 78, 75, 12, 27, 31, 80, 53, 59, 44, 30, 3, 55, 62, 58, 56, 35, 4, 5, 52, 45, 10, 41, 26, 6, 57, 8, 47, 37, 38, 51, 39, 42, 50, 36, 33, 34, 49, 13, 92, 29, 46, 11, 17, 87, 22, 90, 88, 67, 20, 21, 84, 77, 24, 73, 16, 23, 89, 76, 14, 69, 70, 83, 71, 74, 82, 68, 65, 66, 81, 63, 60, 91, 1, 98, 97, 95, 96, 100, 101, 102, 99, 104, 105, 106, 103, 108, 109, 110, 107, 112, 113, 114, 111, 116, 117, 118, 115, 122, 121, 119, 120, 126, 125, 123, 124, 128, 129, 130, 127, 134, 133, 131, 132, 136, 138, 135, 137, 140, 142, 139, 141, 145, 146, 143, 144, 148, 149, 150, 147, 152, 153, 154, 151, 155, 157, 158, 159, 156, 163, 162, 160, 161, 169, 171, 168, 170, 165, 167, 164, 166, 173, 174, 175, 172, 177, 178, 179, 176, 183, 182, 189, 180, 187, 184, 185, 186, 181, 188
	};
	const int reflectYGlyphs[NUM_GLYPHS] = { // Indices such that `glyphs[reflectYGlyphs[N]]` reflects `glyphs[N]` left to right
		0, 1, 2, 3, 4, 5, 10, 7, 9, 8, 6, 63, 77, 13, 15, 14, 25, 17, 18, 19, 20, 21, 22, 24, 23, 16, 44, 76, 45, 29, 31, 30, 32, 36, 46, 58, 33, 49, 54, 40, 39, 53, 43, 42, 26, 28, 34, 48, 47, 37, 50, 51, 52, 41, 38, 55, 56, 57, 35, 61, 60, 59, 62, 11, 64, 68, 78, 90, 65, 81, 86, 72, 71, 85, 75, 74, 27, 12, 66, 80, 79, 69, 82, 83, 84, 73, 70, 87, 88, 89, 67, 93, 92, 91, 94, 96, 95, 97, 98, 100, 99, 102, 101, 103, 106, 105, 104, 110, 109, 108, 107, 111, 114, 113, 112, 118, 117, 116, 115, 120, 119, 121, 122, 124, 123, 125, 126, 130, 129, 128, 127, 131, 132, 134, 133, 136, 135, 138, 137, 140, 139, 142, 141, 144, 143, 146, 145, 147, 150, 149, 148, 151, 154, 153, 152, 155, 157, 156, 159, 158, 160, 161, 163, 162, 165, 164, 167, 166, 169, 168, 171, 170, 176, 179, 178, 177, 172, 175, 174, 173, 180, 189, 182, 183, 187, 186, 185, 184, 188, 181
	};
	const int reflectXGlyphs[NUM_GLYPHS] = { // Indices such that `glyphs[reflectXGlyphs[N]]` reflects `glyphs[N]` up to down
		0, 1, 2, 3, 50, 52, 53, 7, 47, 48, 41, 93, 75, 29, 76, 27, 80, 17, 18, 19, 82, 84, 89, 85, 73, 79, 31, 15, 43, 13, 44, 26, 32, 33, 34, 37, 36, 35, 38, 39, 40, 10, 45, 28, 30, 42, 46, 8, 9, 58, 4, 51, 5, 6, 54, 56, 55, 62, 49, 59, 60, 61, 57, 91, 64, 65, 66, 69, 68, 67, 70, 71, 72, 24, 77, 12, 14, 74, 78, 25, 16, 90, 20, 83, 21, 23, 86, 88, 87, 22, 81, 63, 92, 11, 94, 95, 96, 98, 97, 102, 101, 100, 99, 105, 104, 103, 106, 108, 107, 110, 109, 113, 112, 111, 114, 116, 115, 118, 117, 119, 120, 122, 121, 123, 124, 126, 125, 128, 127, 130, 129, 132, 131, 133, 134, 137, 138, 135, 136, 141, 142, 139, 140, 144, 143, 146, 145, 149, 148, 147, 150, 153, 152, 151, 154, 155, 159, 158, 157, 156, 161, 160, 162, 163, 166, 167, 164, 165, 170, 171, 168, 169, 178, 177, 176, 179, 174, 173, 172, 175, 180, 181, 188, 183, 185, 184, 187, 186, 182, 189
	};

	/*const ConnectionType   rotateTypes[NUM_TYPES] = { // Each ConnectionType's map after a 90deg CW rotation
		EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_RIGHT, ORTHO_LEFT, ORTHO_UP, ORTHO_DOWN
	};//*/
	const ConnectionType reflectYTypes[NUM_TYPES] = { // Each ConnectionType's map after a left-right reflection
		EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_UP, ORTHO_DOWN, ORTHO_RIGHT, ORTHO_LEFT
	};
	const ConnectionType reflectXTypes[NUM_TYPES] = { // Each ConnectionType's map after an up-down reflection
		EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_DOWN, ORTHO_UP, ORTHO_LEFT, ORTHO_RIGHT
	};

	const ConnectionType connectionForGlyphOnSide[NUM_GLYPHS][NUM_SIDES] = {
		// `connectionForGlyphOnSide[int glyphIndex][Side side]` is the type of connection of `glyphs[glyphIndex]` on Side side
		{EMPTY, EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {EMPTY, EMPTY, ORTHO_BOTH, ORTHO_BOTH}, {EMPTY, ORTHO_BOTH, EMPTY, EMPTY}, {EMPTY, DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {DIAG_BOTH, ORTHO_BOTH, EMPTY, ORTHO_BOTH}, {DIAG_BOTH, DIAG_BOTH, DIAG_BOTH, DIAG_BOTH}, {EMPTY, ORTHO_BOTH, EMPTY, ORTHO_BOTH}, {EMPTY, ORTHO_BOTH, ORTHO_BOTH, EMPTY}, {DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH, EMPTY}, {DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH}, {DIAG_BOTH, EMPTY, ORTHO_BOTH, DIAG_BOTH}, {DIAG_BOTH, ORTHO_BOTH, EMPTY, EMPTY}, {DIAG_BOTH, EMPTY, EMPTY, ORTHO_BOTH}, {DIAG_BOTH, EMPTY, ORTHO_BOTH, EMPTY}, {EMPTY, ORTHO_BOTH, DIAG_BOTH, EMPTY}, {EMPTY, EMPTY, DIAG_BOTH, DIAG_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH}, {DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {EMPTY, DIAG_BOTH, EMPTY, EMPTY}, {DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH}, {EMPTY, ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH}, {DIAG_BOTH, ORTHO_BOTH, EMPTY, DIAG_BOTH}, {DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH, EMPTY}, {EMPTY, ORTHO_BOTH, EMPTY, DIAG_BOTH}, {EMPTY, ORTHO_BOTH, ORTHO_BOTH, EMPTY}, {EMPTY, DIAG_BOTH, ORTHO_BOTH, EMPTY}, {ORTHO_BOTH, EMPTY, ORTHO_BOTH, DIAG_BOTH}, {ORTHO_BOTH, DIAG_BOTH, EMPTY, EMPTY}, {ORTHO_BOTH, EMPTY, EMPTY, ORTHO_BOTH}, {ORTHO_BOTH, EMPTY, ORTHO_BOTH, EMPTY}, {ORTHO_BOTH, ORTHO_BOTH, EMPTY, EMPTY}, {ORTHO_BOTH, ORTHO_BOTH, EMPTY, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH}, {ORTHO_BOTH, EMPTY, ORTHO_BOTH, EMPTY}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, EMPTY}, {EMPTY, ORTHO_BOTH, ORTHO_BOTH, EMPTY}, {EMPTY, EMPTY, EMPTY, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, EMPTY, DIAG_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH, EMPTY}, {ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH, EMPTY}, {EMPTY, ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH}, {EMPTY, ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH}, {EMPTY, ORTHO_BOTH, EMPTY, ORTHO_BOTH}, {ORTHO_BOTH, EMPTY, DIAG_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, EMPTY, EMPTY, ORTHO_BOTH}, {ORTHO_BOTH, EMPTY, ORTHO_BOTH, EMPTY}, {EMPTY, ORTHO_BOTH, EMPTY, ORTHO_BOTH}, {ORTHO_BOTH, EMPTY, EMPTY, EMPTY}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {DIAG_BOTH, EMPTY, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, DIAG_BOTH, EMPTY, ORTHO_BOTH}, {EMPTY, EMPTY, ORTHO_BOTH, EMPTY}, {EMPTY, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, EMPTY, ORTHO_BOTH, ORTHO_BOTH}, {DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, EMPTY, EMPTY, ORTHO_BOTH}, {EMPTY, EMPTY, DIAG_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, EMPTY, EMPTY}, {EMPTY, EMPTY, ORTHO_BOTH, DIAG_BOTH}, {ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH}, {DIAG_BOTH, DIAG_BOTH, EMPTY, EMPTY}, {DIAG_BOTH, DIAG_BOTH, EMPTY, DIAG_BOTH}, {DIAG_BOTH, DIAG_BOTH, EMPTY, ORTHO_BOTH}, {DIAG_BOTH, EMPTY, DIAG_BOTH, EMPTY}, {DIAG_BOTH, DIAG_BOTH, DIAG_BOTH, EMPTY}, {EMPTY, DIAG_BOTH, DIAG_BOTH, EMPTY}, {EMPTY, EMPTY, EMPTY, DIAG_BOTH}, {DIAG_BOTH, DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH}, {DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH}, {ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH, EMPTY}, {EMPTY, DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH}, {EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH}, {EMPTY, DIAG_BOTH, EMPTY, ORTHO_BOTH}, {DIAG_BOTH, EMPTY, DIAG_BOTH, ORTHO_BOTH}, {DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH, EMPTY}, {ORTHO_BOTH, EMPTY, EMPTY, DIAG_BOTH}, {ORTHO_BOTH, EMPTY, DIAG_BOTH, EMPTY}, {EMPTY, DIAG_BOTH, EMPTY, DIAG_BOTH}, {DIAG_BOTH, EMPTY, EMPTY, EMPTY}, {DIAG_BOTH, DIAG_BOTH, DIAG_BOTH, DIAG_BOTH}, {ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH, DIAG_BOTH}, {ORTHO_BOTH, DIAG_BOTH, EMPTY, DIAG_BOTH}, {EMPTY, EMPTY, DIAG_BOTH, EMPTY}, {EMPTY, DIAG_BOTH, DIAG_BOTH, DIAG_BOTH}, {DIAG_BOTH, EMPTY, DIAG_BOTH, DIAG_BOTH}, {ORTHO_BOTH, EMPTY, DIAG_BOTH, DIAG_BOTH}, {DIAG_BOTH, EMPTY, EMPTY, DIAG_BOTH}, {ORTHO_BOTH, DIAG_BOTH, DIAG_BOTH, ORTHO_BOTH}, {EMPTY, EMPTY, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, DIAG_BOTH, ORTHO_BOTH, DIAG_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, EMPTY, EMPTY}, {ORTHO_RIGHT, ORTHO_RIGHT, EMPTY, EMPTY}, {ORTHO_LEFT, ORTHO_LEFT, EMPTY, EMPTY}, {EMPTY, EMPTY, ORTHO_UP, ORTHO_UP}, {EMPTY, EMPTY, ORTHO_DOWN, ORTHO_DOWN}, {DIAG_FRONT, DIAG_BOTH, DIAG_BACK, DIAG_BOTH}, {DIAG_BACK, DIAG_BOTH, DIAG_BOTH, DIAG_FRONT}, {DIAG_BOTH, DIAG_FRONT, DIAG_BOTH, DIAG_BACK}, {DIAG_BOTH, DIAG_BACK, DIAG_FRONT, DIAG_BOTH}, {EMPTY, DIAG_BOTH, DIAG_BACK, DIAG_FRONT}, {DIAG_BACK, DIAG_FRONT, DIAG_BOTH, EMPTY}, {DIAG_BOTH, EMPTY, DIAG_FRONT, DIAG_BACK}, {DIAG_FRONT, DIAG_BACK, EMPTY, DIAG_BOTH}, {EMPTY, DIAG_FRONT, DIAG_BACK, EMPTY}, {DIAG_BACK, EMPTY, DIAG_FRONT, EMPTY}, {DIAG_FRONT, EMPTY, EMPTY, DIAG_BACK}, {EMPTY, DIAG_BACK, EMPTY, DIAG_FRONT}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_DOWN, ORTHO_DOWN}, {ORTHO_LEFT, ORTHO_LEFT, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_UP, ORTHO_UP}, {ORTHO_RIGHT, ORTHO_RIGHT, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_LEFT, ORTHO_LEFT, ORTHO_DOWN, ORTHO_DOWN}, {ORTHO_LEFT, ORTHO_LEFT, ORTHO_UP, ORTHO_UP}, {ORTHO_RIGHT, ORTHO_RIGHT, ORTHO_UP, ORTHO_UP}, {ORTHO_RIGHT, ORTHO_RIGHT, ORTHO_DOWN, ORTHO_DOWN}, {ORTHO_RIGHT, ORTHO_RIGHT, EMPTY, DIAG_BOTH}, {ORTHO_LEFT, ORTHO_LEFT, DIAG_BOTH, EMPTY}, {DIAG_BOTH, EMPTY, ORTHO_UP, ORTHO_UP}, {EMPTY, DIAG_BOTH, ORTHO_DOWN, ORTHO_DOWN}, {ORTHO_RIGHT, ORTHO_RIGHT, EMPTY, ORTHO_BOTH}, {ORTHO_LEFT, ORTHO_LEFT, ORTHO_BOTH, EMPTY}, {ORTHO_BOTH, EMPTY, ORTHO_UP, ORTHO_UP}, {EMPTY, ORTHO_BOTH, ORTHO_DOWN, ORTHO_DOWN}, {EMPTY, ORTHO_LEFT, ORTHO_DOWN, EMPTY}, {ORTHO_LEFT, EMPTY, ORTHO_UP, EMPTY}, {ORTHO_RIGHT, EMPTY, EMPTY, ORTHO_UP}, {EMPTY, ORTHO_RIGHT, EMPTY, ORTHO_DOWN}, {ORTHO_BOTH, EMPTY, EMPTY, EMPTY}, {EMPTY, ORTHO_BOTH, EMPTY, EMPTY}, {EMPTY, EMPTY, ORTHO_BOTH, EMPTY}, {EMPTY, EMPTY, EMPTY, ORTHO_BOTH}, {EMPTY, ORTHO_LEFT, EMPTY, ORTHO_UP}, {EMPTY, ORTHO_RIGHT, ORTHO_UP, EMPTY}, {ORTHO_LEFT, EMPTY, EMPTY, ORTHO_DOWN}, {ORTHO_RIGHT, EMPTY, ORTHO_DOWN, EMPTY}, {EMPTY, ORTHO_RIGHT, EMPTY, ORTHO_DOWN}, {EMPTY, ORTHO_LEFT, ORTHO_DOWN, EMPTY}, {ORTHO_RIGHT, EMPTY, EMPTY, ORTHO_UP}, {ORTHO_LEFT, EMPTY, ORTHO_UP, EMPTY}, {ORTHO_LEFT, ORTHO_RIGHT, EMPTY, EMPTY}, {ORTHO_RIGHT, ORTHO_LEFT, EMPTY, EMPTY}, {EMPTY, EMPTY, ORTHO_DOWN, ORTHO_UP}, {EMPTY, EMPTY, ORTHO_UP, ORTHO_DOWN}, {EMPTY, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, EMPTY}, {ORTHO_BOTH, EMPTY, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, EMPTY, ORTHO_BOTH}, {EMPTY, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, EMPTY}, {ORTHO_BOTH, EMPTY, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, EMPTY, ORTHO_BOTH}, {ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH, ORTHO_BOTH}, {ORTHO_RIGHT, ORTHO_BOTH, ORTHO_DOWN, ORTHO_BOTH}, {ORTHO_LEFT, ORTHO_BOTH, ORTHO_BOTH, ORTHO_DOWN}, {ORTHO_BOTH, ORTHO_LEFT, ORTHO_BOTH, ORTHO_UP}, {ORTHO_BOTH, ORTHO_RIGHT, ORTHO_UP, ORTHO_BOTH}, {EMPTY, EMPTY, DIAG_FRONT, DIAG_BACK}, {EMPTY, EMPTY, DIAG_BACK, DIAG_FRONT}, {DIAG_BACK, DIAG_FRONT, EMPTY, EMPTY}, {DIAG_FRONT, DIAG_BACK, EMPTY, EMPTY}, {EMPTY, DIAG_BOTH, EMPTY, ORTHO_BOTH}, {EMPTY, DIAG_BOTH, ORTHO_BOTH, EMPTY}, {DIAG_BOTH, EMPTY, EMPTY, ORTHO_BOTH}, {DIAG_BOTH, EMPTY, ORTHO_BOTH, EMPTY}, {EMPTY, ORTHO_BOTH, EMPTY, DIAG_BOTH}, {EMPTY, ORTHO_BOTH, DIAG_BOTH, EMPTY}, {ORTHO_BOTH, EMPTY, EMPTY, DIAG_BOTH}, {ORTHO_BOTH, EMPTY, DIAG_BOTH, EMPTY}, {EMPTY, DIAG_FRONT, ORTHO_DOWN, EMPTY}, {ORTHO_LEFT, EMPTY, DIAG_FRONT, EMPTY}, {DIAG_FRONT, EMPTY, EMPTY, ORTHO_UP}, {EMPTY, ORTHO_RIGHT, EMPTY, DIAG_FRONT}, {EMPTY, DIAG_BACK, EMPTY, ORTHO_DOWN}, {EMPTY, ORTHO_LEFT, DIAG_BACK, EMPTY}, {DIAG_BACK, EMPTY, ORTHO_UP, EMPTY}, {ORTHO_RIGHT, EMPTY, EMPTY, DIAG_BACK}, {EMPTY, EMPTY, ORTHO_BOTH, ORTHO_BOTH}, {EMPTY, EMPTY, EMPTY, DIAG_BOTH}, {EMPTY, DIAG_BOTH, EMPTY, EMPTY}, {ORTHO_BOTH, ORTHO_BOTH, EMPTY, EMPTY}, {EMPTY, DIAG_BOTH, EMPTY, DIAG_BOTH}, {DIAG_BOTH, EMPTY, EMPTY, DIAG_BOTH}, {DIAG_BOTH, EMPTY, DIAG_BOTH, EMPTY}, {EMPTY, DIAG_BOTH, DIAG_BOTH, EMPTY}, {DIAG_BOTH, EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, DIAG_BOTH, EMPTY}
	};
	const std::vector<int> glyphsWithTypeOnSide[NUM_TYPES][NUM_SIDES] = {
		// `glyphsWithTypeOnSide[ConnectionType type][Side side]` is the sorted vector of the glyphIndices that have ConnectionType type on Side side
		{ {0, 1, 3, 4, 5, 8, 9, 16, 17, 20, 22, 25, 26, 27, 37, 38, 42, 43, 44, 49, 54, 55, 59, 61, 69, 70, 74, 75, 76, 81, 86, 87, 92, 97, 98, 103, 107, 110, 122, 126, 127, 130, 132, 133, 134, 135, 136, 139, 140, 145, 146, 147, 151, 160, 161, 164, 165, 168, 169, 172, 175, 176, 177, 180, 181, 182, 184, 187, 189}, {0, 1, 3, 12, 14, 15, 17, 28, 30, 31, 35, 38, 45, 47, 48, 50, 52, 54, 56, 58, 59, 61, 67, 70, 77, 79, 80, 82, 86, 88, 89, 90, 92, 97, 98, 105, 108, 109, 121, 125, 128, 129, 131, 133, 134, 137, 138, 141, 142, 145, 146, 149, 153, 160, 161, 166, 167, 170, 171, 173, 174, 178, 179, 180, 181, 185, 186, 188, 189}, {0, 4, 6, 8, 13, 14, 20, 23, 25, 29, 30, 32, 33, 38, 39, 44, 47, 49, 50, 53, 58, 60, 64, 65, 66, 70, 76, 79, 81, 82, 85, 90, 94, 95, 96, 106, 109, 110, 119, 123, 129, 130, 131, 132, 134, 135, 137, 139, 141, 143, 144, 150, 154, 162, 163, 164, 166, 168, 170, 174, 175, 176, 179, 181, 182, 183, 184, 185, 188}, {0, 4, 9, 10, 13, 15, 16, 20, 24, 26, 27, 29, 31, 32, 35, 36, 37, 40, 41, 48, 50, 54, 60, 64, 67, 68, 69, 73, 78, 80, 82, 86, 94, 95, 96, 104, 107, 108, 120, 124, 127, 128, 131, 132, 133, 136, 138, 140, 142, 143, 144, 148, 152, 162, 163, 165, 167, 169, 171, 172, 173, 177, 178, 182, 183, 186, 187, 188, 189} }, { {6, 7, 10, 11, 12, 13, 14, 15, 19, 21, 23, 24, 52, 57, 63, 64, 65, 66, 67, 68, 71, 72, 77, 78, 82, 83, 88, 90, 101, 102, 105, 121, 166, 167, 185, 186, 188}, {5, 7, 19, 20, 27, 29, 41, 53, 62, 64, 65, 66, 68, 69, 71, 72, 73, 74, 75, 76, 78, 81, 83, 84, 85, 87, 91, 93, 99, 100, 103, 122, 164, 165, 182, 184, 187}, {7, 16, 17, 18, 21, 22, 24, 40, 42, 45, 46, 59, 63, 67, 68, 69, 71, 73, 74, 77, 80, 83, 84, 86, 87, 88, 89, 91, 100, 101, 104, 120, 169, 171, 186, 187, 189}, {7, 11, 12, 17, 18, 21, 22, 23, 25, 28, 34, 39, 43, 61, 65, 70, 72, 75, 79, 81, 83, 84, 85, 87, 88, 89, 90, 93, 99, 102, 106, 119, 168, 170, 181, 184, 185} }, { {2, 18, 28, 29, 30, 31, 32, 33, 34, 35, 36, 39, 40, 41, 45, 46, 47, 48, 50, 51, 53, 56, 58, 60, 62, 73, 79, 80, 84, 85, 89, 91, 93, 94, 111, 113, 125, 131, 148, 149, 150, 152, 153, 154, 155, 158, 159, 170, 171, 183}, {2, 4, 6, 8, 9, 10, 11, 13, 16, 18, 21, 22, 23, 24, 25, 26, 32, 33, 34, 36, 37, 39, 40, 42, 43, 44, 46, 49, 51, 55, 57, 60, 63, 94, 111, 113, 126, 132, 147, 148, 150, 151, 152, 154, 155, 156, 157, 168, 169, 183}, {1, 2, 3, 5, 9, 10, 11, 12, 15, 19, 26, 27, 28, 31, 34, 35, 36, 37, 41, 43, 48, 51, 52, 54, 55, 56, 57, 61, 62, 72, 75, 78, 92, 93, 112, 114, 124, 133, 147, 148, 149, 151, 152, 153, 155, 157, 158, 165, 167, 180}, {1, 2, 3, 5, 6, 8, 14, 19, 30, 33, 38, 42, 44, 45, 46, 47, 49, 51, 52, 53, 55, 56, 57, 58, 59, 62, 63, 66, 71, 74, 76, 77, 91, 92, 112, 114, 123, 134, 147, 149, 150, 151, 153, 154, 155, 156, 159, 164, 166, 180} }, { {99, 106, 109, 163, 174}, {101, 104, 107, 162, 172}, {102, 105, 108, 160, 173}, {100, 103, 110, 161, 175} }, { {100, 104, 108, 162, 178}, {102, 106, 110, 163, 176}, {99, 103, 107, 161, 177}, {101, 105, 109, 160, 179} }, { {}, {}, {97, 113, 116, 117, 121, 125, 128, 136, 142, 146, 159, 178}, {97, 113, 116, 117, 121, 125, 129, 135, 141, 145, 158, 174} }, { {}, {}, {98, 111, 115, 118, 122, 126, 127, 138, 140, 145, 156, 172}, {98, 111, 115, 118, 122, 126, 130, 137, 139, 146, 157, 176} }, { {96, 112, 115, 116, 120, 124, 128, 137, 142, 143, 157, 173}, {96, 112, 115, 116, 120, 124, 127, 135, 140, 144, 158, 177}, {}, {} }, { {95, 114, 117, 118, 119, 123, 129, 138, 141, 144, 156, 179}, {95, 114, 117, 118, 119, 123, 130, 136, 139, 143, 159, 175}, {}, {} }
	};

	const std::vector<int>   allGlyphIndices = { // The indices of the glyphs in order, from 0 to NUM_GLYPHS
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189
	};
	const std::vector<int>   sameAfterRotate = { // Sorted vector of all glyph indices for glyphs that are the same after a 90deg CW rotation
		0, 2, 7, 51, 83, 155
	};
	const std::vector<int> sameAfterReflectY = { // Sorted vector of all glyph indices for glyphs that are the same after a left-right reflection
		0, 1, 2, 3, 4, 5, 7, 13, 17, 18, 19, 20, 21, 22, 29, 32, 50, 51, 52, 55, 56, 57, 60, 62, 64, 82, 83, 84, 87, 88, 89, 92, 94, 97, 98, 103, 105, 111, 113, 121, 122, 125, 126, 131, 132, 147, 149, 151, 153, 155, 160, 161, 180, 182, 183, 188
	};
	const std::vector<int> sameAfterReflectX = { // Sorted vector of all glyph indices for glyphs that are the same after an up-down reflection
		0, 1, 2, 3, 7, 17, 18, 19, 32, 33, 34, 36, 38, 39, 40, 46, 51, 54, 59, 60, 61, 64, 65, 66, 68, 70, 71, 72, 78, 83, 86, 92, 94, 95, 96, 104, 106, 112, 114, 119, 120, 123, 124, 133, 134, 148, 150, 152, 154, 155, 162, 163, 180, 181, 183, 189
	};
	const std::vector<int> connectToReflectY = { // Sorted vector fo all glyph indices for glyphs that can connect on the RIGHT side to their Y-reflected glyph
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 102, 104, 106, 107, 108, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 176, 177, 178, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189
	};
	const std::vector<int> connectToReflectX = { // Sorted vector fo all glyph indices for glyphs that can connect on the BOTTM side to their X-reflected glyph
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 103, 105, 108, 109, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 164, 165, 166, 167, 168, 169, 170, 171, 173, 174, 175, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189
	};
#endif /* End definitions for `Knot` class */

#define NUM_SIDES 4
#define NUM_TYPES 9
const ConnectionType  rotateTypes[]{ EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_RIGHT, ORTHO_LEFT, ORTHO_UP, ORTHO_DOWN };
const ConnectionType mirrorXTypes[]{ EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_DOWN, ORTHO_UP, ORTHO_LEFT, ORTHO_RIGHT };
const ConnectionType mirrorYTypes[]{ EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_BACK, DIAG_FRONT, ORTHO_UP, ORTHO_DOWN, ORTHO_RIGHT, ORTHO_LEFT };

struct Glyph {
	wxString chr;

	unsigned int index     : 16;
	unsigned int rotated   : 16;
	unsigned int mirroredX : 16;
	unsigned int mirroredY : 16;

	union {
		struct {
			ConnectionType up : 4;
			ConnectionType down : 4;
			ConnectionType left : 4;
			ConnectionType right : 4;

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

	Glyph(unsigned int index_, unsigned int rotated_, unsigned int mirroredX_, unsigned int mirroredY_,
	wxString chr_, ConnectionType up_, ConnectionType down_, ConnectionType left_, ConnectionType right_) :
		up(up_), down(down_), left(left_), right(right_), chr(chr_),
		index(index_), rotated(rotated_), mirroredX(mirroredX_), mirroredY(mirroredY_),
		sameAfterRotate(index_ == rotated_),
		sameAfterMirrorY(index_ == mirroredY_),
		sameAfterMirrorX(index_ == mirroredX_),
		connectToMirrorUp(up_ == mirrorXTypes[up_]),
		connectToMirrorDown(down_ == mirrorXTypes[down_]),
		connectToMirrorLeft(left_ == mirrorYTypes[left_]),
		connectToMirrorRight(right_ == mirrorYTypes[right_]) {}
};

struct GlyphPossibilities {
	std::vector<Glyph> glyphList;
	std::array< std::array<unsigned int, NUM_SIDES>, NUM_TYPES> connectionCount{};

	GlyphPossibilities(const std::vector<Glyph>& glyphList_) : glyphList(glyphList_) {
		for (const Glyph& glyph : glyphList) {
			connectionCount[glyph.up][0]++;
			connectionCount[glyph.down][1]++;
			connectionCount[glyph.left][2]++;
			connectionCount[glyph.right][3]++;
		}
	}
	GlyphPossibilities(const GlyphPossibilities& input) :
		glyphList(input.glyphList), connectionCount(input.connectionCount) {}
};

const std::vector<Glyph> allGlyphs {
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

const GlyphPossibilities allGlyphPossibilities{ allGlyphs };

