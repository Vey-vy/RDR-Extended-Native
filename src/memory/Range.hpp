#pragma once
#include "fwddec.hpp"
#include "handle.hpp"

#include <vector>

namespace Memory {
	class Range {
	public:
		Range(Handle base, std::size_t size);

		Handle begin();
		Handle end();
		std::size_t size();

		bool contains(Handle h);

		Handle scan(Pattern const& sig);
		std::vector<Handle> scan_all(Pattern const& sig);

	protected:
		Handle m_base;
		std::size_t m_size;
	};
}
