#pragma once
#include "pure/SelectionIterator.h"
#include "pure/Selection.h"

class SelectionZipIterator
{
	using Self = SelectionZipIterator;

public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t; // Unused
	using value_type = std::pair<const Point&, const Point&>;
	using pointer    = std::pair<const Point&, const Point&>;
	using reference  = std::pair<const Point&, const Point&>;

	SelectionZipIterator() = default;

	SelectionZipIterator(Selection selection1, CornerMovement type1, Selection selection2, CornerMovement type2)
		: it1(selection1, type1)
		, it2(selection2, type2)
	{
		static_assert(std::forward_iterator<Self>);
		static_assert(!std::bidirectional_iterator<Self>);
	}

	friend bool operator==(const Self& lhs, const Self& rhs) = default;

	bool operator==(SelectionSentinel) const
	{
		return (it1 == SelectionSentinel{}) && (it2 == SelectionSentinel{});
	}

	reference operator*() const
	{
		return { *it1, *it2 };
	}

	Self& operator++()
	{
		++it1;
		++it2;
		return *this;
	}

	Self operator++(int)
	{
		auto temp = *this;
		++(*this);
		return temp;
	}

private:
	SelectionIterator it1;
	SelectionIterator it2;
};



class SelectionZipRange
{
public:
	SelectionZipRange(Selection selection1, CornerMovement type1, Selection selection2, CornerMovement type2)
		:  selection1(selection1), type1(type1), selection2(selection2), type2(type2)
	{
		static_assert(std::ranges::forward_range<SelectionZipRange>);
		static_assert(!std::ranges::bidirectional_range<SelectionZipRange>);
	}

	SelectionZipRange(Selection selection, CornerMovement type1, CornerMovement type2)
		: SelectionZipRange(selection, type1, selection, type2)
	{}

	auto begin() const { return SelectionZipIterator{ selection1, type1, selection2, type2 }; }
	auto end()   const { return SelectionSentinel{}; }

private:
	Selection selection1;
	CornerMovement type1;
	Selection selection2;
	CornerMovement type2;
};
