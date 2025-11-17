#pragma once
#include "fwddec.hpp"
#include "handle.hpp"

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace Memory {
	class Pattern {
		friend PatternBatch;
		friend Range;

	public:
		Pattern(std::string_view ida_sig);
		explicit Pattern(const void* bytes, std::string_view mask);

		inline Pattern(const char* ida_sig) :
		    Pattern(std::string_view(ida_sig)) {
		}

	private:
		std::vector<std::optional<std::uint8_t>> m_bytes;
	};
}
