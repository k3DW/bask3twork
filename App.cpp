/* TODO for version 0.2 */
/*
	~~1) Make vert+hori symmetric knots
		~~1a) Button
		~~1b) Generate knot
		~~1c) Check symmetry
	2) Make 2-way rotationally symmetric knots
		~~2a) Button
		2b) Generate knot
		2c) Check symmetry
	3) Make 4-way rotationally symmetric knots (square only)
		~~3a) Button
		3b) Generate knot
		3c) Check symmetry
	~~4) Slightly streamline Knot::generateXXXSym() logic
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
#define MINOR_VERSION 2
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