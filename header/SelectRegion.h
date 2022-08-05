#pragma once
#include "wx/wx.h"
#include "Selection.h"

class MainWindow;

class SelectRegion : public wxStaticBoxSizer
{
public:
	SelectRegion(MainWindow* parent, int h, int w);

	void update_display();     ///< Updates the displayed selection coordinates, also has other effects.
	void set_min(Point point); ///< Sets the \c min portion of the displayed selection
	void set_max(Point point); ///< Sets the \c max portion of the displayed selection
	void normalize();          ///< Normalizes the selection such that \c min is the top left point and \c max is the bottom right point

	void set_toggle_show() { toggle_button->SetLabelText("Show"); }
	void set_toggle_hide() { toggle_button->SetLabelText("Hide"); }

	Selection get_selection() const { return selection; }

private:
	wxButton*   toggle_button;
	wxButton*   reset_button;
	wxBoxSizer* button_sizer;

	Selection selection;   ///< The pair of zero-indexed coordinates representing the top left (lower numerically) and bottom right (higher numerically) of the current selection
	wxStaticText* display; ///< The display of "selection coordinates", i.e. top-left to bottom-right
};