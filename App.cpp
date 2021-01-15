/* TODO for version 0.3 */
/*
	
	1) Implement Wave Function Collapse algorithm for no symmetry
		~~1a) Setup steps
		~~1b) Ending steps
		1c) Looping steps
		1d) Optimizations
	2) Integrate the WFC algorithm as the make generation alg
		2a) Turn the Setup and Ending steps to be more integrated into the `Knot` object,
			i.e use `glyphs` instead of `glyphIndices` as a class member
		2b) Convert the regular generate buttons to the WFC algorithm
		2c) Remove the "Wave Collapse No Sym" button
	
	3) Make 2-way rotationally symmetric knots
		~~3a) Button
		3b) Generate knot
		3c) Check symmetry
	4) Make 4-way rotationally symmetric knots (square only)
		~~4a) Button
		4b) Generate knot
		4c) Check symmetry

*/

/* TODO for later versions */
/*
	-> "Copy to clipboard" button with the export box
	-> Knots wrapping around the borders
	-> More of the symmetries
		-> full sym (square only)
		-> back diag sym (square only)
		-> fwd diag sym (square only)
	-> DisplayGrid automatically changes size depending on the size of the screen/window
	-> Tooltips (and other little details)
	-> Copying sections of the knot
	-> Inputting individual glyphs
		-> Either typing it in
		-> Or selecting from all possible glyphs
	-> Locking glyphs in place
	-> Undo/redo
	-> Save state
		-> Also load state
	-> Circle blocking (selecting)
	-> Simple weaves as example knots
*/

#pragma once
#include "wx/wx.h"
#include "MainWindow.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 3
#define PATCH_VERSION 0

const wxString version = VERSION(MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);

class App : public wxApp {

private:
	MainWindow* main;

public:
	virtual bool OnInit() {
		main = new MainWindow(8, 8, "Celtic Knot Editor v" + version);
		main->SetIcon(wxICON(AppIcon));
		main->Show();
		srand(time(NULL));
		return true;
	}
};

wxIMPLEMENT_APP(App);