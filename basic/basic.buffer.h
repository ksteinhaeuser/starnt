#if !defined basic_buffer_h_included
#define basic_buffer_h_included

#include <algorithm>
#include <stdexcept>
#include "basic/basic.exceptions.h"
#include "glob/glob.debug.h"

namespace basic {
	/* A dynamically allocated (resizable) memory buffer. */
	template<class T> class buffer {
	public:
		typedef T value_type;
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;

		buffer(size_type size = 0) {
			_allocate(size);
		}

		buffer(const buffer& other) {
			_allocate(other._size);
			if (_size != 0)
				std::copy(other.begin(), other.end(), begin());
		}

		buffer(T* buf, size_t size) :
		_buf(buf), _size(size), _owned(false) {
		}

		template<class I> buffer(I first, I last) {
			_allocate(std::distance(first, last));
			std::copy(first, last, begin());
		}

		~buffer() {
			_deallocate();
		}

		buffer& operator=(const buffer& other) {
			if (!_owned)
				throw basic::exception("cannot assign to a static buffer");
			buffer<T> temp(other);
			_swap_buf(temp);
			_swap_size(temp);
			return *this;
		}

		// Resizes the buffer, preserves its contents.
		void resize(size_type size) {
			if (!_owned)
				throw basic::exception("cannot resize a static buffer");
			if (size == 0)
				_deallocate();
			else if (_size == 0)
				_allocate(size);
			else if (size != _size) {
				T* copy_buf = _buf;
				size_t copy_size = std::min(_size, size);
				_allocate(size);
				std::copy(&copy_buf[0], &copy_buf[copy_size], _buf);
				delete[] copy_buf;
			}
		}

		// Resizes the buffer, discarding its contents.
		void reset(size_type size) {
			if (!_owned)
				throw basic::exception("cannot resize a static buffer");
			if (size != _size) {
				_deallocate();
				if (size != 0)
					_allocate(size);
			}
		}

		// Same as reset() but avoids downsizing.
		void reset_up(size_type size) {
			if (_size < size)
				reset(size);
		}

		// Discards and deallocates the buffer.
		void discard() {
			reset(0);
		}

		void ensure(size_t size, size_t increment = 1 << 8) {
			if (_size <= size)
				resize(_size + increment);
		}

		const T* data() const {
			return _buf;
		}

		size_type size() const {
			return _size;
		}

		size_type element_size() const {
			return sizeof(T);
		}

		size_type allocated_size() const {
			return _size * sizeof(T);
		}

		bool empty() const {
			return _size == 0;
		}

		static size_type max_size() {
			return size_t(-1);
		}

		void copy(buffer& other) {
			other.reset_up(_size);
			std::copy(begin(), end(), other.begin());
		}

		void fill(const T& value) {
			std::fill_n(begin(), size(), value);
		}

		iterator begin() {
			return _buf;
		}

		iterator end() {
			return _buf + _size;
		}

		reference front() {
			return *begin();
		}

		reference back() {
			iterator result(end());
			return *--result;
		}

		const_iterator begin() const {
			return _buf;
		}

		const_iterator end() const {
			return _buf + _size;
		}

		const_reference front() const {
			return *begin();
		}

		const_reference back() const {
			iterator result(end());
			return *--result;
		}

		reference operator[](size_type i) {
			#if glob_debug
			_range_check(i);
			#endif
			return _buf[i];
		}

		const_reference operator[](size_type i) const {
			#if glob_debug
			_range_check(i);
			#endif
			return _buf[i];
		}

	protected:
		T* _buf;
		size_t _size;

	private:
		bool _owned;

		void _allocate(size_type size = 0) {
			_buf = (size == 0) ? 0 : new T[size];
			_size = size;
			_owned = true;
		}

		void _deallocate() {
			if (_owned)
				delete[] _buf;
			_buf = 0;
			_size = 0;
		}

		void _swap_buf(buffer& other) {
			std::swap(_buf, other._buf);
		}

		void _swap_size(buffer& other) {
			std::swap(_size, other._size);
		}

		void _range_check(size_type i) const {
			if (i > _size && i != 0 && _size != 0) // allow container end iterators to reference [_size]
				throw std::range_error("buffer index out of bounds");
		}
	};
}

#endif

