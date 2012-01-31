#if !defined fs_files_h_included
#define fs_files_h_included

#include "glob/glob.platform.h"
#include <string>
#if glob_platform_win32
#include <windows.h>
#else
//#include <dirent.h>
//#include <unistd.h>
//#include <sys/stat.h>
#endif
#include "io/io.streams.h"

namespace fs {
	/* A platform-dependent open file handle type. */
	#if glob_platform_win32
	typedef void* _handle_type;
	static const _handle_type _invalid_handle = reinterpret_cast<_handle_type>(-1);
	#else
	typedef int _handle_type;
	static const _handle_type _invalid_handle = -1;
	#endif

	/* Base class for input and output files. Encapsulates common behaviors. */
	class file {
	public:
		file();
		virtual ~file();

		/* Determines if the file is a console. */
		virtual bool console();

		/* Closes an open file. */
		virtual void close();

		/* Returns the current position in the file. */
		virtual io::size tell() const;

	protected:
		_handle_type _handle;

		/* Opens a file named 'name' in read-only (create == false) or write-only (create == true) mode.
		When creating a new file, overwrites an existing file if overwrite == true.
		'name' is a UTF-8 string. If the file cannot be opened, throws an fs::exception. */
		_handle_type _open(const std::string& name, bool create = false, bool overwrite = false);
	};

	class input_file : public io::input, public fs::file {
	public:
		/* Opens an existing file. 'name' is a UTF-8 string. If the file cannot be opened, throws an fs::exception. */
		void open(const std::string& name);

		/* Reads up to size characters into a buffer. Returns the number of characters read.
		A return value of zero indicates the end of the stream.
		If the file cannot be read, throws an fs::exception. */
		size_t read(char* buf, size_t size);
	};

	class output_file : public io::output, public fs::file {
	public:
		/* Creates a new file and opens it for writing. Optionally overwrites an existing file.
		'name' is a UTF-8 string. If the file cannot be created, throws an fs::exception. */
		void create(const std::string& name, bool force = false);

		void write(const char* buf, size_t size);
	};
}

#endif

