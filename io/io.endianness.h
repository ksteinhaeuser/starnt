#if !defined io_endianness_h_included
#define io_endianness_h_included

#include <algorithm>
#include "glob/glob.endianness.h"
#include "glob/glob.stdint.h"
#include "glob/glob.warnings.h"

namespace io {
	/* Inverts byte order, returns the inverted value. */
	template<class T> T flip(const T value) {
		T result = value;
		char* p = reinterpret_cast<char*>(&result);
		std::reverse(p, p + sizeof(T));
		return result;
	}

	/* More efficient versions of io::flip for common integer sizes. */
	static uint16_t flip16(uint16_t n) {
		return (n << 8) | (n >> 8);
	}

	static uint32_t flip32(uint32_t n) {
		return (n << 24) |
			((n << 8) & static_cast<uint32_t>(0x00ff0000)) |
			((n >> 8) & static_cast<uint32_t>(0x0000ff00)) |
			(n >> 24);
	}

	static uint64_t flip64(uint64_t n) {
		return (n << 56) |
			((n << 40) & static_cast<uint64_t>(0x00ff000000000000ull)) |
			((n << 24) & static_cast<uint64_t>(0x0000ff0000000000ull)) |
			((n << 8) & static_cast<uint64_t>(0x000000ff00000000ull)) |
			((n >> 8) & static_cast<uint64_t>(0x00000000ff000000ull)) |
			((n >> 24) & static_cast<uint64_t>(0x0000000000ff0000ull)) |
			((n >> 40) & static_cast<uint64_t>(0x000000000000ff00ull)) |
			(n >> 56);
	}

	/* Converts a value to big-endian byte order. */
	template<class T> T big(const T value) {
		return glob_endianness_big ? value : flip(value);
	}

	/* More efficient versions of io::big for common integer sizes. */
	static uint16_t big16(uint16_t value) {
		return glob_endianness_big ? value : flip16(value);
	}

	static uint32_t big32(uint32_t value) {
		return glob_endianness_big ? value : flip32(value);
	}

	static uint64_t big64(uint64_t value) {
		return glob_endianness_big ? value : flip64(value);
	}

	/* Converts a value to little-endian byte order. */
	template<class T> T little(const T value) {
		return glob_endianness_little ? value : io::flip(value);
	}

	/* More efficient versions of io::little for common integer sizes. */
	static uint16_t little16(uint16_t value) {
		return glob_endianness_little ? value : flip16(value);
	}

	static uint32_t little32(uint32_t value) {
		return glob_endianness_little ? value : flip32(value);
	}

	static uint64_t little64(uint64_t value) {
		return glob_endianness_little ? value : flip64(value);
	}

}

#endif

