#include "pch.h"
#include "grid/Display.h"
#include "grid/Knot.h"
#include "grid/Tile.h"
#include "pure/Glyph.h"
#include "pure/GridSize.h"
#include "pure/Selection.h"
#include "pure/SelectionIterator.h"
#include "Constants.h"
#include "MainWindow.h"
#include <wx/dcmemory.h>
#include <wx/graphics.h>
#include <wx/dcgraph.h>

DisplayGrid::DisplayGrid(MainWindow* parent, GridSize size)
	: wxWindow(parent, wxID_ANY)
	, parent(parent)
	, glyph_font(Fonts::glyph)
	, axis_font(Fonts::axis)
{
	Hide();
	resize(size);

	Bind(wxEVT_LEFT_DOWN, &DisplayGrid::on_left_down, this);
	Bind(wxEVT_RIGHT_DOWN, &DisplayGrid::on_right_down, this);
	Bind(wxEVT_LEFT_DCLICK, &DisplayGrid::on_left_dclick, this);
	Bind(wxEVT_PAINT, &DisplayGrid::on_paint, this);

	SetDoubleBuffered(true);
	Show();
}

DisplayGrid::~DisplayGrid()
{
	Hide();
}



void DisplayGrid::resize(GridSize size)
{
	grid_size = size;
	make_tiles();
	update_sizes_and_offsets();
	knot = nullptr;
}

void DisplayGrid::set_glyph_font_size(int i)
{
	wxSize size{ i, i };
	glyph_font.SetPixelSize(size);

	const int axis_point_size = std::max(std::min(Sizes::font_point, size.x / 3), 1);
	axis_font.SetPointSize(axis_point_size);

	update_sizes_and_offsets();
}

void DisplayGrid::update_sizes_and_offsets()
{
	showing = false;
	background_cache = {};
	glyph_font_size = glyph_font.GetPixelSize();

	wxSize axis_font_size = axis_font.GetPixelSize();
	tiles_offset = { axis_font_size.y / 2 + 1, axis_font_size.y + 1 }; // For some reason, the x value reads 0
	if (grid_size.rows >= 10)
		tiles_offset.x *= 2;

	tiles_size = { glyph_font_size.x * grid_size.columns, glyph_font_size.y * grid_size.rows };
	window_size = { tiles_offset.x + tiles_size.x, tiles_offset.y + tiles_size.y };

	update_tile_offsets();
	SetMinSize(wxDefaultSize);
	SetMinSize(window_size);

	reset_selection();
}



void DisplayGrid::on_left_down(wxMouseEvent& evt)
{
	const Point tile_pos = tile_position(evt.GetPosition());
	if (tile_pos == Point{ -1, -1 })
		return evt.Skip();

	if (evt.GetModifiers() == wxMOD_CONTROL)
	{
		Tile& tile = tiles[tile_pos.i][tile_pos.j];
		tile.locked()
			? tile.unlock()
			: tile.lock();
		render();
		return evt.Skip();
	}

	parent->disable_buttons();

	wxCursor cursor(wxCURSOR_HAND);
	SetCursor(cursor);

	showing = true;
	highlight_in_progress = true;
	selection_start = tile_pos;
	selection = { tile_pos, tile_pos };

	Bind(wxEVT_LEFT_UP, &DisplayGrid::on_left_up, this);
	Bind(wxEVT_MOTION, &DisplayGrid::on_motion, this);
	Bind(wxEVT_MOUSE_CAPTURE_LOST, &DisplayGrid::on_capture_lost, this);

	CaptureMouse();

	evt.Skip();
}

void DisplayGrid::on_right_down(wxMouseEvent& evt)
{
	const Point tile_pos = tile_position(evt.GetPosition());
	if (tile_pos == Point{ -1, -1 })
		return evt.Skip();

	if (evt.HasAnyModifiers())
		return evt.Skip();

	unhighlight();
	evt.Skip();
}

void DisplayGrid::on_left_dclick(wxMouseEvent& evt)
{
	const Point tile_pos = tile_position(evt.GetPosition());
	if (tile_pos == Point{ -1, -1 })
		return evt.Skip();

	if (evt.HasAnyModifiers())
		return evt.Skip();

	reset_selection();
	parent->enable_buttons();
	render();
	evt.Skip();
}



void DisplayGrid::on_motion(wxMouseEvent& evt)
{
	Point pos = tile_position_clamp(evt.GetPosition());
	Selection new_selection{ selection_start, pos };
	new_selection.normalize();
	if (selection != new_selection)
	{
		selection = new_selection;
		render();
	}
}

void DisplayGrid::on_left_up(wxMouseEvent& evt)
{
	Point pos = tile_position_clamp(evt.GetPosition());
	selection = { selection_start, pos };
	selection.normalize();
	finish_highlight();
}

void DisplayGrid::on_capture_lost(wxMouseCaptureLostEvent&)
{
	finish_highlight();
}

void DisplayGrid::finish_highlight()
{
	if (!highlight_in_progress)
		return;

	SetCursor(wxNullCursor);
	if (HasCapture())
	{
		ReleaseMouse();
	}

	Unbind(wxEVT_LEFT_UP, &DisplayGrid::on_left_up, this);
	Unbind(wxEVT_MOTION, &DisplayGrid::on_motion, this);
	Unbind(wxEVT_MOUSE_CAPTURE_LOST, &DisplayGrid::on_capture_lost, this);

	selection_start = { -1, -1 };
	highlight_in_progress = false;
	render();

	parent->enable_buttons();
}



void DisplayGrid::on_paint(wxPaintEvent&)
{
	wxPaintDC dc(this);
	render(dc);
}

void DisplayGrid::render()
{
	wxClientDC dc(this);
	render(dc);
}

void DisplayGrid::render(wxDC& dc)
{
	dc.SetPen(*wxTRANSPARENT_PEN);
	if (!background_cache)
	{
		background_cache = wxBitmap{ window_size };
		wxMemoryDC mem_dc(*background_cache);
		mem_dc.SetPen(*wxTRANSPARENT_PEN);
		render_axis_labels(mem_dc);
		render_tiles(mem_dc, false);
	}
	dc.DrawBitmap(*background_cache, 0, 0);
	//render_tiles(dc, true);

	wxBitmap bmp{ window_size };
	bmp.UseAlpha();
	wxMemoryDC memdc(bmp);

	[[maybe_unused]]
#ifdef __WXMSW__
	wxGraphicsContext* gc = wxGraphicsRenderer::GetDirect2DRenderer()->CreateContext(memdc);
#else
	wxGraphicsContext* gc = wxGraphicsContext::Create(memdc);
#endif

	wxColour colour = Colours::tile_highlighted[0][0];
	//wxColour c(colour.Red(), colour.Green(), colour.Blue(), 128);
	//wxBrush b(c);

	//dc.SetBrush(b);
	//dc.DrawRectangle(50, 50, 450, 450);

	//gc->BeginLayer(0.5);
	////gc->SetBrush(b);
	//gc->DrawRectangle(50, 50, 250, 250);
	//gc->EndLayer();

	delete gc;

	//wxGCDC gcdc(gc);
	//gcdc.SetBrush(b);
	//gcdc.DrawRectangle(50, 50, 450, 450);




	auto Square_marker = [](wxGraphicsContext & gc, wxPoint2DDouble center, double halfSize) -> wxGraphicsPath
	{

		wxGraphicsPath path = gc.CreatePath();
		wxPoint2DDouble botRight = center + wxPoint2DDouble(halfSize, -halfSize);
		wxPoint2DDouble topRight = botRight + wxPoint2DDouble(0.0, 2 * halfSize);
		wxPoint2DDouble topLeft = topRight + wxPoint2DDouble(-2 * halfSize, 0.0);
		wxPoint2DDouble botLeft = topLeft + wxPoint2DDouble(0.0, -2 * halfSize);

		path.MoveToPoint(botRight);
		path.AddLineToPoint(topRight);
		path.AddLineToPoint(topLeft);
		path.AddLineToPoint(botLeft);
		path.AddLineToPoint(botRight);
		path.CloseSubpath();
		return path;
	};

	auto MarkerBitmap = [&]() -> wxBitmap // Draw a square marker onto a bitmap
	{
		// Create a memory DC
		wxBitmap   theBitmap(window_size);

		// Enable transparency
		theBitmap.UseAlpha();

		// Initialize image
		wxMemoryDC memDC(theBitmap);
		memDC.Clear();
		memDC.SetBackground(*wxTRANSPARENT_BRUSH);

		wxGraphicsContext* dc = wxGraphicsContext::Create(memDC);
		//wxGraphicsContext* dc = wxGraphicsRenderer::GetDirect2DRenderer()->CreateContext(memDC);

		//dc->SetPen(wxPen(wxColour(255, 0, 0, 255), 2));

		//wxPoint2DDouble center(12.0, 12.0);
		//double radius = 8;
		//
		//// Create shape on the graphics context
		//wxGraphicsPath shape;
		//
		//// Get point marker drawing
		//shape = Square_marker(*dc, center, radius);
		//// Draw shape
		//dc->FillPath(shape);
		//dc->StrokePath(shape);


		//dc->DrawRectangle(50, 50, 200, 200);

		dc->BeginLayer(0.1);

		dc->SetPen(*wxTRANSPARENT_PEN);
		dc->SetBrush(colour);
		dc->DrawRectangle(100, 100, 200, 200);

		//wxGCDC gcdc(dc);
		//gcdc.SetBrush(colour);
		//gcdc.DrawRectangle(100, 100, 200, 200);

		dc->EndLayer();
		delete dc;

		return theBitmap;
	};

	memdc.SetPen(*wxTRANSPARENT_PEN);
	memdc.SetBrush(colour);
	memdc.DrawRectangle(50, 50, 50, 50);


	dc.DrawBitmap(MarkerBitmap(), 0, 0);


	//render_knot(dc);
}

void DisplayGrid::render_axis_labels(wxDC& dc)
{
	static const wxBrush background = Colours::background;
	dc.SetBrush(background);
	dc.DrawRectangle({ 0, 0 }, { tiles_offset.x, window_size.y });
	dc.DrawRectangle({ 0, 0 }, { window_size.x, tiles_offset.y });

	dc.SetFont(axis_font);

	for (int i = 0; i < grid_size.rows; i++)
		dc.DrawText(wxString::Format("%i", i + 1), y_label_offset(i));

	for (int j = 0; j < grid_size.columns; j++)
		dc.DrawText(wxString::Format("%i", j + 1), x_label_offset(j));
}

void DisplayGrid::render_tiles(wxDC& dc, bool special) const
{
	for (int i = 0; i < grid_size.rows; i++)
		for (int j = 0; j < grid_size.columns; j++)
		{
			if (special)
				tiles[i][j].render_special(dc, glyph_font_size, TileHighlighted{ showing && selection.contains({ i, j }) });
			else
				tiles[i][j].render_base(dc, glyph_font_size);
		}
}

void DisplayGrid::render_knot(wxDC& dc)
{
	if (nullptr == knot)
		return;

	dc.SetFont(glyph_font);
	for (int i = 0; i < grid_size.rows; i++)
	{
		wxString str;
		for (int j = 0; j < grid_size.columns; j++)
		{
			str << knot->get(i, j);
		}
		dc.DrawText(str, tile_offset(i, 0));
	}
}



void DisplayGrid::lock(Point point)
{
	tiles[point.i][point.j].lock();
	render();
}

void DisplayGrid::lock_no_render(Point point)
{
	tiles[point.i][point.j].lock();
}

void DisplayGrid::unlock(Point point)
{
	tiles[point.i][point.j].unlock();
	render();
}

void DisplayGrid::lock()
{
	for (Point p : SelectionRange(selection))
		tiles[p.i][p.j].lock();
	render();
}

void DisplayGrid::unlock()
{
	for (Point p : SelectionRange(selection))
		tiles[p.i][p.j].unlock();
	render();
}

void DisplayGrid::invert_locking()
{
	for (Point p : SelectionRange(selection))
	{
		Tile& tile = tiles[p.i][p.j];
		tile.locked()
			? tile.unlock()
			: tile.lock();
	}
	render();
}



void DisplayGrid::make_tiles()
{
	tiles.clear();
	const auto [rows, columns] = grid_size;
	tiles.reserve(rows);
	for (int i = 0; i < rows; i++)
	{
		auto& row = tiles.emplace_back();
		row.reserve(columns);
		for (int j = 0; j < columns; j++)
		{
			row.emplace_back(TileBrushes::all[i % 2][j % 2], tile_offset(i, j));
		}
	}
}

void DisplayGrid::update_tile_offsets()
{
	if (tiles.empty())
		return;

	const auto [rows, columns] = grid_size;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			tiles[i][j].set_offset(tile_offset(i, j));
}

wxPoint DisplayGrid::x_label_offset(int pos) const
{
	int x = tiles_offset.x + (pos * glyph_font_size.x) + (glyph_font_size.x / 2) - (tiles_offset.x / 2);
	int y = 0;
	return { x, y };
}

wxPoint DisplayGrid::y_label_offset(int pos) const
{
	int x = (grid_size.rows < 10 || pos >= 9) ? 0 : tiles_offset.x / 4;
	int y = tiles_offset.y + (pos * glyph_font_size.y) + (glyph_font_size.y / 2) - (tiles_offset.y / 2);
	return { x, y };
}

wxPoint DisplayGrid::tile_offset(int i, int j) const
{
	int x = tiles_offset.x + (j * glyph_font_size.x);
	int y = tiles_offset.y + (i * glyph_font_size.y);
	return { x, y };
}

Point DisplayGrid::tile_position(wxPoint offset) const
{
	offset -= tiles_offset;
	if (offset.x < 0 || offset.y < 0)
		return { -1, -1 };
	const int i = offset.y / glyph_font_size.y;
	const int j = offset.x / glyph_font_size.x;
	return { i, j };
}

Point DisplayGrid::tile_position_clamp(wxPoint offset) const
{
	offset -= tiles_offset;
	const int i = offset.y / glyph_font_size.y;
	const int j = offset.x / glyph_font_size.x;
	return { std::max(0, std::min(i, grid_size.rows - 1)), std::max(0, std::min(j, grid_size.columns- 1)) };
}
