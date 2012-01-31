#if !defined basic_main_h_included
#define basic_main_h_included

#include <iostream>
#include "basic/basic.exceptions.h"
#include "glob/glob.warnings.h"

namespace basic {
	typedef int main_type(int argc, char** argv);

	static int main(basic::main_type main_impl, int argc, char** argv) {
		try {
			main_impl(argc, argv);
			return 0;
		}
		catch (basic::exception& e) {
			std::cout << "exception: " << e.what() << std::endl;
			return 1;
		}
		catch (std::exception& e) {
			std::cout << "std exception: " << e.what() << std::endl;
			return 2;
		}
		catch (...) {
			std::cout << "exception: end of the world" << std::endl;
			return 3;
		}
	}
}

#endif

