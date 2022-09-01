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

	for (auto& glyph : AllGlyphs)
		if (compatible(connections, glyph) && (glyph.flags % flags))
			glyph_list.push_back(&glyph);

	if (glyph_list.empty())
		return nullptr;
	else
	{
		static std::mt19937 twister{ std::random_device{}() };
		static const Glyph* glyph = nullptr;
		std::ranges::sample(glyph_list, &glyph, 1, twister);
		return glyph;
	}
}
