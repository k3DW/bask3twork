/* TODO for version 0.4 */
/*
	~~0) Change Glyph structure
		~~0a) Change GlyphFlag to be 64 bit
		~~0b) Propagate those changes everywhere (yikes)
		~~0c) Add SA_MIRFD and SA_MIRBD flags
		~~0d) Add corresponding flags to the Glyph struct
	1) More symmetries
		1a) Add new buttons to the window
		1b) Change `Knot::tryGenerating()` to make diagonal structures
		1c) Generate full symmetry (square only)
		1d) Generate forward diagonal symmetry (square only)
		1e) Generate backward diagonal symmetry (square only)
		1f) Check forward diagonal symmetry
		1g) Check backward diagonal symmetry
	2) Save states
		2a) Function to write a GlyphVec2 into a ".knot" file in a specified file location, with visual file explorer
		2b) Menu bar, with "File -> Save" that calls that function, saves as ".knot" file
		2c) Function to read a ".knot" file, check to see if it's valid, then put into a GlyphVec2/Knot object and set the knot
		2b) Menu bar, with "File -> Open" that calls that function
*/

/* TODO for version 0.5 */
/*
	1) Tile locking
	2) Undo/redo (up to a certain level)
	3) Wrapping around borders?
*/

/* TODO for later versions */
/*
	-> "Copy to clipboard" button with the export box
	-> Knots wrapping around the borders
	-> DisplayGrid automatically changes size depending on the size of the screen/window
	-> Tooltips (and other little details)
	-> Copying sections of the knot
	-> Inputting individual glyphs
		-> Either typing it in
		-> Or selecting from all possible glyphs
	-> Circle blocking (selecting)
	-> Simple weaves as example knots
*/

#pragma once
#include "MainWindow.h"

class App : public wxApp {

private:
	MainWindow* main;

	static constexpr int MAJOR_VERSION = 0;
	static constexpr int MINOR_VERSION = 4;
	static constexpr int PATCH_VERSION = 0;
	static const wxString VERSION;

public:
	virtual bool OnInit() {
		main = new MainWindow(8, 8, "Celtic Knot Editor v" + VERSION);
		main->SetIcon(wxICON(AppIcon));
		main->Show();
		srand(time(NULL));
		return true;
	}
};

const wxString App::VERSION = intWX(MAJOR_VERSION) + "." + intWX(MINOR_VERSION) + (PATCH_VERSION == 0 ? wxString("") : wxString("." + intWX(PATCH_VERSION)));

wxIMPLEMENT_APP(App);