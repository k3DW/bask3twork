#pragma once
#include <type_traits>

template <class T>
struct opt_into_enum_operations : std::false_type {};

template <class T>
concept UsableEnum = opt_into_enum_operations<T>::value;

template <UsableEnum E>
constexpr std::underlying_type_t<E> to_underlying(E e)
{
	return static_cast<std::underlying_type_t<E>>(e);
}

template <UsableEnum E> constexpr E operator&(E lhs, E rhs) { return static_cast<E>(to_underlying(lhs) & to_underlying(rhs)); }
template <UsableEnum E> constexpr E operator|(E lhs, E rhs) { return static_cast<E>(to_underlying(lhs) | to_underlying(rhs)); }

template <UsableEnum E> constexpr E operator*(E sym, bool b) { return static_cast<E>(to_underlying(sym) * b); }
template <UsableEnum E> constexpr E operator*(bool b, E sym) { return static_cast<E>(to_underlying(sym) * b); }

template <UsableEnum E> constexpr bool operator%(E lhs, E rhs) { return (lhs & rhs) == rhs; }
