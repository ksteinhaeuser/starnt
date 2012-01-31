#if !defined io_transform_h_included
#define io_transform_h_included

#include "basic/basic.noncopyable.h"
#include "io/io.streams.h"

namespace io {
	/* A transform. Has an input stream and an output stream as its members.
	Reads from the input stream, (usually) applies a transformation, and writes transformed data to the output stream. */ 
	class transform : public basic::noncopyable {
	public:
		transform(io::input& in, io::output& out) : _in(in), _out(out) {
		}

		virtual ~transform() {
		}

		/* Allocates resources. */
		virtual void begin() {
		}

		/* Processes data. */
		virtual void run() {
		}

		/* Deallocates resources. */
		virtual void end() {
		}

 	protected:
		io::input& _in;
		io::output& _out;
	};
}

#endif

