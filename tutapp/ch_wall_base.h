#ifndef ch_wall_base_h__
#define ch_wall_base_h__

/********************************************************************
	created:	2014/12/18
	created:	18:12:2014   13:30
	filename: 	c:\prj\tools\QtOCC-0.11\tut\tutapp\ch_wall_base.h
	file path:	c:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_wall_base
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Implements wall base object
*********************************************************************/

#include <TopoDS_Solid.hxx>

#include "ch_arch_object.h"

class ch_wall_base : public ch_arch_object, public TopoDS_Solid
{
public:
	typedef TopoDS_Solid root_type;

	ch_wall_base();
	ch_wall_base(ch_arch_object* parent, gp_Pnt const& p1, gp_Pnt const& p2);
	ch_wall_base(ch_wall_base const& other);

	~ch_wall_base();

	void assign(gp_Pnt const& p1, gp_Pnt const& p2);
	void assign(ch_wall_base const& other);

	virtual ArchObjectType arch_type() const;
	virtual TopAbs_ShapeEnum topo_type() const;

	friend class ch_arch_builder;
	friend class ch_scene_builder;

private:
	gp_Pnt _p1;
	gp_Pnt _p2;

	void connect_all();
};

#endif // ch_wall_base_h__
