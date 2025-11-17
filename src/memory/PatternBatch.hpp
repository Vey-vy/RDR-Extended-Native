#pragma once
#include "pattern.hpp"
#include "pch.hpp"

#include <functional>
#include <vector>

namespace Memory {
	class PatternBatch {
	public:
		explicit PatternBatch()  = default;
		~PatternBatch() noexcept = default;

		void add(std::string name, Pattern pattern, std::function<void(Memory::Handle)> callback);
		void run(Range region);

		struct Entry {
			std::string m_name;
			Pattern m_pattern;
			std::function<void(Memory::Handle)> m_callback;

			explicit Entry(std::string name, Pattern pattern, std::function<void(Memory::Handle)> callback) :
			    m_name(std::move(name)), m_pattern(std::move(pattern)), m_callback(std::move(callback)) {
			}
		};

	private:
		std::vector<Entry> m_entries;
	};
}
