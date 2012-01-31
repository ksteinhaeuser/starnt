#if !defined basic_pointers_h_included
#define basic_pointers_h_included

#include "glob/glob.stdint.h"

namespace basic {
	/* A pointer to a dynamically allocated object. Deletes the object in the destructor or when reassigned. This class should not
	have any time or space overhead over built-in pointers. This class uses a scalar delete and will not work correctly with arrays of
	objects; auto_array should be used for pointers to arrays. A private copy constructor and a private assignment operator disallow
	transfer of ownership of the stored pointer; counted_ptr should be used for shared-ownership pointers.

	[1] David Dodgson, Evolution of the C++ Standard Library, ACM SIGPLAN Notices, v. 31, n. 12 (December 1996), p. 22-26.
	[2] T. A. Cargill, Localized Ownership: Managing Dynamic Objects in C++, in Pattern Languages of Program Design - 2 / edited by
	J. Vlissides, J. O. Coplien, and N. Kerth, Reading, MA : Addison-Wesley, 1996, p. 518.
	[3] Greg Colvin <gregor@netcom.com>, Specifications for auto_ptr and counted_ptr, comp.std.c++, May 25, 1995.
	[4] C++ Boost Smart Pointer Library by Greg Colvin and Beman Dawes. http://www.boost.org. */
	template<class T> class auto_ptr {
	public:
		explicit auto_ptr(T* p = 0) :
		_p(p) {
		}

		~auto_ptr() {
			if (_p != 0)
				delete _p;
		}

		void operator=(T* p) {
			if (_p != p) {
				delete _p;
				_p = p;
			}
		}

		T* operator->() const {
			return _p;
		}

		operator T*() const {
			return _p;
		}

	protected:
		T* _p;

	private:
		auto_ptr(const auto_ptr& other);
		const auto_ptr& operator=(const auto_ptr& other);
	};

	/* A pointer to a dynamically allocated array of objects. Deletes the array of objects in the destructor or when reassigned. This
	class should not have any time or space overhead over built-in pointers. This class uses a vector delete and will not work
	correctly with individual objects; auto_ptr should be used for pointers to objects. A private copy constructor and a private
	assignment operator disallow transfer of ownership of the stored pointer; counted_array should be used for shared-ownership
	pointers. */
	template<class T> class auto_array {
	public:
		explicit auto_array(T* p = 0) :
		_p(p) {
		}

		~auto_array() {
			if (_p != 0)
				delete[] _p;
		}

		void operator=(T* p) {
			if (_p != p) {
				delete[] _p;
				_p = p;
			}
		}

		operator T*() const {
			return _p;
		}

	private:
		T* _p;

		auto_array(const auto_array& other);
		const auto_array& operator=(const auto_array& other);
	};

	/* A pointer to a dynamically allocated object. Deletes the object in the destructor or when reassigned. This is an extension of
	auto_ptr that adds reference counted copy semantics. This class uses a scalar delete and will not work correctly with arrays of
	objects; counted_array should be used for shared pointers to arrays.

	This class meets the CopyConstructible requirement on STL template arguments and the Assignable container requirement. It has a
	copy constructor and an assignment operator, thus allowing shared ownership of the stored pointer.

	This class will not work correctly with cyclic data structures. For example, if a block holds a counted_ptr to object A, which
	directly or indirectly holds a counted_ptr back to object A, then object A's ref_count will be 2, and destruction of the block's
	counted_ptr will leave object A dangling with a ref_count of 1.

	[1] International Standard ISO/IEC 14882, Programming languages - C++, First edition, 1998-09-01, Published by American National
	Standards Institute, New York, NY, 20.1, Requirements, p. 353-357, and 23.1, Container requirements, p. 459-462. */
	template<class T> class counted_ptr {
	public:
		explicit counted_ptr(T* p = 0) :
		_p(p) {
			try {
				_count = new size_t(1);
				*_count = 1;
			}
			catch (...) {
				delete _p;
				throw;
			} // prevent leak if new throws
		}

		counted_ptr(const counted_ptr<T>& other) :
		_p(other._p) {
			_count = other._count;
			(*_count)++;
		}

		~counted_ptr() {
			_dispose();
		}

		counted_ptr& operator=(const counted_ptr<T>& other) {
			_share(other._p, other._count);
			return *this;
		}

		void operator=(T* p) {
			if (_p == p)
				return; // self-assignment
			(*_count)--;
			if (*_count == 0)
				delete _p;
			else {
				// allocate new reference counter
				try {
					_count = new size_t;
				}
				catch (...) {
					(*_count)++;
					delete p;
					throw;
				} // prevent leak if new throws
			}
			*_count = 1;
			_p = p;
		}

		T* operator->() const {
			return _p;
		}

		operator T*() const {
			return _p;
		}

		size_t ref_count() const {
			return *_count;
		}

		bool unique() const {
			return *_count == 1;
		}

	private:
		T* _p;
		size_t* _count; // reference counter (the number of counted_ptrs sharing ownership of the pointer)

		/* Decrements ref_count, deletes the object that the stored pointer points to when ref_count becomes 0. */
		void _dispose() {
			(*_count)--;
			if (*_count == 0) {
				delete _p;
				delete _count;
			}
		}

		/* Shares the object that the stored pointer points to. */
		void _share(T* rpointer, size_t* rcount) {
			if (_count != rcount) {
				_dispose();
				_p = rpointer;
				_count = rcount;
				(*_count)++;
			}
		}
	};

	/* A pointer to a dynamically allocated array of objects. Deletes the array of objects in the destructor or when reassigned. This
	is an extension of auto_array that adds reference counted copy semantics. This class uses a vector delete and therefore will not
	work correctly with individual objects; counted_ptr should be used for shared pointers to individual objects.

	This class meets the CopyConstructible requirement on STL template arguments and the Assignable container requirement. It has a
	copy constructor and an assignment operator, thus allowing shared ownership of the stored pointer.

	This class will not work correctly with cyclic data structures. For example, if a block holds a counted_array to array A, which
	directly or indirectly holds a counted_array back to array A, then array A's ref_count will be 2, and destruction of the block's
	counted_array will leave array A dangling with a ref_count of 1. */
	template<class T> class counted_array {
	public:
		explicit counted_array(T* p = 0) :
		_p(p) {
			try {
				_count = new size_t(1);
				*_count = 1;
			}
			catch (...) {
				delete[] _p;
				throw;
			} // prevent leak if new throws
		}

		counted_array(const counted_array<T>& other) :
		_p(other._p) {
			_count = other._count;
			(*_count)++;
		}

		~counted_array() {
			_dispose();
		}

		counted_array& operator=(const counted_array<T>& other) {
			_share(other._p, other._count);
			return *this;
		}

		void operator=(T* p) {
			if (_p == p)
				return; // self-assignment
			(*_count)--;
			if (*_count == 0)
				delete[] _p;
			else {
				// allocate new reference counter
				try {
					_count = new size_t;
				}
				catch (...) {
					(*_count)++;
					delete[] p;
					throw;
				} // prevent leak if new throws
			}
			*_count = 1;
			_p = p;
		}

		operator T*() const {
			return _p;
		}

		size_t ref_count() const {
			return *_count;
		}

		bool unique() const {
			return *_count == 1;
		}

	private:
		T* _p;
		size_t* _count; // reference counter (the number of counted_ptrs sharing ownership of the pointer)

		/* Decrements ref_count, deletes the array of objects that the stored pointer points to when ref_count becomes 0. */
		void _dispose() {
			(*_count)--;
			if (*_count == 0) {
				delete[] _p;
				delete _count;
			}
		}

		/* Shares the array of objects that the stored pointer points to. */
		void _share(T* rpointer, size_t* rcount) {
			if (_count != rcount) {
				_dispose();
				_p = rpointer;
				_count = rcount;
				(*_count)++;
			}
		}
	};
}

#endif

