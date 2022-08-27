#include "controls/MenuBar.h"
#include "MainWindow.h"

MenuBar::MenuBar(MainWindow* parent)
	: wxMenuBar()
{
	file_menu = new wxMenu();
	file_menu->Bind(wxEVT_MENU, &MainWindow::menu_event_handler, parent);
	file_menu->Append(static_cast<int>(MenuID::OPEN), "&Open\tCtrl-O", "Open a knot file.");
	file_menu->Append(static_cast<int>(MenuID::SAVE), "&Save\tCtrl-S", "Save a knot file.");

	generate_menu = new wxMenu();
	generate_menu->Bind(wxEVT_MENU, &MainWindow::menu_event_handler, parent);
	wrap_x = generate_menu->AppendCheckItem(static_cast<int>(MenuID::WRAP_X), "Wrap &X\tCtrl-X", "Toggle wrapping around the grid in the left-right direction.");
	wrap_y = generate_menu->AppendCheckItem(static_cast<int>(MenuID::WRAP_Y), "Wrap &Y\tCtrl-Y", "Toggle wrapping around the grid in the up-down direction.");
	generate_menu->AppendSeparator();
	generate_menu->Append(static_cast<int>(MenuID::REGEN_GRID), "&Regenerate\tCtrl-R", "Resize and reinitialize the grid.");

	Append(file_menu, "&File");
	Append(generate_menu, "&Generate");

	parent->SetMenuBar(this);
}

void MenuBar::reset_wrapping()
{
	wrap_x->Check(false);
	wrap_y->Check(false);
}
