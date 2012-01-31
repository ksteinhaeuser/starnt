#include "argv/argv.options.h"
#include "basic/basic.main.h"
#include "basic/basic.pointers.h"
#include "fs/fs.files.h"
#include "io/io.buffers.h"
#include "starnt/starnt.decoder.h"
#include "starnt/starnt.encoder.h"
#include "tbb/tbb_thread.h"


int main_impl(int argc, char** argv) {
	argv::options options;
	argv::option<bool> decode(options, "-d", false);
	options.digest(argc, argv);
	if (options.args.size() != 2)
		throw argv::exception("usage: starnt [-d] input-file output-file");

	fs::input_file in;
	in.open(options.args[0]);
	io::buffered_input buf_in(in);

	fs::output_file out;
	out.create(options.args[1], true);
	io::buffered_output buf_out(out);

	buf_in.begin();
	buf_out.begin();

	basic::auto_ptr<io::transform> transform;
	if (decode)
		transform = new starnt::decoder(buf_in, buf_out);
	else
		transform = new starnt::encoder(buf_in, buf_out);

	std::cout<< "Running...\n";
	tbb::tbb_thread T0([&transform]()
	{
		transform->begin();
		
		transform->run();

		transform->end();
	});

	std::cout<<"Run another job? [Y/N] ";
	std::string ans;
	std::cin>> ans;


	T0.join();
	buf_in.end();
	buf_out.end();
	
	in.close();
	out.close();

	return 0;
}

int main(int argc, char** argv) {
	return basic::main(main_impl, argc, argv);
}

