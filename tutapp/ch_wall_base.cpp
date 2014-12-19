#include "ch_arch_builder.h"
#include "ch_wall_base.h"

ch_wall_base::ch_wall_base()
	: ch_arch_object(0, TopAbs_UNKNOWN)
{
	connect_all();
}

ch_wall_base::ch_wall_base(ch_arch_object* parent, gp_Pnt const& p1, gp_Pnt const& p2)
	: ch_arch_object(parent, TopAbs_ON)
	, _p1(p1)
	, _p2(p2)
{
	connect_all();
}

ch_wall_base::ch_wall_base(ch_wall_base const& other)
{
	*this = other;
}

ch_wall_base::~ch_wall_base(void)
{
	disconnect();
}

void ch_wall_base::assign(gp_Pnt const& p1, gp_Pnt const& p2)
{
	if(p1.Distance(p2) >= ch_arch::_accuracy && 
		(p1.Distance(_p1) >= ch_arch::_accuracy ||
		p2.Distance(_p2) >= ch_arch::_accuracy))
	{
		_p1 = p1;
		_p2 = p2;
	}
}

void ch_wall_base::assign(ch_wall_base const& other)
{
	*this = other;
}

ArchObjectType ch_wall_base::arch_type() const
{
	return aotWallBase;
}

TopAbs_ShapeEnum ch_wall_base::topo_type() const
{
	return ShapeType();
}

void ch_wall_base::connect_all()
{
	connect(this, SIGNAL(build(ch_arch_object&)), ch_arch::arch_builder(), SLOT(on_build(ch_arch_object&)));
}
