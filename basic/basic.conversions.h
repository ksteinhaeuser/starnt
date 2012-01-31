#if !defined basic_conversions_h_included
#define basic_conversions_h_included

#include <string>

namespace basic {
	/* Reads a decimal integer of type T from a string contained in type U. */
	template<typename T, typename U> T from_s(const U& s) {
		T result = static_cast<T>(0);
		for (typename U::const_iterator i = s.begin(); i != s.end(); i++) {
			bool is_digit = (*i >= '0' && *i <= '9');
			if (!is_digit)
				break;
			T digit = *i - '0';
			result = static_cast<T>(result * 10 + digit);
		}
		return result;
	}
}

#endif

