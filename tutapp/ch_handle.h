#ifndef ch_handle_h__
#define ch_handle_h__

/********************************************************************
	created:	2014/12/01
	created:	1:12:2014   14:13
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_handle.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_handle
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Implements safe object handling
*********************************************************************/

#include <memory>

//template<typename T>
//struct handle : public std::shared_ptr<T>
//{
//	handle() : shared_ptr() {}
//	template<class U> explicit handle(U* p) : shared_ptr(p) {}
//	handle(const handle& x) : shared_ptr(x) {}
//	template<class U> handle(const handle<U>& x) : shared_ptr(x) {}
//	handle<T>& operator=(const handle<T> &) throw() { handle }
//};

class QAtomicInt;

/** reference class
 *  Implementation of the reference counting pattern.
 *  Only able to instantiate with a class inheriting
 *  Base::Handled.
 */
template <class T>
class reference
{
public:
	//**************************************************************************
	// construction & destruction

	/** Pointer and default constructor */
	reference() : _handle(0) {}
	reference(T* p) : _handle(p) {
		if (_handle)
			_handle->ref();
	}

	/** Copy constructor */
	reference(const reference<T>& p) : _handle(p._handle) {
		if (_handle)
			_handle->ref();
	}

	/** destructor
		*  Release the reference counter which causes,
		*  in case of the last one, the referenced object to
		*  be destructed!
		*/
	~reference() {
		if (_handle)
			_handle->unref();
	}

	//**************************************************************************
	// operator implementation

	/** Assign operator from a pointer */
	reference <T>& operator=(T* p) {
		// check if we want to reassign the same object
		if (_handle == p)
			return *this;
		if (_handle)
			_handle->unref();
		_handle = p;
		if (_handle)
			_handle->ref();
		return *this;
	}

	/** Assign operator from a handle */
	reference <T>& operator=(const reference<T>& p) {
		// check if we want to reassign the same object
		if (_handle == p._handle)
			return *this;
		if (_handle)
			_handle->unref();
		_handle = p._handle;
		if (_handle)
			_handle->ref();
		return *this;
	}

	/** Dereference operator */
	T& operator*() const {
		return *_handle;
	}

	/** Dereference operator */
	T* operator->() const {
		return _handle;
	}

	operator T*() const {
		return _handle;
	}

	/** Lower operator, needed for sorting in maps and sets */
	bool operator<(const reference<T>& p) const {
		return _handle < p._handle;
	}

	/** Equal operator */
	bool operator==(const reference<T>& p) const {
		return _handle == p._handle;
	}

	bool operator!=(const reference<T>& p) const {
		return _handle != p._handle;
	}


	//**************************************************************************
	// checking on the state

	/// Test if it handles something
	bool is_valid() const {
		return _handle != 0;
	}

	/// Test if it does not handle anything
	bool is_null() const {
		return _handle == 0;
	}

	/// Get number of references on the object, including this one
	int use_count() const {
		if (_handle)
			return _handle->use_count();
		return 0;
	}

private:
	T* _handle; /** the pointer to the handled object */
};

/** ch_managed class
 *  Implementation of the reference counting pattern.
 */
class ch_managed
{
public:
	ch_managed();
	virtual ~ch_managed();

	void ref() const;
	void unref() const;

	int use_count() const;
	ch_managed const& operator=(ch_managed const&);

private:
	QAtomicInt* _ref_count;
};

#endif // ch_handle_h__