#if !defined io_streams_h_included
#define io_streams_h_included

#include "glob/glob.hint.h"
#include "glob/glob.unused.h"
#include "io/io.interfaces.h"

namespace io {
	typedef uint64_t size; // size type

	static const io::size no_size = static_cast<io::size>(-1); // unknown size
	static const io::size max_size = static_cast<io::size>(-1); // maximum size

	/* An input stream. */
	class input : public io::readable {
	public:
		input() :
		_position(0) {
		}

		virtual ~input() {
		}

		virtual void begin() {
		}

		virtual void end() {
		}

		virtual size_t read(char* buf, size_t size) {
			glob_unused(buf);
			size_t result = 0;
			if (size == 0)
				return result;
			glob_hint("calling an unimplemented read");
			return result;
		}

		/* Reads one character from the stream. Returns io::eof at the end of the stream. */
		virtual int read_char() {
			char c;
			return read(&c, 1) == 1 ? static_cast<uchar_t>(c) : io::eof;
		}

		/* Returns the position of the stream, i.e., the distance (in characters) from the beginning of the stream. */
		virtual io::size tell() const {
			return _position;
		}

	protected:
		io::size _position;
	};

	/* An output stream. */
	class output : public io::writable { //extends abstract class writable 
	public:
		output() :
		_position(0) {
		}

		virtual ~output() {
		}

		virtual void begin() {
		}

		virtual void end() {
		}

		/* Writes size characters from a buffer into the stream. Throws a io::exception if writing fails. */
		virtual void write(const char* buf, size_t size) {
			glob_unused(buf);
			glob_unused(size);
			glob_hint("calling an unimplemented write");
		}

		/* Writes one character into the stream. */
		virtual void write_char(char c) {
			write(&c, 1);
		}

		template<class T> void write_byte(T n) {
			write_char(static_cast<char>(n));
		}

		/* Writes a string into the stream. */
		void write_string(const std::string& string) {
			write(string.c_str(), string.size());
		}

		/* Flushes the stream by writing any characters accumulated in memory. */
		virtual void flush() {
		}

		/* Returns the position of the stream, i.e., the distance (in characters) from the beginning of the stream. */
		virtual io::size tell() const {
			return _position;
		}

	protected:
		io::size _position;
	};
}

#endif

