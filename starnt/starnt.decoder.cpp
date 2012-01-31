#include <cctype>
#include <string>
#include <fstream>
#include <algorithm>	
#include <iostream>
#include <unordered_map>
#include <omp.h>
#include "starnt/starnt.decoder.h"
#include "starnt/dictionary.h"


starnt::decoder::decoder(io::input& in, io::output& out) :io::transform(in, out)
{}

starnt::decoder::~decoder() 
{}

void starnt::decoder::begin() 
{	
	decoder_dict.load(L"data/english");
}

void starnt::decoder::run() 
{
	int buffer;
	buffer = _in.read_char();

	while(buffer != io::eof)	// for each loop, buffer will hold the next char to be checked
	{
		if(std::isalpha(buffer))
			isCode(buffer);
		else 
			isSymbol(buffer);
	}
}

void starnt::decoder::isCode(int& buffer)
{
	char code[4];
	size_t length = 0;
	char cbuffer = static_cast<char>(buffer);

	do{	// accumulate code
		code[length] = cbuffer;
		length++;
		buffer = _in.read_char();
		cbuffer = static_cast<char>(buffer);
	}while(std::isalpha(buffer));
	code[length] = '\0';

	// find and write out word
	dict_container::const_iterator it;
	bool target_found = decoder_dict.find(code, it);
	
	if ( ! target_found)
	{
		std::cout<<"\nCode not found, error in input data\n";
		exit(1);
	}
	std::string word = (*it).second;


	// handle capitalization
	if (cbuffer == '~')	// first letter upper
	{
		word[0] = std::toupper(word[0]);
		buffer = _in.read_char();	// read in the next char for the next loop
	}
	else if (cbuffer == '`')	//all upper
	{
		for(int i = 0; i < word.size() ; i++)
			word.at(i) = std::toupper(word[i]);
		buffer = _in.read_char();	// read in the next char for the next loop
	}
	
	_out.write_string(word);
}

void starnt::decoder::isSymbol(int& buffer)
{
	char cbuffer = static_cast<char>(buffer);

	switch( cbuffer )
	{
		case '\\':	// escaped special character
			_out.write_byte(_in.read_char());
			buffer = _in.read_char() ;
			break;

		case '*':	// write out words not in dictionary
			buffer = _in.read_char();
			do{
				_out.write_char( static_cast<char>(buffer));
				buffer = _in.read_char();
			}while(std::isalpha(buffer));
			break;

		default:	// other symbol
				_out.write_char(cbuffer);
				buffer = _in.read_char();
	}

}



void starnt::decoder::end() 
{}

