/* TODO for version 0.4 */
/*
	
	

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
#include "MainWindow.h"

class App : public wxApp {

private:
	MainWindow* main;

	static constexpr int MAJOR_VERSION = 0;
	static constexpr int MINOR_VERSION = 3;
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