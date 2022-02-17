
#pragma once

#include "utils.hpp"
#include <fmt/format.h>

namespace ez_paint::log
{
	template <typename...Args> concept RequireMinArgs = requires (std::size_t min){ sizeof...(Args) > min; };
	
	static void vlog(fmt::string_view format, fmt::format_args args){
		fmt::vprint(format, args);
	}

	template <typename S, typename...Args>
	static constexpr void info(const S& format, Args&&...args){
		// static_assert(sizeof...(Args) > 1, "Function requires at least one argument");
		vlog(
			fmt::format("[INFO {}] - {}\n", utils::timestamp(), format),
			fmt::make_args_checked<Args...>(format, args...)
		);
	}

	template <typename S, typename...Args>
	static constexpr void error(const S& format, Args&&...args){
		vlog(
			fmt::format("[ERROR {}] - {}\n", utils::timestamp(), format),
			fmt::make_args_checked<Args...>(format, args...)
		);
	}

	template <typename S, typename...Args>
	static constexpr void debug(const S& format, Args&&...args){
		vlog(
			fmt::format("[DEBUG {}] - {}\n", utils::timestamp(), format),
			fmt::make_args_checked<Args...>(format, args...)
		);
	}

}