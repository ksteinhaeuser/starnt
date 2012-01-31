#if !defined io_safe_input_h_included
#define io_safe_input_h_included

#include "io/io.exceptions.h"
#include "io/io.filters.h"

namespace io {
	/* An input filter that either reads the requested number of bytes or throws an io::exception. */
	class safe_input : public io::filter_input {
	public:
		safe_input(io::input& in) :
		io::filter_input(in) {
		}

		/* Reads size characters from the channel into a buffer. Throws an io::exception if fewer than size bytes are available. */
		size_t read(char* buf, size_t size) {
			size_t result = _source->read(buf, size);
			if (result < size)
				throw io::exception("unexpected end of input");
			_position += result;
			return result;
		}
	};
}

#endif

