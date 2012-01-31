#if !defined basic_exceptions_h_included
#define basic_exceptions_h_included

#include <string>

namespace basic {
	/* Base exception class. Carries (a pointer to) a human-readable message. */
	class exception {
	public:
		exception(const std::string& reason = "unspecified error") :
		_reason(reason) {
		}

		const char* what() const {
			return _reason.c_str();
		}

	private:
		std::string _reason;
	};
}

#endif

