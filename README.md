# buffer (bufferx)

Small C++ helper library providing a byte buffer wrapper, `plib::bufferx`, for reading and writing primitive values and length-prefixed strings.

Overview
- Implements a read/write cursor and helpers to serialize/deserialize primitive types: `set_ubyte`, `set_ushort`, `set_uint`, `set_ulong`, `set_byte`, `set_short`, `set_int`, `set_long`, `set_float`, `set_double`, `set_string` and the corresponding `get_*` accessors.
- Supports wrapping external memory or owning an allocated block.
- Optional `auto_resize` to grow buffer on writes.

Repository layout
- `bufferx.h` — public header declaring `plib::bufferx`.
- `bufferx.cpp` — implementation.
- `buffer.vcxproj` / Visual Studio project files.

Requirements
- C++17 or newer. Note: `<format>` usage may require C++20 on some compilers — set the appropriate language standard in your build settings.
- Visual Studio (project provided).

Build
1. Open the solution or `buffer.vcxproj` in Visual Studio.
2. Select the appropriate C++ language standard under __Project Properties > C/C++ > Language__ if needed.
3. Build the project.

Important note about precompiled headers
- The header currently contains an include of `pch.h`. If your project does **not** use precompiled headers, remove or comment out the line:
  - `#include "pch.h"`
  - from `bufferx.h` before building. If your project uses a different precompiled header, adapt or remove that include accordingly.

Behavior notes for floating-point accessors
- `get_float()`:
  - Reads an IEEE 754 single-precision (4-byte) value starting at the current cursor.
  - Advances the internal cursor by `sizeof(float)` (4 bytes).
  - Does not perform bounds checks; call `is_enough(sizeof(float))` before reading to ensure the buffer has sufficient bytes.
- `get_double()`:
  - Reads an IEEE 754 double-precision (8-byte) value starting at the current cursor.
  - Advances the internal cursor by `sizeof(double)` (8 bytes).
  - Does not perform bounds checks; call `is_enough(sizeof(double))` before reading to ensure the buffer has sufficient bytes.

Usage example
- Minimal example showing write/read of `float`, `double` and a length-prefixed `string`. Always ensure reads are safe by using `is_enough()`.

