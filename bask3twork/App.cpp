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

#include "pch.h"
#include "MainWindow.h"
#include "Version.h"

class App : public wxApp
{
public:
	bool OnInit() override
	{
		auto main = new MainWindow(8, 8, "Bask3twork v" + Version::string);
		main->SetIcon(wxIcon("AppIcon"));
		main->Show();

		return true;
	}
};

wxIMPLEMENT_APP(App);
