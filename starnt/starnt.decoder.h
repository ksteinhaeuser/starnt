#pragma once


#include <string>
#include <unordered_map>
#include "io/io.transform.h"


namespace starnt 
{
	typedef std::unordered_map<std::string, std::string> dict_container;
	class decoder : public io::transform {
	public:
		decoder(io::input& in, io::output& out);

		~decoder();

		/* Allocates resources. */
		void begin();

		/* Processes data. */
		void run();

		/* Deallocates resources. */
		void end();

	private:

		/** handles a character input
		*@pre		buffer contains the first letter of a code
		*@post		outputs the word to _out and buffer will contain the next input to be checked
		*@param		[in/out]char& buffer: the current input to be processed					*/
		void isCode(int& buffer);

		/** handles a symbol input
		*@pre		buffer contains a non-character
		*@post		if buffer==*, output the following characters, otherwise, output the symbol to _out
		*@param		[in/out]char& buffer: the symbol					*/
		void isSymbol(int& buffer);
		
	};
}



