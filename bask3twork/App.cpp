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
*/

#include "pch.h"
#include "MainWindow.h"
#include "Version.h"

#include <wx/dcmemory.h>
#include <wx/graphics.h>
#include <wx/dcgraph.h>
#include <wx/dcbuffer.h>

class Window : public wxWindow
{
public:
	Window(wxFrame* parent)
		: wxWindow(parent, wxID_ANY)
	{
		Bind(wxEVT_PAINT, &Window::OnPaint, this);
		SetMinSize(wxSize(400,400));
	}

	void OnPaint(wxPaintEvent&)
	{
		wxPaintDC dc(this);

		dc.SetPen(*wxTRANSPARENT_PEN);

		wxBitmap bmp(400, 400);
		bmp.UseAlpha();
		wxMemoryDC memDC(bmp);
		memDC.SetPen(*wxTRANSPARENT_PEN);

		wxColour colour(255, 0, 0);
		memDC.SetBrush(colour);
		memDC.DrawRectangle(100, 100, 200, 200);


		dc.SetBrush(colour);
		dc.DrawRectangle(50, 50, 50, 50);
		dc.DrawBitmap(bmp, 0, 0);
	}
};

class App : public wxApp
{
public:
	bool OnInit() override
	{
		wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Test");
		[[maybe_unused]] Window* window = new Window(frame);
		frame->SetMinSize(wxSize(400, 400));
		frame->Show();
		return true;
	}
};

wxIMPLEMENT_APP(App);
