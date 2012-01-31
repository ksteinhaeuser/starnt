#if !defined io_copier_h_included
#define io_copier_h_included

#include "basic/basic.buffer.h"
#include "io/io.transform.h"

namespace io {
	/* A stream copier. */ 
	class copier : public io::transform {
	public:
		static const size_t default_increment = 1 << 16; // copy in 64 KB increments by default

		copier(io::input& in, io::output& out, size_t increment = io::copier::default_increment) :
		io::transform(in, out), _increment(increment) {
		}

		virtual ~copier() {
		}

		/* Allocates resources. */
		virtual void begin() {
			_buf.resize(_increment);
		}

		/* Copies data. */
		virtual void run() {
			while (true) {
				size_t read_size = _in.read(&_buf[0], _increment);
				if (read_size == 0)
					break;
				_out.write(&_buf[0], read_size);
			}
		}

		/* Deallocates resources. */
		virtual void end() {
			_buf.discard();
		}

 	protected:
		size_t _increment;
		basic::buffer<char> _buf;
	};
}

#endif

