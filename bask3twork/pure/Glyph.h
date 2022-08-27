#pragma once
#include "pure/Connection.h"
#include "pure/Enum.h"
#include "Forward.h"
#include <map>
#include <random>
#include <vector>
/// \file

/// The bit flag for each of the properties of a \c Glyph object
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

/// A struct for each of the unit transformations on a \c Glyph, to keep the code inside the \c Glyph struct cleaner
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
struct Glyph : GlyphsTransformed
{
	int32_t code_point; ///< The character that gets displayed
	
	Connection up;
	Connection down;
	Connection left;
	Connection right;
	operator Connections() const { return { up, down, left, right }; }

	GlyphFlag flags; ///< The total signature of this glyph
	consteval GlyphFlag get_flags() const;

	/// All the \c connectTo____ flags are determined from the other parameters, but the other parameters are given explicitly
	consteval Glyph(int32_t code_point, GlyphsTransformed transformed, Connection up, Connection down, Connection left, Connection right) :
		code_point(code_point), GlyphsTransformed(transformed), up(up), down(down), left(left), right(right)
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
		| (GlyphFlag::SA_ROT4 * (this == rotate_90))
		| (GlyphFlag::SA_ROT2 * (this == rotate_180))
		| (GlyphFlag::SA_MIRX * (this == mirror_x))
		| (GlyphFlag::SA_MIRY * (this == mirror_y))
		| (GlyphFlag::CT_ROT4U * (up == ::rotate_90(right)))
		| (GlyphFlag::CT_ROT4D * (down == ::rotate_90(left)))
		| (GlyphFlag::CT_ROT4L * (left == ::rotate_90(up)))
		| (GlyphFlag::CT_ROT4R * (right == ::rotate_90(down)))
		| (GlyphFlag::CT_ROT2U * (up == ::rotate_180(up)))
		| (GlyphFlag::CT_ROT2D * (down == ::rotate_180(down)))
		| (GlyphFlag::CT_ROT2L * (left == ::rotate_180(left)))
		| (GlyphFlag::CT_ROT2R * (right == ::rotate_180(right)))
		| (GlyphFlag::CT_MIRU * (up == ::mirror_x(up)))
		| (GlyphFlag::CT_MIRD * (down == ::mirror_x(down)))
		| (GlyphFlag::CT_MIRL * (left == ::mirror_y(left)))
		| (GlyphFlag::CT_MIRR * (right == ::mirror_y(right)))
		| (GlyphFlag::SA_MIRFD * (this == mirror_forward_diagonal))
		| (GlyphFlag::SA_MIRBD * (this == mirror_backward_diagonal))
		| (GlyphFlag::CT_SELFU * (up == down))
		| (GlyphFlag::CT_SELFD * (up == down))
		| (GlyphFlag::CT_SELFL * (left == right))
		| (GlyphFlag::CT_SELFR * (left == right));
}


/// The constexpr array of every \c Glyph in the program, the only place where a Glyph object is initialized;
/// every other place a \c Glyph is referenced in the whole codebase is actually a pointer to a \c Glyph in \c AllGlyphs.
#include "generated/AllGlyphs.impl"

/// The mapping from the unicode character to the Glyph that uses that character, used for reading knots.
#include "generated/UnicharToGlyph.impl"

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
