#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>

class Logger {
public:
	enum class Level {
		Info,
		Warning,
		Error,
		Success
	};

	static void Log(Level level, const std::string& message) {
		switch (level) {
		case Level::Info:
			std::cout << "[INFO] " << message << "\n";
			break;
		case Level::Warning:
			std::cout << "[WARNING] " << message << "\n";
			break;
		case Level::Error:
			std::cerr << "[ERROR] " << message << "\n";
			break;
		case Level::Success:
			std::cout << "[SUCCESS] " << message << "\n";
			break;
		}
	}

	static void Info(const std::string& message) { Log(Level::Info, message); }
	static void Warning(const std::string& message) { Log(Level::Warning, message); }
	static void Error(const std::string& message) { Log(Level::Error, message); }
	static void Success(const std::string& message) { Log(Level::Success, message); }
};
