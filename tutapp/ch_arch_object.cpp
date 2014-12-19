#include <xutility>
#include <algorithm>

#include "ch_arch_object.h"

ch_arch_object::ch_arch_object(ch_arch_object* parent, TopAbs_State state)
	: _id(QUuid::createUuid())
	, _parent(parent)
	, _state(state)
	, _min_size(1, 1, 1)
	, _length(1)
	, _width(1)
	, _height(1)
{

}

ch_arch_object::ch_arch_object(ch_arch_object const& other)
{
	*this = other;
}

ch_arch_object::~ch_arch_object()
{

}

ch_arch_object& ch_arch_object::operator=(ch_arch_object const& rhs)
{
	_id = rhs._id;
	_parent = rhs._parent;
	_state = rhs._state;
	_min_size = rhs._min_size;
	_length = rhs._length;
	_width = rhs._width;
	_height = rhs._height;
	_shape = rhs._shape;

	return *this;
}

void ch_arch_object::id(QUuid const& id)
{
	_id = id;
}

QUuid ch_arch_object::id() const
{
	return _id;
}

void ch_arch_object::parent(ch_arch_object* parent)
{
	_parent = parent;
}

ch_arch_object* ch_arch_object::parent() const
{
	return _parent;
}


void ch_arch_object::state(TopAbs_State state)
{
	_state = state;
}

TopAbs_State ch_arch_object::state() const
{
	return _state;
}

void ch_arch_object::length(Standard_Real length)
{
	if(_length != length)
	{
		_length = length;
		emit length_changed(*this, _length);
	}
}

Standard_Real ch_arch_object::length() const
{
	return _length;
}

void ch_arch_object::width(Standard_Real width)
{
	if(_width != width)
	{
		_width = width;
		emit width_changed(*this, _width);
	}
}

Standard_Real ch_arch_object::width() const
{
	return _width;
}

void ch_arch_object::height(Standard_Real height)
{
	if(_height != height)
	{
		_height = height;
		emit height_changed(*this, _height);
	}
}

Standard_Real ch_arch_object::height() const
{
	return _height;
}

void ch_arch_object::LWH(Standard_Real length, Standard_Real width, Standard_Real height)
{
	if(_length != length || _width != width || _height != height)
	{
		_length = length;
		_width = width; 
		_height = height;
		emit LWH_changed(*this, _length, _width, _height);
	}
}

void ch_arch_object::LWH(Standard_Real& length, Standard_Real& width, Standard_Real& height) const
{
	length = _length;
	width = _width; 
	height = _height;
}

gp_XYZ ch_arch_object::min_size() const
{
	return _min_size;
}

void ch_arch_object::min_size(gp_XYZ const& min_size)
{
	assert(min_size.X() > ch_arch::_confusion && min_size.Y() > ch_arch::_confusion && min_size.Z() > ch_arch::_confusion);

	_min_size = min_size;
}

void ch_arch_object::add(ch_arch_object* obj)
{
	_childs.insert(std::end(_childs), ref_type(obj));
}

void ch_arch_object::add(ref_type const& obj)
{
	_childs.insert(std::end(_childs), obj);
}

Standard_Integer ch_arch_object::find(ref_type const& obj, childs_coll_type& objs, Standard_Boolean recursive)
{
	find_if(objs, recursive, [&obj](ref_type& o) { return o == obj; });

	return objs.size();
}

Standard_Integer ch_arch_object::find(QUuid const& id, childs_coll_type& objs, Standard_Boolean recursive)
{
	find_if(objs, recursive, [&id](ref_type& obj) {	return obj->id() == id; });

	return objs.size();
}

Standard_Integer ch_arch_object::find_if(ArchObjectType type, childs_coll_type& objs, Standard_Boolean recursive)
{
	find_if(objs, recursive, [type](ref_type& obj) { return obj->arch_type() == type; });

	return objs.size();
}

Standard_Integer ch_arch_object::find_if_not(ArchObjectType type, childs_coll_type& objs, Standard_Boolean recursive)
{
	find_if(objs, recursive, [type](ref_type& obj) { return obj->arch_type() != type; });

	return objs.size();
}

void ch_arch_object::remove(ref_type const& obj, Standard_Boolean recursive)
{
	remove_if(recursive, [&obj](ref_type& o) { return o == obj; });
}

void ch_arch_object::remove(QUuid const& id, Standard_Boolean recursive)
{
	remove_if(recursive, [&id](ref_type& obj) {	return obj->id() == id; });
}

void ch_arch_object::remove_if(ArchObjectType type, Standard_Boolean recursive)
{
	remove_if(recursive, [type](ref_type& obj) { return obj->arch_type() == type; });
}

void ch_arch_object::remove_if_not(ArchObjectType type, Standard_Boolean recursive)
{
	remove_if(recursive, [type](ref_type& obj) { return obj->arch_type() != type; });
}

void ch_arch_object::clear()
{
	_childs.clear();
}

ch_arch_object::childs_coll_type& ch_arch_object::childs()
{
	return _childs;
}

ch_arch_object::childs_coll_type const& ch_arch_object::childs() const
{
	return _childs;
}

ArchObjectType ch_arch_object::arch_type() const
{
	return aotArchObject;
}

TopAbs_ShapeEnum ch_arch_object::topo_type() const
{
	return TopAbs_SHAPE;
}

void ch_arch_object::make()
{
	std::for_each(std::begin(_childs), std::end(_childs), [](ref_type& obj) {
		obj->make();
	});

	emit build(*this);
}
