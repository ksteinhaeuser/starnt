#if !defined io_bit_streams_h_included
#define io_bit_streams_h_included

#include "io/io.filters.h"

namespace io {
	/* Reads bits from a byte-based input channel, least-significant or most-significant bit first, using an int buffer.
	On a machine with 32-bit ints, it can read up to 24 bits at a time. It never reads from its source farther than
	the nearest byte boundary past the peeked at or consumed bits.
	*/
	template<bool lsb> class bit_input : public io::filter_input {
	public:
		bit_input(io::input& in) :
		io::filter_input(in), _buf(0), _bits(0) {
		}

		void begin() {
			_buf = 0;
			_bits = 0;
		}

		void end() {
			skip_bits(_bits % 8);
			rt_assert(_bits % 8 == 0);
		}

		template<class T> T read_bit() {
			return read_bits<T>(1);
		}

		template<class T> T read_bits(int bits) {
			T result = peek_bits<T>(bits);
			skip_bits(bits);
			return result;
		}

		template<class T> T large_read_bits(int bits) {
			T result = 0;
			int pos = 0;
			while (bits > 0) {
				int i = std::min(bits, _max_bits);
				T n = peek_bits<T>(i);
				result = lsb ? (result | (n << pos)) : ((result << pos) | n);
				skip_bits(i);
				bits -= i;
				pos += i;
			}
			return result;
		}

		template<class T> T peek_bits(int bits) {
			if (!_refill(bits))
				return static_cast<T>(io::eof);
			return static_cast<T>((lsb ? _buf : (_buf >> (_bits - bits))) & ((1 << bits) - 1));
		}

		template<class T> T inverted_peek_bits(int bits) {
			if (!_refill(bits))
				return static_cast<T>(io::eof);
			return static_cast<T>((lsb ? ~_buf : ~(_buf >> (_bits - bits))) & ((1 << bits) - 1));
		}

		void skip_bits(int bits) {
			if (bits > _bits)
				_refill(bits); // debug_hint("skipping more bits than available");
			if (lsb)
				_buf >>= bits;
			_bits -= bits;
		}

		void align() {
			if (_bits > 0)
				skip_bits(_bits & 7);
		}

		io::size tell_bits() const {
			return _source->tell() * 8 - _bits;
		}

		int available() const {
			return _bits;
		}

	protected:
		static const int _max_bits = static_cast<int>(8 * sizeof(int) - 8);

		int _buf; // bit buffer
		int _bits; // number of bits in the buffer (needs to be signed, see skip_bits above)

		size_t _read(char* buf, size_t size) {
			size_t result = 0;
			while (size > 0) {
				int c = peek_bits<int>(8);
				if (c == io::eof)
					return result;
				skip_bits(8);
				*buf++ = static_cast<char>(c);
				result++;
				size--;
			}
			return result;
		}

		io::size _tell() const {
			return _source->tell() - (_bits > 0 ? (_bits + 7) / 8 : 0);
		}

		bool _refill(int bits) {
			rt_assert(bits <= _max_bits);
			int c = 0;
			while (_bits < bits) {
				c = _source->read_char();
				uchar_t n = static_cast<uchar_t>(c);
				if (lsb)
					_buf |= n << _bits;
				else
					_buf = (_buf << 8) | n;
				_bits += 8;
			}
			return c != io::eof;
		}
	};

	/* Writes bits to a byte-based output channel, least-significant or most-significant bit first using an int buffer.
	On a machine with 32-bit ints, it can write up to 24 bits at a time. When flushed, it writes up to the first byte
	boundary past the last output bit. */
	template<bool lsb> class bit_output : public io::filter_output {
	public:
		bit_output(io::output& out) :
		io::filter_output(out), _buf(0), _bits(0) {
		}

		void begin() {
			_buf = 0;
			_bits = 0;
		}

		void end() {
			align();
		}

		template<class T> void write_bit(T n) {
			if (_bits + 1 >= sizeof(int) * 8)
				_drain();
			_buf |= static_cast<int>(n) << (lsb ? _bits : (sizeof(int) * 8 - 1 - _bits));
			_bits++;
		}

		template<class T> void write_bits(T n, int bits) {
			if (_bits + bits >= sizeof(int) * 8)
				_drain();
			_buf |= lsb ?
				((static_cast<int>(n) & ~(static_cast<unsigned int>(-1) << bits)) << _bits) :
				((static_cast<int>(n) & (static_cast<unsigned int>(-1) >> (sizeof(int) * 8 - bits))) << (sizeof(int) * 8 - _bits - bits));
			_bits += bits;
		}

		void flush() {
			align();
		}

		void align() {
			while (_bits > 0)
				_drain_char();
			_bits = 0;
		}

		void write(const char* buf, size_t size) {
			for (size_t i = 0; i < size; i++)
				write_bits(buf[i], 8);
		}

		io::size tell() const {
			return _sink->tell() + (_bits > 0 ? (_bits + 7) / 8 : 0);
		}

		io::size tell_bit() const {
			return _sink->tell() * 8 + _bits;
		}

	protected:
		int _buf; // bit buffer
		int _bits; // number of bits in the buffer

		void _drain() {
			while (_bits >= 8)
				_drain_char();
		}

		void _drain_char() {
			uchar_t c = static_cast<uchar_t>(lsb ? _buf : _buf >> ((sizeof(int) - 1) * 8));
			_sink->write_char(static_cast<char>(c));
			if (lsb)
				_buf >>= 8;
			else
				_buf <<= 8;
			_bits -= 8;
		}
	};

	typedef io::bit_input<true> lsb_input;
	typedef io::bit_output<true> lsb_output;

	typedef io::bit_input<false> msb_input;
	typedef io::bit_output<false> msb_output;
}

#endif

