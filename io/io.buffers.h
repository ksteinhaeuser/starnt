#if !defined io_buffered_streams_h_included
#define io_buffered_streams_h_included

#include <algorithm>
#include "basic/basic.buffer.h"
#include "basic/basic.noncopyable.h"
#include "io/io.filters.h"

namespace io {
	/* A buffered input channel. By setting up such an input channel, an application can read
	from a channel without necessarily causing a call to the source channel for each read.
	The data is read by blocks into a buffer; subsequent reads can access the data directly from the buffer. */
	class buffered_input : public io::filter_input, public basic::noncopyable {
	public:
		buffered_input(io::input& in, size_t buf_size = 1 << 16) :
		io::filter_input(in), _buf_size(buf_size), _count(0), _next(0), _buf_position(0) {
			_buf.resize(_buf_size);
		}

		~buffered_input() {
			_buf.discard();
		}

		void begin() {
			_count = 0;
			_next = 0;
			_buf_position = _source->tell();
		}

		void end() {
		}

		size_t read(char* buf, size_t size) {
			if (size == 1) {
				int c = read_char();
				buf[0] = static_cast<char>(c);
				return c == io::eof ? 0 : 1;
			}

			size_t result = std::min(_count - _next, size);
			if (result != 0) {
				char* p = &_buf[_next];
				std::copy(p, p + result, buf);
				_next += result;
				buf += result;
				size -= result;
			}
			if (size >= _buf_size / 2) {
				size_t n = _source->read(buf, size);
				_buf_position += _next + n;
				_next = 0;
				_count = 0;
				result += n;
			}
			else if (size != 0) {
				_refill();
				if (_count == 0)
					return result;
				size_t n = std::min(_count, size);
				char* p = &_buf[_next];
				std::copy(p, p + n, buf);
				_next += n;
				result += n;
			}
			return result;
		}

		int read_char() {
			if (_next >= _count) {
				_fill();
				if (_next >= _count)
					return io::eof;
			}
			return static_cast<uchar_t>(_buf[_next++]);
		}

		io::size tell() const {
			return _buf_position + _next;
		}

	protected:
		basic::buffer<char> _buf;
		const size_t _buf_size;
		size_t _count; // index one greater than the index of the last valid character in the buffer
		size_t _next; // current position in the buffer; index of the next character to be read from _buf
		io::size _buf_position; // position of the start of the buffer in the source channel

		void _refill() {
			if (_next < _count)
				std::copy(&_buf[_next], &_buf[_count], &_buf[0]);
			_count -= _next;
			_buf_position += _next;
			_next = 0;
			_count += _source->read(&_buf[_count], _buf_size - _count);
		}

		void _fill() {
			_buf_position += _next;
			_next = 0;
			_count = _source->read(&_buf[0], _buf_size);
		}
	};

	/* A buffered output channel. By setting up such an output channel, an application can write characters
	to a channel without necessarily causing a call to the sink channel for each character written.
	The data is written into a buffer; and then written to the underlying channel when the buffer
	reaches its capacity, the buffered channel is closed, or the buffered channel is explicity flushed.
	*/
	class buffered_output : public io::filter_output, public basic::noncopyable {
	public:
		buffered_output(io::output& out, size_t buf_size = 1 << 16) :
		io::filter_output(out), _buf_size(buf_size), _next(0), _buf_position(0) {
			_buf.resize(_buf_size);
		}

		~buffered_output() {
			if (_next != 0)
				glob_hint("buffered output has unflushed data");
			_buf.discard();
		}

		void begin() {
			_next = 0;
			_buf_position = _sink->tell();
		}

		void end() {
			flush();
		}

		void write(const char* buf, size_t size) {
			if (size == 1) {
				write_char(buf[0]);
				return;
			}

			if (size >= _buf_size) {
				// when requested size exceeds buffer size, write data directly; this way, buffered channels will cascade harmlessly
				flush();
				_sink->write(buf, size);
				_buf_position += size;
			}
			else {
				if (size > _buf_size - _next)
					flush();
				std::copy(&buf[0], &buf[size], &_buf[_next]);
				_next += size;
			}
		}

		void write_char(char c) {
			if (_next == _buf_size)
				flush();
			_buf[_next++] = c;
		}

		void flush() {
			if (_next != 0) {
				_sink->write(&_buf[0], _next);
				_buf_position += _next;
				_next = 0;
			}
		}

		io::size tell() const {
			return _buf_position + _next;
		}

	protected:
		basic::buffer<char> _buf;
		const size_t _buf_size;
		size_t _next; // current position in the buffer; equals the number of valid characters in the buffer
		io::size _buf_position; // position of the start of the buffer relative to the start of the sink channel
	};
}

#endif

