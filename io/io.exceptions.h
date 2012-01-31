#if !defined io_exceptions_h_included
#define io_exceptions_h_included

#include "basic/basic.exceptions.h"

namespace io {
	class exception : public basic::exception {
	public:
		exception(const std::string& reason) :
		basic::exception(reason) {
		}
	};
}

#endif

