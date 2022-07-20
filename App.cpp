/* TODO for later versions */
/*
	-> Tile locking
	-> Knot navigation, like moving all the data in a certain direction
	-> Undo/redo (up to a certain level)
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
	static constexpr int PATCH_VERSION = 1;
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