#pragma once
#include <wx/menu.h>
#include <array>
#include "MainWindow.h"

class MenuBar : wxMenuBar
{
public:
	MenuBar(MainWindow* parent);

	void reset_wrapping();

	bool is_wrap_x() const { return wrap_x->IsChecked(); }
	bool is_wrap_y() const { return wrap_y->IsChecked(); }

	enum class MenuID
	{
		OPEN,
		SAVE,
		EXPORT_GRID,
		WRAP_X,
		WRAP_Y,
		REGEN_GRID,
	};

	static constexpr std::array functions =
	{
		&MainWindow::open_file,
		&MainWindow::save_file,
		&MainWindow::export_grid,
		&MainWindow::update_wrap_x,
		&MainWindow::update_wrap_y,
		&MainWindow::regenerate_grid,
	};

private:
	wxMenu* file_menu;

	wxMenu* generate_menu;
	wxMenuItem* wrap_x;
	wxMenuItem* wrap_y;
};
