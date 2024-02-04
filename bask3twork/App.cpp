/* TODO for later versions */
/*
	-> Properly fix regeneration performance
		-> See https://forums.wxwidgets.org/viewtopic.php?p=220992
		-> May need to draw text manually
	-> Knot navigation, like moving all the data in a certain direction
	-> Undo/redo (up to a certain level)
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
		auto main = new MainWindow({ .rows = 8, .columns = 8 }, "Bask3twork v" + Version::string);
		main->SetIcon(wxIcon("AppIcon"));
		main->Show();

		return true;
	}
};

wxIMPLEMENT_APP(App);
