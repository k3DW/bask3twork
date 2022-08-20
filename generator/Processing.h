#pragma once
#include "Common.h"

using InputLines = std::vector<std::vector<std::string>>;

struct ProcessedLine
{
	int codepoint;
	int rotate_90_index;
	int rotate_180_index;
	int mirror_x_index;
	int mirror_y_index;
	int mirror_forward_diag_index;
	int mirror_backward_diag_index;
	std::string up_connection;
	std::string down_connection;
	std::string left_connection;
	std::string right_connection;
};

template <std::size_t I, class T>
requires std::same_as<ProcessedLine, std::remove_cvref_t<T>>
decltype(auto) get(T&& line)
{
	static_assert(I < std::tuple_size_v<ProcessedLine>);

	if constexpr (I == 0) return std::forward<T>(line).codepoint;
	if constexpr (I == 1) return std::forward<T>(line).rotate_90_index;
	if constexpr (I == 2) return std::forward<T>(line).rotate_180_index;
	if constexpr (I == 3) return std::forward<T>(line).mirror_x_index;
	if constexpr (I == 4) return std::forward<T>(line).mirror_y_index;
	if constexpr (I == 5) return std::forward<T>(line).mirror_forward_diag_index;
	if constexpr (I == 6) return std::forward<T>(line).mirror_backward_diag_index;
	if constexpr (I == 7) return std::forward<T>(line).up_connection;
	if constexpr (I == 8) return std::forward<T>(line).down_connection;
	if constexpr (I == 9) return std::forward<T>(line).left_connection;
	if constexpr (I == 10) return std::forward<T>(line).right_connection;
}

template<>              struct std::tuple_size<ProcessedLine>       : std::integral_constant<std::size_t, 11> {};
template<std::size_t I> struct std::tuple_element<I, ProcessedLine> : std::type_identity<std::remove_cvref_t<decltype(get<I>(std::declval<ProcessedLine>()))>> {};

using CodepointToIndex = std::map<int, int>;
using ProcessedLines   = std::vector<ProcessedLine>;
using ProcessedData    = std::pair<CodepointToIndex, ProcessedLines>;

Expected<ProcessedData> get_processed_data(std::ifstream& csv_file);
