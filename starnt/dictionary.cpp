#include "dictionary.h"
#include <fstream>
#include <cstring>
#include <Windows.h>
#include <stdio.h>
#include <cstringt.h>
#include <array>

starnt::dictionary::dictionary(int type): code_table ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
									dict_file(L"data/english")
{ this->type = type; }

starnt::dictionary::~dictionary()
{}

void starnt::dictionary::load(const std::wstring file)
{	
	if( !dict.empty())
		if(file == dict_file)
			return;
		else
			dict.clear();
	
	char* word;
	[&file, &word]()->void
	{	
		#if defined _WIN32
			HANDLE FileIn;
			FileIn = CreateFileW( file.c_str(),GENERIC_READ ,0,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
			if (FileIn == INVALID_HANDLE_VALUE) 
				return;
			LARGE_INTEGER len;
			GetFileSizeEx(FileIn, &len);
			word = new char[len.LowPart + 1];
			DWORD count;
			ReadFile(FileIn, word, len.LowPart, &count, NULL);
			if(count != len.LowPart)
				return;

			word[len.LowPart] = '\0';
			CloseHandle(FileIn);
		#else
			// nix version
		#endif
	}();

	//load individual words into strings
	std::pair<std::string, std::string> dict_entry;	
	char* wordtok;
	wordtok = strtok(word, "\n");
	for(int j = 0 ; wordtok != NULL ; j++ , wordtok = strtok(NULL, "\n"))
	{
		if(type == 1)
		{	// encode
			dict_entry.first = wordtok;		// the word
			transform(j, dict_entry.second);	// code
		}
		else
		{	//decode	
			transform(j, dict_entry.first);	// code
			dict_entry.second = wordtok;		// the word
		}
		dict.insert( dict_entry );	
	}



	// replace LF with \0
/*	for(size_t i=0; i < len ; i++)
	{
		while( (word[i] != '\n') && i < len)
			i++;
		word[i] = '\0';
	}

	// then load individual words into strings
	std::pair<std::string, std::string> dict_entry;

	dict_entry.first= &word[0];
	dict_entry.second = "a";
	dict.insert( dict_entry );
	
	for(int i = 2, j = 1 ; i < len ; i++,j++)
	{
		while(word[i] != '\0')
			i++;
		i++;
		if( i <len)
		{	
			dict_entry.first = &word[i];		// the word
			dict_entry.second = transform(j);	// code

			dict.insert( dict_entry );	
		}
		else
			break;
	}
*/

	delete [] word;

}

/*
void dictionary::load_for_decode(const std::string file)
{	
	std::ifstream is;
	is.open(file, std::ios::binary);
	if(! is.is_open())
	{
		printf("Failed to open dictionary\n");
		exit(1);
	}
	// get length of file
	size_t len = 0;
	is.seekg(0, std::ios::end);
	len = is.tellg();
	is.seekg(0, std::ios::beg);

	// allocate memory:
	char* word;
	word = new char[len];

	// read data as a block:
	is.read(word, len);
	word[len] = '\0';
	is.close();
	
	//load individual words into strings
	std::pair<std::string, std::string> dict_entry;	
	char* wordtok;

	wordtok = strtok(word, "\n");
	for(int j = 0 ; wordtok != NULL ; j++ , wordtok = strtok(NULL, "\n"))
	{
		dict_entry.first = transform(j);	// code
		dict_entry.second = wordtok;		// the word

		dict.insert( dict_entry );	
	}



	// replace LF with \0
	for(size_t i=0; i < len ; i++)
	{
		while( (word[i] != '\n') && i < len)
			i++;
		word[i] = '\0';
	}

	// then load individual words into strings
	std::pair<std::string, std::string> dict_entry;

	dict_entry.first = "a";
	dict_entry.second = &word[0];
	dict.insert( dict_entry );

	for(int i = 2, j = 1 ; i < len ; i++,j++)
	{
		while(word[i] != '\0')
			i++;
		i++;
		if( i <len)
		{	
			dict_entry.first = transform(j);	// code
			dict_entry.second = &word[i];		// the word

			dict.insert( dict_entry );	
		}
		else
			break;
	}

	delete []word;
}
*/
void starnt::dictionary::transform(unsigned int num, std::string& code) const
{	// calculate code word and transform word into code
	int tmp = num;
	char result[4];		// Log52 (dict.size())
	char* p_result = &result[3];
	*p_result = '\0';

	while(num / 52 != 0)
	{
		p_result--;
		*p_result = code_table[num % 52 ] ;		
		num = num / 52;
	}
	p_result--;
	*p_result = code_table[tmp > 51 ? num - 1 : num];		// always want to start with a, so after Z, it's aa not ba

	code = p_result;
}

bool starnt::dictionary::find(const std::string& target, dict_container::const_iterator& output) const
{
	output = dict.find(target);
	if(output == dict.end())
		return false;
	else
		return true;
}

