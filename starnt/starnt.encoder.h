#pragma once

#include <string>
#include <unordered_map>
#include "io/io.transform.h"
//#include "starnt/parser.h"

namespace starnt 
{
	typedef std::unordered_map<std::string, std::string> dict_container;
	
	class encoder : public io::transform
	{
	public:
		encoder(io::input& in, io::output& out);

		~encoder();

		/* Allocates resources. */
		void begin();

		/* Processes data.   */
		void run();

		/* Deallocates resources. */
		void end();


	private:

		/** handles a character input
		*@pre		buffer contains the first letter of a word
		*@post		outputs the code to _out and buffer will contain the next input to be checked
		*@param		[in/out]char& buffer: the current input to be processed					*/
		template<typename T> void isAlpha(T& temp, int& buffer) const;

		/** handles a symbol input
		*@pre		buffer contains a non-character
		*@post		pre-fix \,if necessary, then output symbol to _out
		*@param		[in/out]char& buffer: the symbol					*/
		void notAlpha(int& buffer) const;

//		dictionary dict;
//		starnt::parser data;

	};
	
}

