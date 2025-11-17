#include "PatternBatch.hpp"

#include "pch.hpp"
#include "range.hpp"
#include "logger/Logger.hpp"

#include <sstream>


namespace Memory {
	void PatternBatch::add(std::string name, Pattern pattern, std::function<void(Handle)> callback) {
		m_entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
	}

	void PatternBatch::run(Range region) {
		bool all_found = true;
		for (auto& entry : m_entries) {
			if (auto result = region.scan(entry.m_pattern)) {
				if (entry.m_callback) {
					std::invoke(std::move(entry.m_callback), result);
					std::stringstream ss;
					ss << "Found '" << entry.m_name << "' at address: " << result.as<void*>();
					Logger::Success(ss.str());
				} else {
					all_found = false;
					std::stringstream ss;
					ss << "Failed to find '" << entry.m_name << "'";
					Logger::Warning(ss.str());
				}
			} else {
				all_found = false;
				std::stringstream ss;
				ss << "Failed to find '" << entry.m_name << "'";
				Logger::Warning(ss.str());
			}
		}

		m_entries.clear();
		if (!all_found) {
			throw std::runtime_error("Failed to find some patterns.");
		}
	}
}
