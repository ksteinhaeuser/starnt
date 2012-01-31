#if !defined fs_exceptions_h_included
#define fs_exceptions_h_included

#include "basic/basic.exceptions.h"

namespace fs {
	class exception : public basic::exception {
	public:
		exception(const std::string& reason) :
		basic::exception(reason) {
		}
	};
}

#endif

