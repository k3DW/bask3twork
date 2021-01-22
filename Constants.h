#pragma once
#include "wx/wx.h"
#include <array>
#include <optional>

#define intWX(x) wxString::Format(wxT("%i"),x)
#define VERSION(MAJOR, MINOR, PATCH) intWX(MAJOR) + "." + intWX(MINOR) + (PATCH == 0 ? wxString("") : wxString(".") + intWX(PATCH))

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

/* Begin definitions for `Knot` class */ #if true
	#define MAX_ATTEMPTS 10000
	#define ATTEMPTS_DISPLAY_INCREMENT 100
	const wxString MAX_ATTEMPTS_STR = intWX(MAX_ATTEMPTS);

	enum SideOLD { TOP, BOTTOM, LEFT, RIGHT };
	enum SideBitfield {
		bTOP	= 1 << TOP,
		bBOTTOM	= 1 << BOTTOM,
		bLEFT	= 1 << LEFT,
		bRIGHT	= 1 << RIGHT
	};
	enum ConnectionType { EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_UP, ORTHO_DOWN, ORTHO_LEFT, ORTHO_RIGHT };

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

	const ConnectionType   rotateTypes[NUM_TYPES] = { // Each ConnectionType's map after a 90deg CW rotation
		EMPTY, DIAG_BOTH, ORTHO_BOTH, DIAG_FRONT, DIAG_BACK, ORTHO_RIGHT, ORTHO_LEFT, ORTHO_UP, ORTHO_DOWN
	};
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

template<typename T>
T pop_random(std::vector<T>& vec) {
	int index = rand() % vec.size();	// Grab a random index
	T output(vec[index]);				// Store the value at that index
	vec.erase(vec.begin() + index);		// Remove the value from the vector
	return output;						// Return the value
}

template<typename T>
inline T pick_random(const std::vector<T>& vec) {
	return vec[rand() % vec.size()];
}