/* TODO for version 0.5 */
/*
	~~1) "Copy to clipboard" button with the export box
		~~1a) Add physical button
		~~1b) Add functionality
	2) Wrapping around borders
		~~2a) Add wxMenuItems to toggle wrapping in X and Y respectively
		~~2b) Add 2 bool member variables to the Knot class to save wrapping state
		~~2c) Update Knot::tryGenerating() to incorporate wrapping functionality
		~~2c-1) Fix Knot::generateHoriSym()
		~~2c-2) Fix Knot::generateVertSym()
		2c-3) Fix Knot::generateHoriVertSym()
		2c-4) Fix Knot::generateRot2Sym()
		2c-5) Fix Knot::generateRot4Sym()
		2c-6) Fix Knot::generateFwdDiag()
		2c-7) Fix Knot::generateBackDiag()
		2c-8) Fix Knot::generateFullSym()
		2d) Update Knot::tryGeneratingDiag() to incorporate wrapping functionality
		2e) Update Knot::checkHoriSym() to incorporate wrapping functionality
		2f) Update Knot::checkVertSym() to incorporate wrapping functionality
		2g) Update Knot::checkRot2Sym() to incorporate wrapping functionality
		2h) Update Knot::checkRot4Sym() to incorporate wrapping functionality
		2i) Update Knot::checkFwdDiag() to incorporate wrapping functionality
		2j) Update Knot::checkBackDiag() to incorporate wrapping functionality
	3) Put more things into the wxMenuBar to remove bloat from the right side
		3a) Enable or disable symmetry buttons
		3b) Put grid regeneration into its own wxMenu
	4) Tile locking
		4a) Flesh this one out once the others are done
*/

/* TODO for later versions */
/*
	-> Knot navigation, like moving all the data in a certain direction
	-> Undo/redo (up to a certain level)
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
	static constexpr int MINOR_VERSION = 5;
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