#pragma once

#include <deque>
#include <string>
#include "io/io.transform.h"
#include "tbb/tbb_thread.h"
#include "tbb/concurrent_queue.h"


namespace starnt
{
	class parser : public io::transform
	{
	public:

		parser(io::input& in, io::output& out): io::transform(in, out), is_empty(false)	
		{	buffer = _in.read_char();	}

		parser& operator=(const parser& source)
		{
			if(this != &source)
			{
				_in = source._in;
				_out = source._out;
				is_empty = source.is_empty;
			}
			return *this;
		}
		
//		data_struct nextTok(int count = 10);
		bool nextTok(std::string& data, int& type)
		{
			parse(data, type);
			return is_empty ? false : true;
		}
/*
		bool empty() const
		{	return pool.empty();	}
*/
	private:
		void parse(std::string& data, int& type);

		bool is_empty;
		int buffer;
	};

}