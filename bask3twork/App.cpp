/* TODO for later versions */
/*
	-> Knot navigation, like moving all the data in a certain direction
	-> Undo/redo (up to a certain level)
	-> Tooltips (and other little details)
	-> Copying sections of the knot
	-> Inputting individual glyphs
		-> Either typing it in
		-> Or selecting from all possible glyphs
	-> Circle blocking (selecting)
	-> Simple weaves as example knots
	-> Hotkeys
		-> Ctrl+C, enter in the export dialog
		-> Enter in the regen dialog
		-> Space to toggle highlighting
		-> Escape to reset highlighting
		-> Ctrl+1 - Ctrl+9 to generate the knots
	-> Add a "clear" button that clears the area
		-> Only if that's possible given the borders
		-> Hotkey Ctrl+Del
*/

#include "pch.h"
#include "MainWindow.h"
#include "Version.h"

class App : public wxApp
{
public:
	bool OnInit() override
	{
		auto main = new MainWindow({ .rows = 8, .columns = 8 }, "Bask3twork v" + Version::string);
		main->SetIcon(wxIcon("AppIcon"));
		main->Show();

		return true;
	}
};

wxIMPLEMENT_APP(App);
