#if !defined argv_options_h_included
#define argv_options_h_included

#include "glob/glob.unused.h"
#include <algorithm>
#include <list>
#include <vector>
#include "argv/argv.exceptions.h"
#include "basic/basic.conversions.h"

namespace argv {
	/* Non-template base type for argv::option. */
	class element {
	public:
		/* Constructor. Takes one or two names of the option, separated by '|'. */
		element(const std::string& names) {
			std::string::size_type i = names.find('|');
			_names.push_back(names.substr(0, i));
			if (i != std::string::npos)
				_names.push_back(names.substr(i + 1, names.size() - 1));
		}

		bool matches(const std::string& name) const {
			return std::find(_names.begin(), _names.end(), name) != _names.end();
		}

		virtual void parse(const std::string& arg) = 0;

	protected:
		std::list<std::string> _names;
};

	/* Command-line parser. */
	class options {
	public:
		options() {
		}

		void add(element* element) {
			_options.push_back(element);
		}

		void digest(int argc, char** argv);

		/* Throws an argv::exception when argument count is less than 'at_least'. */
		void ensure_at_least(int at_least) const {
			if (static_cast<int>(args.size()) < at_least)
				throw argv::exception("too few arguments");
		}

		/* Throws an argv::exception when argument count is greater than 'at_most'. */
		void ensure_at_most(int at_most) const {
			if (at_most != -1 && static_cast<int>(args.size()) > at_most)
				throw argv::exception("too many arguments");
		}

		typedef std::vector<const char*> args_type;
		args_type args; // non-option arguments from the command line

	protected:
		typedef std::list<element*> _options_type;
		_options_type _options; // options to parse
	};

	/* Command-line option. Stores the value of the option. */
	template<typename T> class option : public element {
	public:
		// Constructor. names contains option's name(s), separated by '|'.
		option(argv::options& options, const std::string& names, const T& default_value) :
		element(names), _value(default_value) {
			options.add(this);
		}

		void parse(const std::string& /*arg*/) {
			throw argv::exception("unknown option type");
		}

		operator T() const {
			return _value;
		}

		T _value;
	};

	// argv::option template specializations for common types

	template<> inline void argv::option<bool>::parse(const std::string& arg) {
		glob_unused(arg);
		_value = !_value;
	}

	template<> inline void argv::option<char>::parse(const std::string& arg) {
		_value = arg[0];
	}

	template<> inline void argv::option<int>::parse(const std::string& arg) {
		_value = basic::from_s<int>(arg);
	}

	template<> inline void argv::option<std::string>::parse(const std::string& arg) {
		_value = arg;
	}
}

#endif

