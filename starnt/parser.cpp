#include <cctype>
#include <string>
#include <algorithm>
#include <iostream>
#include "parser.h"


/*
data_struct starnt::parser::nextTok(int count)
{
	if(pool.size() < 2 && ! is_empty)
	{
//		if(T0.joinable())
//			T0.join();
//		T0 = tbb::tbb_thread([this, count](){ parse(count); } );
		parse(1);
	}

	data_struct tmp;
	tmp = pool.front();
	pool.pop_front();
	return tmp;
}
*/


void starnt::parser::parse(std::string& data, int& type)
{
	const size_t maxsize = 20;
	char temp[maxsize];
	size_t length;
	bool first_letter_upper;
	int cap;

//	for(int i = 0; i < count && buffer != io::eof; i++)
	{
		length = 0;
		cap=0;	// assume lower case
		if(std::isalpha(buffer))
		{
			first_letter_upper = std::isupper(buffer) ? true : false;
			do{	// go thru the letters
				if(std::isupper(buffer) )
					cap++;
				temp[length] = static_cast<char>(buffer);
				length++;
				buffer = _in.read_char();
			}while(std::isalpha(buffer) && length < maxsize );
		
			if( length < 3 || length == maxsize-1 )	 // pass thru invalid words	
				cap = 3;
			else if( cap != 0)
				if(cap == 1 && first_letter_upper) // first letter is upper case
					cap = 1;
				else if( cap == length)	// all upper case
					cap = 2;
				else					// invalid
					cap = 3;
		}
		else
		{
			char cbuffer;

			while( !std::isalpha(buffer) && buffer != io::eof && length < maxsize)
			{
				cbuffer = static_cast<char>(buffer);
				temp[length] = cbuffer;
				length++;
				buffer = _in.read_char();
			}
			cap = -1;
		}

		temp[length] = '\0';
//		pool.push_back(data_struct(cap, temp));
//		pool.push(data_struct(cap, temp));
	}

	if(buffer == io::eof )
		is_empty = true;

	type = cap;
	data = temp;
}