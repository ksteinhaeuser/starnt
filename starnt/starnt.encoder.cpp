#include <cctype>
//#include <string>
//#include <fstream>
#include <algorithm>
#include <unordered_map>
//#include <iostream>
#include <array>
#include "starnt/starnt.encoder.h"
#include "fs/fs.files.h"
#include "starnt/dictionary.h"
//#include <boost/date_time/posix_time/posix_time.hpp>



starnt::encoder::encoder(io::input& in, io::output& out): io::transform(in, out)											
{}

starnt::encoder::~encoder() 
{}

void starnt::encoder::begin() 
{	encoder_dict.load();	}



void starnt::encoder::run() 
{	
	const size_t maxsize = 20;
	std::array<char, maxsize> temp;
	
	int buffer;
	buffer = _in.read_char();

	while(buffer != io::eof)	// each loop will always start with a new char in the buffer
		if(std::isalpha(buffer))
			isAlpha(temp, buffer);
		else 
			notAlpha(buffer);
}




template<typename T> void starnt::encoder::isAlpha(T& temp, int& buffer) const
{
	const size_t maxsize = temp.max_size();
	size_t length = 0;
	bool first_letter_upper = std::isupper(buffer) ? true : false;
	size_t cap=0;	// assume lower case
	
	do{	// go thru the letters
		if(std::isupper(buffer) )
			cap++;
		temp[length] = static_cast<char>(buffer);
		length++;
		buffer = _in.read_char();
	}while(std::isalpha(buffer) && length < maxsize-1 );
	temp[length] = '\0';

	if( length < 3 || length == maxsize-1 )	
	{	// pass thru invalid words
		_out.write_char('*');
		_out.write_string(temp.data());
		while(std::isalpha(buffer))
		{
			_out.write_char(static_cast<char>(buffer));
			buffer = _in.read_char();
		}
	}
	else// handle words longer than 2 letters
	{	
		// convert upper case to lower case to ensure search accuracy for valid word forms
		T original;	// used save the original form of the word in case it's not found
		
		if( cap != 0)
			if(cap == 1 && first_letter_upper)
			{	// first letter is upper case
				cap = 1;
				original = temp;
				temp[0] = std::tolower(temp[0]);
			}
			else if( cap == length)
			{	// all upper case
				cap = 2;
				original = temp;
				for(size_t i = 0; i < length ; i++)
					temp[i] = std::tolower(temp[i]);
			}
			else
			{	//pass thru invalid words
				_out.write_char('*');
				_out.write_string(temp.data());
				return;
			}
		
		
		// find word in dictionary
		dict_container::const_iterator it;
		bool target_found = encoder_dict.find(temp.data(), it);
		
		if( target_found )
		{
			_out.write_string( (*it).second );
			if(cap == 1)
				_out.write_char('~');
			else if(cap == 2)
				_out.write_char('`');
		}
		else	// not found, write * + word
		{
			_out.write_char('*');
			_out.write_string( cap == 0 ? temp.data() : original.data());
		}
		
	}

}

void starnt::encoder::notAlpha(int& buffer) const
{
	char cbuffer;
	while( buffer != io::eof && !std::isalpha(buffer) )
	{
		cbuffer = static_cast<char>(buffer);
		if(cbuffer == '\\' || cbuffer == '*' || cbuffer =='~' || cbuffer =='`')
			_out.write_char('\\');
		_out.write_char(cbuffer);
		
		buffer = _in.read_char();	// read in the next char for the next loop
	}
}


void starnt::encoder::end()
{
}
