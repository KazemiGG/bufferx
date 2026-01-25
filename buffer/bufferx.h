#ifndef __BUFFERX_H__
#define __BUFFERX_H__
#include "pch.h"
#include <string>
#include <stdint.h>
#include <cstdint>
#include <vector>
#include <ctime>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <format>
namespace plib // project library
{
	class bufferx {
	public:
		bool auto_resize = false;
		// if true, automatically resize on writes that exceed current size
	private:
		/**
		 * Pointer to the underlying byte buffer.
		 * - Owned by this instance when allocated by the class.
		 * - May point to an external block when constructed with a raw pointer.
		 */
		uint8_t* data;
	private:
		/**
		 * Total allocated capacity (bytes) of `data`.
		 */
		uintmax_t size; 
		/**
		 * Current read/write cursor (byte offset) within `data`.
		 */
		uintmax_t pos;
	public:
		/**
		 * Default constructor - creates an empty, non-allocated bufferx.
		 */
		explicit bufferx() = default;
		/**
		 * Allocate a buffer of `size` bytes and zero-initialize it.
		 * @param size Number of bytes to allocate. If zero, no allocation is performed.
		 * @throws std::exception on allocation failure.
		 */
		explicit bufferx(uintmax_t size);
		/**
		 * Wrap an external block of memory.
		 * Does not copy the block; `bufferx` will point to `blc`.
		 * @param blc Pointer to external byte block.
		 * @param size Size of the provided block in bytes.
		 * @param pos Initial cursor position inside the block (default 0).
		 * @throws std::exception on invalid parameters.
		 */
		explicit bufferx(uint8_t* blc, uintmax_t size, uintmax_t pos = 0);
		/**
		 * Construct by reading an entire file into the buffer.
		 * The file contents are loaded into a newly allocated block owned by this instance.
		 * @param path Filesystem path to read.
		 * @throws std::exception if the file does not exist, cannot be opened, or memory allocation fails.
		 */
		explicit bufferx(const _STD string& path);
		/**
		 * Destructor. Frees owned memory (if any) and resets internal state.
		 */
		~bufferx();
	public:
		/**
		 * Get the allocated buffer capacity in bytes.
		 * @return Current buffer size (capacity).
		 */
		const uintmax_t get_size() const;
		/**
		 * Get the current read/write cursor position.
		 * @return Current position (byte offset) within the buffer.
		 */
		const uintmax_t get_pos() const;
	public:
		/**
		 * Direct access to the underlying data pointer.
		 * Note: pointer ownership semantics depend on how the instance was constructed.
		 * @return Pointer to the buffer bytes (may be nullptr).
		 */
		uint8_t* get_data() const { return data; }
	public: 
		/**
		 * Write a UTF-8 (or raw) string to the buffer prefixed by a 16-bit length.
		 * Advances the internal cursor and records a STRING detector.
		 * @param value String to write.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_string(const _STD string& value);
		/**
		 * Write a C-string to the buffer prefixed by a 16-bit length.
		 * Advances the internal cursor and records a STRING detector.
		 * @param value Null-terminated C string.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_string(const char* value);
	public: 
		/**
		 * Append an unsigned byte to the buffer and record UBYTE detector.
		 * @param value Value to append.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_ubyte(const uint8_t& value);
		/**
		 * Append an unsigned 16-bit integer and record USHORT detector.
		 * @param value Value to append.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_ushort(const uint16_t& value);
		/**
		 * Append an unsigned 32-bit integer and record UINT detector.
		 * @param value Value to append.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_uint(const uint32_t& value);
		/**
		 * Append an unsigned 64-bit integer and record ULONG detector.
		 * @param value Value to append.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_ulong(const uint64_t& value);
	public: 
		/**
		 * Append a signed byte and record BYTE detector.
		 * @param value Value to append.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_byte(const int8_t& value);
		/**
		 * Append a signed 16-bit integer and record SHORT detector.
		 * @param value Value to append.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_short(const int16_t& value);
		/**
		 * Append a signed 32-bit integer and record INT detector.
		 * @param value Value to append.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_int(const int32_t& value);
		/**
		 * Append a signed 64-bit integer and record LONG detector.
		 * @param value Value to append.
		 * @throws std::exception if there is insufficient space.
		 */
		void set_long(const int64_t& value);
		/**
		 * Append a 32-bit floating-point value (IEEE 754 single precision) to the buffer
		 * and record a FLOAT detector.
		 * - Writes the raw bytes of `value` starting at the current cursor.
		 * - Advances the internal cursor by sizeof(float).
		 * @param value Float value to append.
		 * @throws std::exception if there is insufficient space (unless auto_resize is enabled and successful).
		 */
		void set_float(const float& value);
		/**
		 * Append a 64-bit floating-point value (IEEE 754 double precision) to the buffer
		 * and record a DOUBLE detector.
		 * - Writes the raw bytes of `value` starting at the current cursor.
		 * - Advances the internal cursor by sizeof(double).
		 * @param value Double value to append.
		 * @throws std::exception if there is insufficient space (unless auto_resize is enabled and successful).
		 */
		void set_double(const double& value);
	public: 
		/**
		 * Read an unsigned byte from the current cursor and advance the cursor.
		 * @return Read value.
		 * Note: no bounds checking at the call site; ensure pos is valid before calling.
		 */
		const uint8_t get_ubyte();
		/**
		 * Read an unsigned 16-bit integer from the current cursor and advance the cursor.
		 * @return Read value.
		 */
		const uint16_t get_ushort();
		/**
		 * Read an unsigned 32-bit integer from the current cursor and advance the cursor.
		 * @return Read value.
		 */
		const uint32_t get_uint();
		/**
		 * Read an unsigned 64-bit integer from the current cursor and advance the cursor.
		 * @return Read value.
		 */
		const uint64_t get_ulong();
	public: 
		/**
		 * Read a signed byte from the current cursor and advance the cursor.
		 * @return Read value.
		 */
		const int8_t get_byte();
		/**
		 * Read a signed 16-bit integer from the current cursor and advance the cursor.
		 * @return Read value.
		 */
		const int16_t get_short();
		/**
		 * Read a signed 32-bit integer from the current cursor and advance the cursor.
		 * @return Read value.
		 */
		const int32_t get_int();
		/**
		 * Read a signed 64-bit integer from the current cursor and advance the cursor.
		 * @return Read value.
		 */
		const int64_t get_long();
		/**
		 * Read a 32-bit floating-point value (IEEE 754 single precision) from the current cursor
		 * and advance the cursor by sizeof(float).
		 * - Interprets the next bytes starting at the current cursor as an IEEE 754 single-precision float.
		 * - Advances the internal cursor by sizeof(float).
		 * @return The extracted float value.
		 * @note No bounds checking is performed by this method; ensure there are enough bytes available
		 *       (for example, by calling `is_enough(sizeof(float))`) before invoking this function.
		 */
		const float get_float();
		/**
		 * Read a 64-bit floating-point value (IEEE 754 double precision) from the current cursor
		 * and advance the cursor by sizeof(double).
		 * - Interprets the next bytes starting at the current cursor as an IEEE 754 double-precision float.
		 * - Advances the internal cursor by sizeof(double).
		 * @return The extracted double value.
		 * @note No bounds checking is performed by this method; ensure there are enough bytes available
		 *       (for example, by calling `is_enough(sizeof(double))`) before invoking this function.
		 */
		const double get_double();
	public:
		/**
		 * Read a length-prefixed (uint16_t) string starting at the current cursor and advance the cursor.
		 * @return Extracted std::string.
		 */
		const std::string get_string();
	public:
		/**
		 * Set the internal cursor position.
		 * @param pos New cursor position (byte offset).
		 */
		void set_pos(const uintmax_t& pos) { this->pos = pos; }
		/**
		 * Advance the internal cursor by `pos` bytes.
		 * @param pos Number of bytes to skip forward.
		 */
		void skip_pos(const uintmax_t& pos) { this->pos += pos; }
		/**
		 * Resize the underlying buffer to `size` bytes.
		 * Existing contents are preserved up to the new size.
		 * @param size New buffer capacity (must be > 0).
		 * @throws std::exception on invalid size or allocation failure.
		 */
		void resize(const uintmax_t& size);
		/**
		 * Determine whether `size` bytes are available from the current cursor to the end of the buffer.
		 * - Considers the instance's capacity (`this->size`) and current position (`this->pos`).
		 * - Returns `true` if `data` is non-null and `this->pos + size <= this->size`.
		 * - Does not modify object state.
		 * @param size Number of bytes required to be available starting at the current cursor.
		 * @return `true` when the requested number of bytes is available; `false` otherwise.
		 */
		bool is_enough(const uintmax_t& size);
	};
}
#endif __BUFFERX_H__
