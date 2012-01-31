#pragma once


#include <string>
#include <unordered_map>

namespace starnt
{

	typedef std::unordered_map<std::string, std::string> dict_container;
	class dictionary
	{
	public:
		dictionary(int type);
		~dictionary();

		/** loads dictionary into dictionary
		*@pre		valid filename and dir
		*@post		file will be loaded into dict and each word will be assigned an index	*/
		void load(const std::wstring file = L"data/english");

		bool find(const std::string& target, dict_container::const_iterator& output) const;

		size_t size() const
		{	return dict.size();	}
	
	private:

		/** applies StarNT transform to an input and output the corresponding code
		*@pre		dictionary loaded successfully
		*@post		according to the index, calculate the code word
		*@param		[in]uint num: the index of the input word
		*@return	the transfromed code word		*/
		void transform(const unsigned int num, std::string& code) const;

		const std::string code_table;
		dict_container dict;		// dictionary
		int type;
		const std::wstring dict_file;

	};


	static dictionary encoder_dict(1);
	static dictionary decoder_dict(2);

}