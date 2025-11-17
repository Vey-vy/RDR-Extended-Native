# Extended Native System Documentation

## Architecture

### Native Registration

- **Hash**: All natives are identified by hash
- **Handler Type**: `void (*)(scrNativeCallContext*)`
- **Initialization**: Natives are automatically registered when `ExtendedNative::Init()` is called
- **Context**: Each native receives a `scrNativeCallContext` pointer for argument access and result setting

### FNV1a Hash Function

```cpp
constexpr uint64_t fnv1a_hash(const char* str)
```

Example hashes:
- `"READ_INT32"` → `0x...` (computed at compile-time)
- `"WRITE_FLOAT"` → `0x...` (computed at compile-time)

## Native Categories

### 1. Memory Scanner Natives

Pattern-based memory scanning for finding addresses in modules.

#### SCANNER_BEGIN
- **Hash**: `0x74997D4D86261033`
- **Arguments**: 
  - `(0)` moduleName: `const char*` - Module name (e.g., "RDR.exe", "script.dll")
- **Returns**: `bool` - Always true
- **Description**: Initializes a new scanner session, clearing previous results
- **Usage**: Call this before adding patterns

#### SCANNER_ADD_PATTERN
- **Hash**: `0xD5E272FA192490E4`
- **Arguments**:
  - `(0)` name: `const char*` - Pattern identifier
  - `(1)` pattern: `const char*` - Hex pattern string (e.g., "48 89 5C 24 ? 57")
  - `(2)` offset: `int32_t` - Offset to apply after match
  - `(3)` operationType: `int32_t` - Operation type (0=None, 1=Rip, 2=Sub, 3=Add, 4=RipAdd, 5=RipSub)
  - `(4)` operationValue: `int32_t` - Value for Add/Sub operations
- **Returns**: `bool` - Success status
- **Description**: Adds a pattern to the scanner queue
- **Operation Types**:
  - `0` (None): No operation
  - `1` (Rip): RIP-relative offset resolution
  - `2` (Sub): Subtract offset from address
  - `3` (Add): Add offset to address
  - `4` (RipAdd): RIP resolution then add offset
  - `5` (RipSub): RIP resolution then subtract offset

#### SCANNER_RUN
- **Hash**: `0x30480E1B90848DA7`
- **Arguments**:
  - `(0)` flags: `uint32_t` - Reserved for future use
- **Returns**: `bool` - Success status
- **Description**: Executes all added patterns against the target module

#### SCANNER_GET_RESULT
- **Hash**: `0xD4542B7AB11D47A6`
- **Arguments**:
  - `(0)` name: `const char*` - Pattern name
- **Returns**: `uint64_t` - Address (0 if not found)
- **Description**: Retrieves the address found for a specific pattern

#### SCANNER_SET_MODULE
- **Hash**: `0x06A513C3BEB1FC67`
- **Arguments**:
  - `(0)` moduleName: `const char*` - New module name
- **Returns**: `bool` - Success status
- **Description**: Changes the target module for scanning

#### SCANNER_CLEAR_RESULTS
- **Hash**: `0x44C5FCEF3BFF633E`
- **Arguments**: None
- **Returns**: `bool` - Always true
- **Description**: Clears all stored scan results and errors

#### SCANNER_GET_LAST_ERROR
- **Hash**: `0xD44D6107FBEC04C6`
- **Arguments**: None
- **Returns**: `const char*` - Error message
- **Description**: Returns the last error that occurred during scanning

#### SCANNER_END
- **Hash**: `0x9D76FB1B3D7BD4CB`
- **Arguments**:
  - `(0)` clearResults: `bool` - Whether to clear results on end
- **Returns**: `bool` - Always true
- **Description**: Finalizes the scanner session

#### SCANNER_COUNT_RESULTS
- **Hash**: `0x46AAECAA41FA4104`
- **Arguments**: None
- **Returns**: `int32_t` - Number of patterns found
- **Description**: Returns count of successfully found patterns

---

### 2. Memory Read Natives

Reading values from memory addresses.

#### READ_INT32
- **Hash**: `0x5B41FE069EDA2F42`
- **Arguments**:
  - `(0)` address: `uint64_t` - Memory address
- **Returns**: `int32_t` - Value at address
- **Description**: Reads a signed 32-bit integer from memory

#### READ_UINT32
- **Hash**: `0x85C4EFB7475A8243`
- **Arguments**:
  - `(0)` address: `uint64_t` - Memory address
- **Returns**: `uint32_t` - Value at address
- **Description**: Reads an unsigned 32-bit integer from memory

#### READ_UINT64
- **Hash**: `0x85CEFFB74762F1F8`
- **Arguments**:
  - `(0)` address: `uint64_t` - Memory address
- **Returns**: `uint64_t` - Value at address
- **Description**: Reads an unsigned 64-bit integer from memory

#### READ_FLOAT
- **Hash**: `0x978987E041571564`
- **Arguments**:
  - `(0)` address: `uint64_t` - Memory address
- **Returns**: `float` - Value at address
- **Description**: Reads a floating-point value from memory

#### READ_STRING
- **Hash**: `0x8677D786F81760DF`
- **Arguments**:
  - `(0)` address: `uint64_t` - Memory address
  - `(1)` maxLength: `int32_t` - Maximum characters to read
- **Returns**: `const char*` - String at address
- **Description**: Reads a null-terminated string from memory

---

### 3. Memory Write Natives

Writing values to memory addresses.

#### WRITE_INT32
- **Hash**: `0xF43B3796FADD87EB`
- **Arguments**:
  - `(0)` address: `uint64_t` - Memory address
  - `(1)` value: `int32_t` - Value to write
- **Returns**: `bool` - Success status
- **Description**: Writes a signed 32-bit integer to memory

#### WRITE_UINT32
- **Hash**: `0x6D13F5F4B44F6EA8`
- **Arguments**:
  - `(0)` address: `uint64_t` - Memory address
  - `(1)` value: `uint32_t` - Value to write
- **Returns**: `bool` - Success status
- **Description**: Writes an unsigned 32-bit integer to memory

#### WRITE_FLOAT
- **Hash**: `0xD46B8F39844BB441`
- **Arguments**:
  - `(0)` address: `uint64_t` - Memory address
  - `(1)` value: `float` - Value to write
- **Returns**: `bool` - Success status
- **Description**: Writes a floating-point value to memory

---

### 4. Module Information Natives

Retrieving module metadata.

#### GET_MODULE_BASE
- **Hash**: `0x5C72358DD41FCB30`
- **Arguments**:
  - `(0)` moduleName: `const char*` - Module name (e.g., "RDR.exe")
- **Returns**: `uint64_t` - Base address of module
- **Description**: Gets the base address of a loaded module

#### GET_MODULE_SIZE
- **Hash**: `0x338B18FD88A8A414`
- **Arguments**:
  - `(0)` moduleName: `const char*` - Module name
- **Returns**: `uint32_t` - Size of module in bytes
- **Description**: Gets the size of a loaded module

---

### 5. Vector Math Natives

3D vector calculations.

#### VECTOR_DISTANCE
- **Hash**: `0x7223ED37CC9BB780`
- **Arguments**:
  - `(0)` x1: `float` - First point X
  - `(1)` y1: `float` - First point Y
  - `(2)` z1: `float` - First point Z
  - `(3)` x2: `float` - Second point X
  - `(4)` y2: `float` - Second point Y
  - `(5)` z2: `float` - Second point Z
- **Returns**: `float` - Distance between points
- **Description**: Calculates Euclidean distance between two 3D points

#### VECTOR_LENGTH
- **Hash**: `0x8EA0A40DA049F827`
- **Arguments**:
  - `(0)` x: `float` - Vector X component
  - `(1)` y: `float` - Vector Y component
  - `(2)` z: `float` - Vector Z component
- **Returns**: `float` - Vector magnitude
- **Description**: Calculates the magnitude (length) of a 3D vector

---

### 6. Utility Natives

General utility functions.

#### LOG
- **Hash**: `0x2560B319B8E23FD1`
- **Arguments**:
  - `(0)` message: `const char*` - Message to log
- **Returns**: `bool` - Success status
- **Description**: Prints a message to console with "[ExtendedNative]" prefix
- **Output Format**: `[ExtendedNative] <message>`

#### FORMAT_STRING
- **Hash**: `0x51DFA790482861A2`
- **Arguments**:
  - `(0)` format: `const char*` - Format string
- **Returns**: `const char*` - Formatted string
- **Description**: Returns a formatted string (currently passes through the input)

---

## Data Structures

### scrNativeCallContext

The context passed to every native function, providing access to arguments and return value handling.

```cpp
class scrNativeCallContext {
    template <typename T>
    T GetArgument(uint32_t index);
    
    template <typename T>
    void SetResult(T value);
    
    uint32_t m_ArgCount;
    scrValue* m_Args;
    scrValue* m_ReturnValue;
};
```

### scrValue (Union)

Stores native argument/return values:

```cpp
union scrValue {
    int32_t Int;
    uint32_t Uns;
    float Float;
    const char* String;
    scrValue* Reference;
    uint64_t Any;
};
```

---

## Usage Example

### Using Natives.hpp Wrappers

The `Natives.hpp` header provides organized namespace wrappers with automatic logging for all natives:

```cpp
#include "Natives.hpp"

SCANNER::Begin("RDR.exe");
SCANNER::AddPattern("MyFunc", "48 89 5C 24 ? 57", 0, 0, 0);
SCANNER::Run();
uint64_t addr = SCANNER::GetResult("MyFunc");
SCANNER::End();

MEMORY::Read::Int32(addr);
MEMORY::Write::Float(addr + 4, 3.14f);

uint64_t base = MODULE::GetBase("RDR.exe");
uint32_t size = MODULE::GetSize("RDR.exe");

float distance = VECTOR::Distance(0, 0, 0, 10, 0, 0);
float length = VECTOR::Length(3, 4, 0);
```

### Pattern Scanning Example

```cpp
#include "Natives.hpp"

SCANNER::Begin("RDR.exe");

SCANNER::AddPattern("scrEngine_Register", "48 89 5C 24 ? 57 48 83 EC 20", 0, 0, 0);
SCANNER::AddPattern("RegistrationTable", "48 8B 05 ? ? ? ?", 3, 1, 0);

if (SCANNER::Run()) {
	uint64_t engineFunc = SCANNER::GetResult("scrEngine_Register");
	uint64_t regTable = SCANNER::GetResult("RegistrationTable");
	
	Logger::Success("Patterns found: " + std::to_string(SCANNER::CountResults()));
} else {
	Logger::Error(SCANNER::GetLastError());
}

SCANNER::End(true);
```

### Memory Read/Write Example

```cpp
#include "Natives.hpp"

uint64_t address = 0x140000000;

int32_t val32 = MEMORY::Read::Int32(address);
float valFloat = MEMORY::Read::Float(address + 4);
const char* str = MEMORY::Read::String(address + 8, 256);

MEMORY::Write::Int32(address, 42);
MEMORY::Write::Float(address + 4, 3.14f);
```

### Module Information Example

```cpp
#include "Natives.hpp"

uint64_t base = MODULE::GetBase("RDR.exe");
uint32_t size = MODULE::GetSize("RDR.exe");

Logger::Info("RDR.exe base: 0x" + std::to_string(base));
Logger::Info("RDR.exe size: " + std::to_string(size) + " bytes");
```

### Vector Math Example

```cpp
#include "Natives.hpp"

float dist = VECTOR::Distance(0, 0, 0, 10, 0, 0);
float mag = VECTOR::Length(3, 4, 0);

Logger::Info("Distance: " + std::to_string(dist));
Logger::Info("Magnitude: " + std::to_string(mag));
```

---

## Error Handling

All natives follow these conventions:

- **Memory operations**: Return 0/false on invalid addresses
- **Scanner operations**: Use `SCANNER_GET_LAST_ERROR` to retrieve error messages
- **String operations**: Return empty string on null input

---

## Compilation

Natives are compiled as static inline functions within the `Native.hpp` header, ensuring:

- Zero runtime overhead for inlining
- No symbol conflicts
- Compile-time optimization

The registration happens automatically during `ExtendedNative::Init()` via FNV1a hash computation.

---

## Thread Safety

**Note**: Current implementation is not thread-safe. Use appropriate synchronization when calling from multiple threads.

---

## Version Information

- **Implementation**: Static inline functions with FNV1a hashing
- **Total Natives**: 23
- **Categories**: 6 (Scanner, Read, Write, Module, Vector, Utility)
