#if !defined argv_exceptions_h_included
#define argv_exceptions_h_included

#include "basic/basic.exceptions.h"

namespace argv {
	class exception : public basic::exception {
	public:
		exception(const std::string& reason) :
		basic::exception(reason) {
		}
	};
}

#endif

