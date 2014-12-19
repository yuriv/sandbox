#include <assert.h>

#include <QAtomicInt>

#include "ch_handle.h"

//**************************************************************************
// Construction/Destruction

ch_managed::ch_managed()
	: _ref_count(new QAtomicInt(0))
{
}

ch_managed::~ch_managed()
{
	assert((int)(*_ref_count) == 0);
	if ((int)(*_ref_count) != 0)
		return;
	delete _ref_count;
}

void ch_managed::ref() const
{
	_ref_count->ref();
}

void ch_managed::unref() const
{
	assert(_ref_count > 0);
	if (!_ref_count->deref()) {
		delete this;
	}
}

int ch_managed::use_count(void) const
{
	return (int)(*_ref_count);
}

const ch_managed& ch_managed::operator=(const ch_managed&)
{
	// we must not assign _ref_count
	return *this;
}
