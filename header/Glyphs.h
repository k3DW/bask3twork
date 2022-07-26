#pragma once
#include "Glyph.h"

struct Point
{
	int i;
	int j;
};

struct Selection
{
	Point min;
	Point max;

	bool is_square() const { return max.i - min.i == max.j - min.j; }
	bool is_full_selection(int rows, int columns) const { return min.i == 0 && min.j == 0 && max.i == rows - 1 && max.j == columns - 1; }
};

class GlyphIterator;

class Glyphs
{
public:
	Glyphs(std::size_t rows, std::size_t columns) : data(rows, std::vector<const Glyph*>(columns, DefaultGlyph)) {}
	Glyphs(std::vector<std::vector<const Glyph*>> data) : data(data) {}

	GlyphIterator upper_left (Selection selection) const;
	GlyphIterator lower_left (Selection selection) const;
	GlyphIterator upper_right(Selection selection) const;
	GlyphIterator lower_right(Selection selection) const;

	std::pair<GlyphIterator, GlyphIterator> upper_side_bounds(Selection selection) const;
	std::pair<GlyphIterator, GlyphIterator> lower_side_bounds(Selection selection) const;
	std::pair<GlyphIterator, GlyphIterator> left_side_bounds (Selection selection) const;
	std::pair<GlyphIterator, GlyphIterator> right_side_bounds(Selection selection) const;
	std::pair<GlyphIterator, GlyphIterator> forward_diagonal (Selection selection) const;
	std::pair<GlyphIterator, GlyphIterator> backward_diagonal(Selection selection) const;

	const Glyph*  at(int i, int j) const { return data[i][j]; }
	const Glyph*& at(int i, int j)       { return data[i][j]; }

	std::size_t rows()    const { return data.size(); }
	std::size_t columns() const { return data[0].size(); }

private:
	std::vector<std::vector<const Glyph*>> data;
};

class GlyphIterator
{
public:
	GlyphIterator(const Glyphs* glyphs, Point index) : glyphs(glyphs), index(index) {}

	bool is_within(Selection selection) const { return index.i >= selection.min.i && index.i <= selection.max.i && index.j >= selection.min.j && index.j <= selection.max.j; }

	GlyphIterator& move_up   () { index.i--; return *this; }
	GlyphIterator& move_down () { index.i++; return *this; }
	GlyphIterator& move_left () { index.j--; return *this; }
	GlyphIterator& move_right() { index.j++; return *this; }

	const Glyph* operator->() const { return glyphs->at(index.i, index.j); }

	friend bool operator==(const GlyphIterator& lhs, const GlyphIterator& rhs) { return lhs.index.i == rhs.index.i && lhs.index.j == rhs.index.j; }

private:
	const Glyphs* glyphs;
	Point index;
};

inline GlyphIterator Glyphs::upper_left (Selection selection) const { return { this, { selection.min.i, selection.min.j } }; }
inline GlyphIterator Glyphs::lower_left (Selection selection) const { return { this, { selection.max.i, selection.min.j } }; }
inline GlyphIterator Glyphs::upper_right(Selection selection) const { return { this, { selection.min.i, selection.max.j } }; }
inline GlyphIterator Glyphs::lower_right(Selection selection) const { return { this, { selection.max.i, selection.max.j } }; }

inline std::pair<GlyphIterator, GlyphIterator> Glyphs::upper_side_bounds(Selection selection) const { return { upper_left(selection), upper_right(selection) }; }
inline std::pair<GlyphIterator, GlyphIterator> Glyphs::lower_side_bounds(Selection selection) const { return { lower_left(selection), lower_right(selection) }; }
inline std::pair<GlyphIterator, GlyphIterator> Glyphs::left_side_bounds (Selection selection) const { return { upper_left(selection), lower_left(selection) }; }
inline std::pair<GlyphIterator, GlyphIterator> Glyphs::right_side_bounds(Selection selection) const { return { upper_right(selection), lower_right(selection) }; }
inline std::pair<GlyphIterator, GlyphIterator> Glyphs::forward_diagonal (Selection selection) const { return { upper_right(selection), lower_left(selection) }; }
inline std::pair<GlyphIterator, GlyphIterator> Glyphs::backward_diagonal(Selection selection) const { return { upper_left(selection), lower_right(selection) }; }
