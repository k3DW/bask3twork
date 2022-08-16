#pragma once
#include "Constants.h"
#include "Connection.h"
#include "Enum.h"
/// \file

/// The bit flag for each of the properties of a Glyph object
enum class GlyphFlag
{
	NONE	 = 0,       ///< No flag
	SA_ROT4  = 1 <<  0, ///< Is this Glyph the same after rotating by 90 degrees
	SA_ROT2  = 1 <<  1, ///< Is this Glyph the same after rotating by 180 degrees
	SA_MIRX  = 1 <<  2, ///< Is this Glyph the same after mirroring across the horizontal line
	SA_MIRY  = 1 <<  3, ///< Is this Glyph the same after mirroring across the vertical line
	CT_ROT4U = 1 <<  4, ///< Can this Glyph connect to its 90-degree-rotated counterpart on its own upper side
	CT_ROT4D = 1 <<  5, ///< Can this Glyph connect to its 90-degree-rotated counterpart on its own lower side
	CT_ROT4L = 1 <<  6, ///< Can this Glyph connect to its 90-degree-rotated counterpart on its own left side
	CT_ROT4R = 1 <<  7, ///< Can this Glyph connect to its 90-degree-rotated counterpart on its own right side
	CT_ROT2U = 1 <<  8, ///< Can this Glyph connect to its 180-degree-rotated counterpart on its own upper side
	CT_ROT2D = 1 <<  9, ///< Can this Glyph connect to its 180-degree-rotated counterpart on its own lower side
	CT_ROT2L = 1 << 10, ///< Can this Glyph connect to its 180-degree-rotated counterpart on its own left side
	CT_ROT2R = 1 << 11, ///< Can this Glyph connect to its 180-degree-rotated counterpart on its own right side
	CT_MIRU  = 1 << 12, ///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its upper side
	CT_MIRD  = 1 << 13, ///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its lower side
	CT_MIRL  = 1 << 14, ///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its left side
	CT_MIRR  = 1 << 15, ///< Can this Glyph connect to its mirrored counterpart if it is mirrored across its right side
	SA_MIRFD = 1 << 16, ///< Is this Glyph the same after mirroring across the forward diagonal
	SA_MIRBD = 1 << 17, ///< Is this Glyph the same after mirroring across the backward diagonal
	CT_SELFU = 1 << 18, ///< Can this Glyph connect to itself on the upper side of the Glyph
	CT_SELFD = 1 << 19, ///< Can this Glyph connect to itself on the lower side of the Glyph
	CT_SELFL = 1 << 20, ///< Can this Glyph connect to itself on the left side of the Glyph
	CT_SELFR = 1 << 21, ///< Can this Glyph connect to itself on the right side of the Glyph
};

template <> struct opt_into_enum_operations<GlyphFlag> : std::true_type {};

struct Glyph;
using Glyphs = std::vector<std::vector<const Glyph*>>;

struct GlyphsTransformed
{
	const Glyph* rotate_90;  ///< This Glyph rotated by 90 degrees clockwise
	const Glyph* rotate_180; ///< This Glyph rotated by 180 degrees clockwise
	const Glyph* mirror_x;   ///< This Glyph mirrored across its horizontal line
	const Glyph* mirror_y;   ///< This Glyph mirrored across its veritcal line
	const Glyph* mirror_forward_diagonal;  ///< This Glyph mirrored across its forward diagonal line
	const Glyph* mirror_backward_diagonal; ///< This Glyph mirrored across its backward diagonal line
};

/// A struct to store the glyph information of the the Celtic Knots font
struct Glyph
{
	wxUniChar::value_type code_point; ///< The character that gets displayed
	wxUniChar get() const { return wxUniChar(code_point); }

	GlyphsTransformed transformed;

	Connection up;
	Connection down;
	Connection left;
	Connection right;
	operator Connections() const { return { up, down, left, right }; }

	GlyphFlag flags; ///< The total signature of this glyph
	consteval GlyphFlag get_flags() const;

	/// All the \c connectTo____ flags are determined from the other parameters, but the other parameters are given explicitly
	consteval Glyph(wxUniChar::value_type code_point, GlyphsTransformed transformed, Connection up, Connection down, Connection left, Connection right) :
		code_point(code_point), transformed(transformed), up(up), down(down), left(left), right(right)
	{
		flags = get_flags();
	}

	Glyph(const Glyph&) = delete;
	Glyph(Glyph&&) = delete;
	Glyph& operator=(const Glyph&) = delete;
	Glyph& operator=(Glyph&&) = delete;
};

consteval GlyphFlag Glyph::get_flags() const
{
	return GlyphFlag::NONE
		| (GlyphFlag::SA_ROT4 * (this == transformed.rotate_90))
		| (GlyphFlag::SA_ROT2 * (this == transformed.rotate_180))
		| (GlyphFlag::SA_MIRX * (this == transformed.mirror_x))
		| (GlyphFlag::SA_MIRY * (this == transformed.mirror_y))
		| (GlyphFlag::CT_ROT4U * (up == rotate_90(right)))
		| (GlyphFlag::CT_ROT4D * (down == rotate_90(left)))
		| (GlyphFlag::CT_ROT4L * (left == rotate_90(up)))
		| (GlyphFlag::CT_ROT4R * (right == rotate_90(down)))
		| (GlyphFlag::CT_ROT2U * (up == rotate_180(up)))
		| (GlyphFlag::CT_ROT2D * (down == rotate_180(down)))
		| (GlyphFlag::CT_ROT2L * (left == rotate_180(left)))
		| (GlyphFlag::CT_ROT2R * (right == rotate_180(right)))
		| (GlyphFlag::CT_MIRU * (up == mirror_x(up)))
		| (GlyphFlag::CT_MIRD * (down == mirror_x(down)))
		| (GlyphFlag::CT_MIRL * (left == mirror_y(left)))
		| (GlyphFlag::CT_MIRR * (right == mirror_y(right)))
		| (GlyphFlag::SA_MIRFD * (this == transformed.mirror_forward_diagonal))
		| (GlyphFlag::SA_MIRBD * (this == transformed.mirror_backward_diagonal))
		| (GlyphFlag::CT_SELFU * (up == down))
		| (GlyphFlag::CT_SELFD * (up == down))
		| (GlyphFlag::CT_SELFL * (left == right))
		| (GlyphFlag::CT_SELFR * (left == right));
}


/// The array of every Glyph in the program (generated in Excel), the only place where a Glyph object is initialized;
/// every other place a Glyph is referenced in the whole codebase is actually a pointer to one of the Glyphs in \c AllGlyphs.
#include "generated\AllGlyphs.impl"

/// The mapping from the unicode character to the Glyph that uses that character, used for reading knots.
#include "generated\UnicharToGlyph.impl"

/// The default Glyph to fill the Knot upon initialization, which is set as the \c space character, \c \x20
constexpr const Glyph* DefaultGlyph = &AllGlyphs[0];

constexpr bool compatible(Connections known, Connections checking)
{
	return (known.up    == Connection::DO_NOT_CARE || known.up    == checking.up)
	    && (known.down  == Connection::DO_NOT_CARE || known.down  == checking.down)
	    && (known.left  == Connection::DO_NOT_CARE || known.left  == checking.left)
	    && (known.right == Connection::DO_NOT_CARE || known.right == checking.right);
}

inline const Glyph* RandomGlyph(const Connections connections, const GlyphFlag flags)
/// This function takes in the desired flags and outputs the vector of all glyphs which meet the criteria.
{
	/// \param connections The \c Connections desired. If any connection does not matter, then pass \c Connection::DO_NOT_CARE.
	/// \param flags The other condition flags to check for the glyphs, passed by using \c operator| on \c GlyphFlag values. Any bits with a value of \c 0 are ignored

	/// \b Method

	/// This function allows all properties of the Glyphs to be checked at once, instead of checking multiple properties in order.
	/// Using this function allows for high speed Glyph selection, since all attributes are assessed simultaneously.

	/// Loop through \c AllGlyphs.
	/// If the Glyph has compatible connections and it has all the needed flags, then add the pointer to this Glyph to the output vector.
	std::vector<const Glyph*> glyphList;
	for (const Glyph& glyph : AllGlyphs)
		if (compatible(connections, glyph) && (glyph.flags % flags))
			glyphList.push_back(&glyph);

	if (glyphList.empty())
		return nullptr;
	else
	{
		static std::mt19937 twister{ std::random_device{}() };
		static const Glyph* glyph = nullptr;
		std::sample(glyphList.begin(), glyphList.end(), &glyph, 1, twister);
		return glyph;
	}
}
