#include "pch.h"
#include "pure/Glyph.h"

static bool compatible(Connections known, Connections checking)
{
	return (known.up    == Connection::DO_NOT_CARE || known.up    == checking.up)
	    && (known.down  == Connection::DO_NOT_CARE || known.down  == checking.down)
	    && (known.left  == Connection::DO_NOT_CARE || known.left  == checking.left)
	    && (known.right == Connection::DO_NOT_CARE || known.right == checking.right);
}

const Glyph* Glyph::Random(Connections connections, GlyphFlag flags)
/// This function takes in the desired flags and outputs the vector of all glyphs which meet the criteria.
/// 
/// \param connections The \c Connections required. If any connection should be disregarded, then pass \c Connection::DO_NOT_CARE.
/// \param flags The bit flags required for this \c Glyph. Any bits with a value of \c 0 are ignored, and any bits with a value of \c 1 are required.
/// \return A pointer to a randomly selected \c Glyph that fits the criteria, or \c nullptr if nothing exists.
{
	static std::vector<const Glyph*> glyph_list(AllGlyphs.size(), nullptr);
	glyph_list.clear();

	RandomList(glyph_list, connections, flags);

	if (glyph_list.empty())
		return nullptr;
	else
		return glyph_list.front();
}

std::vector<const Glyph*> Glyph::RandomList(Connections connections, GlyphFlag flags)
{
	std::vector<const Glyph*> glyphs;
	RandomList(glyphs, connections, flags);
	return glyphs;
}

void Glyph::RandomList(std::vector<const Glyph*>& glyphs, Connections connections, GlyphFlag flags)
{
	for (auto& glyph : AllGlyphs)
		if (compatible(connections, glyph) && (glyph.flags % flags))
			glyphs.push_back(&glyph);

	static std::mt19937 twister{ std::random_device{}() };
	std::ranges::shuffle(glyphs, twister);
}
