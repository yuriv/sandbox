#include <gp.hxx>
#include <gp_XYZ.hxx>

#include "ch_arch_builder.h"
#include "ch_wall_face.h"
#include "ch_wall_base.h"
#include "ch_wall.h"

ch_wall::ch_wall()
	: ch_arch_object(0, TopAbs_UNKNOWN)
{
	length(min_size().Y());
	width(ch_arch::_wall_width);
	height(ch_arch::_wall_height);

	_end = gp_Pnt(_start.X(), _start.Y() + length(), _start.Z());

	add(new ch_wall_face(this, _start, _end, TopAbs_IN));
	add(new ch_wall_face(this, _start, _end, TopAbs_OUT));
	add(new ch_wall_base(this, _start, _end));

	connect_all();
}

ch_wall::ch_wall(ch_wall const& other)
{
	*this = other;
	connect_all();
}

ch_wall::~ch_wall()
{
	disconnect();
}

ArchObjectType ch_wall::arch_type() const
{
	return aotWall;
}

TopAbs_ShapeEnum ch_wall::topo_type() const
{
	return ShapeType();
}

void ch_wall::on_begin_construct(Handle_V3d_View const view, gp_Pnt2d const& p2D)
{
	if(ch_arch::to_3D(view, p2D, _start, Standard_False))
	{
		_end = gp_Pnt(_start.X(), _start.Y() + length(), _start.Z());
		_direction = gp_Vec(_start, _end);

		ch_arch::rot_3D(view, _start, _end, _direction, _rotation);

		emit build(*this);
		emit constructed(*this);
	}
}

void ch_wall::on_constructing(Handle_V3d_View const view, gp_Pnt2d const& p2D)
{
	if(ch_arch::to_3D(view, p2D, _end, Standard_False))
	{
		length(std::max(_start.Distance(_end), min_size().Y()));

		ch_arch::rot_3D(view, _start, _end, _direction, _rotation);

		emit build(*this);
		emit constructed(*this);
	}
}

void ch_wall::on_cancel_construct()
{
	emit canceled(*this);
}

void ch_wall::on_end_construct(Handle_V3d_View const view, gp_Pnt2d const& p2D)
{
	on_constructing(view, p2D);

	disconnect();
}

void ch_wall::connect_all()
{
	connect(this, SIGNAL(build(ch_arch_object&)), ch_arch::arch_builder(), SLOT(on_build(ch_arch_object&)));
	connect(this, SIGNAL(length_changed(ch_arch_object&, Standard_Real)), 
			ch_arch::arch_builder(), SLOT(on_wall_length_changed(ch_arch_object&, Standard_Real)));
	connect(this, SIGNAL(width_changed(ch_arch_object&, Standard_Real)), 
			ch_arch::arch_builder(), SLOT(on_wall_width_changed(ch_arch_object&, Standard_Real)));
	connect(this, SIGNAL(height_changed(ch_arch_object&, Standard_Real)), 
			ch_arch::arch_builder(), SLOT(on_wall_height_changed(ch_arch_object&, Standard_Real)));
	connect(this, SIGNAL(LWH_changed(ch_arch_object&, Standard_Real, Standard_Real, Standard_Real)), 
			ch_arch::arch_builder(), SLOT(on_wall_LWH_changed(ch_arch_object&, Standard_Real, Standard_Real, Standard_Real)));
}
