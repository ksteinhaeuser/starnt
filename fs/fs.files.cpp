#include "fs/fs.files.h"
#if glob_platform_win32
#include <windows.h>
#else
#include <fcntl.h>
#endif
#include "fs/fs.exceptions.h"

fs::file::file() :
_handle(_invalid_handle) {
}

fs::file::~file() {
	close();
}

bool fs::file::console() {
	#if glob_platform_win32
	return GetFileType(_handle) == FILE_TYPE_CHAR;
	#else
	return isatty(_handle) != 0;
	#endif
}

void fs::file::close() {
	if (_handle != fs::_invalid_handle) {
		#if glob_platform_win32
		CloseHandle(_handle);
		#else
		::close(_handle);
		#endif
		_handle = fs::_invalid_handle;
	}
}

io::size fs::file::tell() const {
	io::size result = io::no_size;
	if (_handle == fs::_invalid_handle)
		return result;
	bool success = true;
	
	#if glob_platform_win32
	LARGE_INTEGER dist_to_move, actual_file_pos;
	dist_to_move.QuadPart = 0;
	/* SetFilePointerEx has natural support for 64-bit operations. */
	success = SetFilePointerEx(_handle, dist_to_move, &actual_file_pos, FILE_CURRENT) == TRUE;
	result = actual_file_pos.QuadPart;
	#else
	if ((result = lseek(_handle, 0, SEEK_CUR)) == -1)
		success = false;
	#endif
	
	if (!success)
		throw fs::exception("cannot get file position");
	return result;
}

fs::_handle_type fs::file::_open(const std::string& name, bool create, bool overwrite) {
	fs::_handle_type result = fs::_invalid_handle;
	
	#if glob_platform_win32
	LPWSTR wFilename = new WCHAR [strlen(name.c_str())+1];
	mbstowcs(wFilename, name.c_str(), strlen(name.c_str())+1);

	if (create && overwrite)
	/* dwShareMode is set to 0 because I assumed no other processes will open this file at the same time! */
		result = CreateFileW(wFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	else if (create && !overwrite)
		result = CreateFileW(wFilename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	else
	/* This accounts for create = FALSE and overwrite = FALSE/TRUE because if create = FALSE, overwrite is a don't care condition. */
		result = CreateFileW(wFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	delete [] wFilename;
	#else
	if (create && overwrite)
	/* If the file exists, overwrite it by truncating it to 0. File permissions are 666. */
		result = open(name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	else if (create && !overwrite)
	/* If the file exists,	and we're not overwriting it, the handle will be set to -1 */
		result = open(name.c_str(), O_WRONLY | O_EXCL);
	else
		result = open(name.c_str(), O_RDONLY);
	#endif
	
	if (result == fs::_invalid_handle)
		throw fs::exception("cannot open a file");
	return result;
}

void fs::input_file::open(const std::string& name) {
	_handle = _open(name);
}

size_t fs::input_file::read(char* buf, size_t size) {
	size_t result = 0;
	bool success = true;
	
	#if glob_platform_win32
	DWORD size_read;
	success = ReadFile(_handle, buf, size, &size_read, NULL) == TRUE;
	result = static_cast<size_t>(size_read);
	#else
	if ((result = ::read(_handle, buf, size)) == -1)
		success = false;
	#endif
	
	if (!success)
		throw fs::exception("cannot read a file");
	return result;
}

void fs::output_file::create(const std::string& name, bool overwrite) {
	_handle = _open(name, true, overwrite);
}

void fs::output_file::write(const char* buf, size_t size) {
	bool success = true;
	
	#if glob_platform_win32
	DWORD size_written;
	success = WriteFile(_handle, buf, size, &size_written, NULL) == TRUE;
	#else
	if (::write(_handle, buf, size) == -1)
		success = false;
	#endif
	
	if (!success)
		throw fs::exception("cannot write a file");
}

