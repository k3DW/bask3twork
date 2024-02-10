#pragma once
#include "pure/GridSize.h"
#include "pure/Selection.h"
#include "pure/SelectionIterator.h"
#include <vector>



template <class T>
class Grid
{
public:
	Grid(GridSize grid_size)
		: grid_size(grid_size)
		, cap(grid_size.area())
	{
		data.reserve(cap);
	}

	Grid(GridSize grid_size, const T& t)
		: Grid(grid_size)
	{
		data.assign(cap, t);
	}

	const T& at(Point p) const { return data.at(p.i * grid_size.rows + p.j); }
	T& at(Point p) { return data.at(p.i * grid_size.rows + p.j); }

	const T& operator[](Point p) const { return at(p); }
	T& operator[](Point p) { return at(p); }

private:
	template <bool is_const>
	class row_proxy
	{
		friend class Grid<T>;
		using grid_ptr = std::conditional_t<is_const, const Grid*, Grid*>;
		row_proxy(grid_ptr ptr, int y)
			: grid(ptr), y(y) {}

		std::conditional_t<is_const, const Grid*, Grid*> grid;
		int y;

	public:
		auto& operator[](int x) const { return grid->at({ x, y }); }
	};

public:
	row_proxy<true> operator[](int y) const { return row_proxy<true>{ this, y }; }
	row_proxy<false> operator[](int y) { return row_proxy<false>{ this, y }; }

	void push_back(const T& t) { if (data.size() == cap) throw std::logic_error("Trying to push_back past grid limit"); data.push_back(t); }
	void push_back(T&& t) { if (data.size() == cap) throw std::logic_error("Trying to push_back past grid limit"); data.push_back(std::move(t)); }
	template <class... Args>
	auto emplace_back(Args&&... args) { if (data.size() == cap) throw std::logic_error("Trying to emplace_back past grid limit"); return data.emplace_back(std::forward<Args>(args)...); }

	GridSize size() const { return grid_size; }
	std::size_t capacity() const { return cap; }

	GridRange<T, true> iterate(Selection selection, CornerMovement type) const;
	GridRange<T, false> iterate(Selection selection, CornerMovement type);

private:
	std::vector<T> data;
	GridSize grid_size;
	std::size_t cap;
};

template <class T, bool is_const>
class GridIterator
{
	using Self = GridIterator;

public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t; // Unused
	using value_type = T;
	using pointer    = std::conditional_t<is_const, const T*, T*>;
	using reference  = std::conditional_t<is_const, const T&, T&>;
	using grid_type  = std::conditional_t<is_const, const Grid<T>, Grid<T>>;

	GridIterator() = default;

	GridIterator(grid_type& grid, Selection selection, CornerMovement type)
		: grid(&grid)
		, it(selection, type)
	{
		static_assert(std::forward_iterator<Self>);
		static_assert(!std::bidirectional_iterator<Self>);
	}

	friend bool operator==(const Self& lhs, const Self& rhs)
	{
		if (lhs.grid != rhs.grid)
			throw std::logic_error("Trying to compare iterators into 2 different grid ranges.");
		return lhs.it == rhs.it;
	}

	bool operator==(SelectionSentinel) const
	{
		return it == SelectionSentinel{};
	}

	reference operator*() const
	{
		return grid->at(*it);
	}

	Point position() const
	{
		return *it;
	}

	Self& operator++()
	{
		++it;
		return *this;
	}

	Self operator++(int)
	{
		auto temp = *this;
		++(*this);
		return temp;
	}

private:
	grid_type* const grid;
	SelectionIterator it;
};

template <class T, bool is_const>
class GridRange
{
public:
	using grid_type = std::conditional_t<is_const, const Grid<T>, Grid<T>>;

	GridRange(grid_type& grid, Selection selection, CornerMovement type)
		: grid(&grid), selection(selection), type(type)
	{
		static_assert(std::ranges::forward_range<GridRange>);
		static_assert(!std::ranges::bidirectional_range<GridRange>);
	}

	auto begin() const { return GridIterator<T, is_const>{ *grid, selection, type }; }
	auto end()   const { return SelectionSentinel{}; }

private:
	grid_type* const grid;
	Selection selection;
	CornerMovement type;
};

template <class T>
GridRange<T, true> Grid<T>::iterate(Selection selection, CornerMovement type) const
{
	return { *this, selection, type };
}

template <class T>
GridRange<T, false> Grid<T>::iterate(Selection selection, CornerMovement type)
{
	return { *this, selection, type };
}
