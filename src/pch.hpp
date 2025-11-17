#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>
#include <windows.h>


#include "memory/Handle.hpp"
#include "memory/Module.hpp"
#include "memory/PatternBatch.hpp"

#define HEX_TO_UPPER(value) \
	"0x" << std::hex << std::uppercase << (DWORD64)value << std::dec << std::nouppercase