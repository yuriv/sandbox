#include "ch_arch_builder.h"
#include "ch_wall_face.h"

ch_wall_face::ch_wall_face()
	: ch_arch_object(0, TopAbs_UNKNOWN)
{
	length(min_size().Y());
	width(ch_arch::_wall_face_width);
	height(ch_arch::_wall_height);

	connect_all();
}

ch_wall_face::ch_wall_face(ch_arch_object* parent, gp_Pnt const& start, gp_Pnt const& end, TopAbs_State state)
	: ch_arch_object(parent, state)
{
	length(min_size().Y());
	width(ch_arch::_wall_face_width);
	height(ch_arch::_wall_height);

	static_cast<ch_arch_object*>(this)->add(new ch_module(this, start, end, this->state()));

	connect_all();
}

ch_wall_face::ch_wall_face(ch_wall_face const& other)
{
	*this = other;

	connect_all();
}

ch_wall_face::~ch_wall_face()
{
	disconnect();
}

void ch_wall_face::assign(gp_Pnt const& start, gp_Pnt const& end)
{
	static_cast<ch_arch_object*>(this)->clear();
	static_cast<ch_arch_object*>(this)->add(new ch_module(this, start, end, state()));
}

void ch_wall_face::assign(ch_module* module)
{
	static_cast<ch_arch_object*>(this)->clear();
	static_cast<ch_arch_object*>(this)->add(module);
}

void ch_wall_face::assign(ch_wall_face const& other)
{
	*this = other;
}

void ch_wall_face::add(gp_Pnt const& start, gp_Pnt const& end)
{
	static_cast<ch_arch_object*>(this)->add(new ch_module(this, start, end, state()));
}

ArchObjectType ch_wall_face::arch_type() const
{
	return aotWallFace;
}

TopAbs_ShapeEnum ch_wall_face::topo_type() const
{
	return ShapeType();
}

void ch_wall_face::connect_all()
{
	connect(this, SIGNAL(build(ch_arch_object&)), ch_arch::arch_builder(), SLOT(on_build(ch_arch_object&)));
}
