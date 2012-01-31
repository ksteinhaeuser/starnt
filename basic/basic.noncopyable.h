#if !defined basic_noncopyable_h_included
#define basic_noncopyable_h_included

namespace basic {
	/* Declares (but does not implement) a private copy constructor and a private assignment operator.
	Inherit from this class to ensure another type is noncopyable and nonassignable. */
	class noncopyable {
	protected:
		noncopyable() {
		}

	private:
		noncopyable(const noncopyable& other);
		const noncopyable& operator=(const noncopyable& rhs);				
	};
}

#endif

