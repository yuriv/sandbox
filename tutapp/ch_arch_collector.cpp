#include <algorithm>

#include "ch_arch_collector.h"

ch_arch_collector::ch_arch_collector()
{

}

ch_arch_collector::ch_arch_collector(ch_arch_collector const& another)
{
	*this = another;
}

ch_arch_collector::~ch_arch_collector()
{

}

ch_arch_collector& ch_arch_collector::operator=(ch_arch_collector const& rhs)
{
	_objects = rhs._objects;

	return *this;
}

void ch_arch_collector::assign(ch_arch_collector::collector_type const& objects)
{
	_objects = objects;
}

void ch_arch_collector::add(ch_arch_object* obj)
{
	_objects.insert(std::end(_objects), ref_type(obj));
}

void ch_arch_collector::add(ch_arch_collector::ref_type const& obj)
{
	_objects.insert(std::end(_objects), obj);
}

void ch_arch_collector::remove(ch_arch_collector::ref_type const& obj)
{
	auto it = std::find(std::begin(_objects), std::end(_objects), obj);
	if(it != std::end(_objects))
	{
		_objects.erase(it);
	}
}

void ch_arch_collector::clear()
{
	_objects.clear();
}

ch_arch_collector::collector_type& ch_arch_collector::objects()
{
	return _objects;
}

ch_arch_collector::collector_type const& ch_arch_collector::objects() const
{
	return _objects;
}
