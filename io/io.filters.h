#if !defined io_filters_h_included
#define io_filters_h_included

#include "glob/glob.rt_assert.h"
#include "io/io.streams.h"

namespace io {
	/* Base class of all input channel filters. A filter adds new functionality to an existing input channel
	(the source). io::filter_input itself simply overrides methods of io::input. Subclasses of io::filter_input
	may further override some of these methods as well as provide additional methods and data.
	*/
	class filter_input : public io::input {
	public:
		filter_input(io::input& in) :
		_source(&in) {
		}

		virtual ~filter_input() {
		}

		virtual io::input& source() const {
			return *_source;
		}

		virtual size_t read(char* buf, size_t size) {
			rt_assert(_source != 0);
			size_t result = _source->read(buf, size);
			_position += result;
			return result;
		}

	protected:
		io::input* _source;
	};

	/* Base class of all output channel filters. A filter adds new functionality to an existing output channel
	(the sink). io::filter_output itself simply overrides methods of io::output. Subclasses of io::filter_output
	may further override some of these methods as well as provide additional methods and data.
	*/
	class filter_output : public io::output {
	public:
		filter_output() :
		_sink(0) {
		}

		filter_output(io::output& out) :
		_sink(&out) {
		}

		virtual ~filter_output() {
			try {
				flush();
			}
			catch (...) {
			}
		}

		virtual io::output& sink() const {
			return *_sink;
		}

		virtual void write(const char* buf, size_t size) {
			rt_assert(_sink != 0);
			_sink->write(buf, size);
			_position += size;
		}

		virtual void flush() {
		}

	protected:
		io::output* _sink;
	};
}

#endif

