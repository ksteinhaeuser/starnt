#if !defined io_interfaces_h_included
#define io_interfaces_h_included

#include <string>
#include "io/io.endianness.h"

namespace io {
	static const int eof = -1; // end-of-file marker

	/* An abstract readable stream type. */
	class readable {
	public:
		/* Reads up to size bytes into a buffer. Blocks until either size bytes are available or the end of the stream is reached.
		Returns the number of bytes read. A return value of zero indicates the end of the stream. */
		virtual size_t read(char* buf, size_t size) = 0;

		/* Reads one character from the stream. Returns io::eof at the end of the stream. */
		virtual int read_char() = 0;

		template<class T> T read_byte() {
			return static_cast<T>(read_char());
		}

		/* Reads a big-endian integer value. */
		template<class T> T read_big() {
			T result = 0;
			read((char*)(&result), sizeof(T));
			return io::big(result);
		}

		/* Reads a little-endian integer value. */
		template<class T> T read_little() {
			T result = 0;
			read((char*)(&result), sizeof(T));
			return io::little(result);
		}
	};

	/* An abstract readable stream type. */
	class writable {
	public:
		/* Writes size bytes from a buffer into the stream. Throws an io::exception if writing fails. */
		virtual void write(const char* buf, size_t size) = 0;

		/* Writes one character into the stream. */
		virtual void write_char(char c) = 0;

		template<class T> void write_byte(T n) {
			write_char(static_cast<char>(n));
		}

		/* Writes a string into the stream. */
		void write_string(const std::string& string) {
			write(string.c_str(), string.size());
		}

		/* Writes an integer into the stream, in big-endian order. */
		template<class T, class U> void write_big(U value) {
			T temp = io::big(static_cast<T>(value));
			write((char*)(&temp), sizeof(T));
		}

		/* Writes an integer into the stream, in little-endian order. */
		template<class T, class U> void write_little(U value) {
			T temp = io::little(static_cast<T>(value));
			write((char*)(&temp), sizeof(T));
		}

		/* Flushes the stream by writing any bytes accumulated in memory. */
		virtual void flush() {
		}
	};
}

#endif

