#include "ch_arch_builder.h"
#include "ch_module.h"

ch_module::ch_module(ch_arch_object* parent, gp_Pnt const& p1, gp_Pnt const& p2, TopAbs_State state)
	: ch_arch_object(parent, state)
	, _p1(p1)
	, _p2(p2)
{
	connect_all();
}

ch_module::ch_module(ch_module const& other)
{
	*this = other;
}

ch_module::~ch_module()
{
	disconnect();
}

void ch_module::assign(gp_Pnt const& p1, gp_Pnt const& p2)
{
	if(p1.Distance(p2) >= ch_arch::_accuracy && 
		(p1.Distance(_p1) >= ch_arch::_accuracy ||
		p2.Distance(_p2) >= ch_arch::_accuracy))
	{
		_p1 = p1;
		_p2 = p2;
	}
}

void ch_module::assign(ch_module const& other)
{
	*this = other;
}

ArchObjectType ch_module::arch_type() const
{
	return aotModule;
}

TopAbs_ShapeEnum ch_module::topo_type() const
{
	return ShapeType();
}

void ch_module::connect_all()
{
	connect(this, SIGNAL(build(ch_arch_object&)), ch_arch::arch_builder(), SLOT(on_build(ch_arch_object&)));
}
