/* TODO for version 0.3 */
/*
	
	~~1) Redo the Knot data structures
		(Breaking this down into subtasks would have been difficult, but it runs a lot faster now!)
	~~2) Redo the horizontal and vertical symmetry checking in `Knot` class
		~~2a) Do horizontal
		~~2b) Do vertical
		~~2c) Implement in `MainWindow::enableGenerateButtons()`
	~~3) Make 2-way rotationally symmetric knots
		~~3a) Button
		~~3b) Generate knot
		3c) Check symmetry
	4) Make 4-way rotationally symmetric knots (square only)
		~~4a) Button
		4b) Generate knot
		4c) Check symmetry
	~~5) Change indices in `Glyph` objects from `unsigned int` to `const Glyph* const`
		~~5a) Update `Glyph` constructors
		~~5b) Update Excel
		~~5c) Update `std::array<Glyph, Glyph::TOTAL> AllGlyphs`
		~~5d) Update all calls to the old index variables

*/

/* TODO for later versions */
/*
	-> Change WFC algorithm to select tiles by entropy instead of random
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