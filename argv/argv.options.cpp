#include "argv/argv.options.h"

void argv::options::digest(int argc, char** argv) {
	std::string unknown_options;
	args.resize(0);
	for (int n = 1; n < argc; n++) {
		if (argv[n][0] == '-') {
			std::string arg(argv[n]);
			std::string::size_type separator_position = arg.find('=');
			std::string name(arg.substr(0, separator_position));
			std::string value;
			if (separator_position != std::string::npos)
				value = arg.substr(separator_position + 1);

			_options_type::iterator i = _options.begin();
			for (; i != _options.end(); i++)
				if ((*i)->matches(name))
					break;
			if (i == _options.end())
				throw argv::exception("unknown option");
			element* p = *i;
			p->parse(value);
		}
		else
			args.push_back(argv[n]);
	}
}

