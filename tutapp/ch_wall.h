#ifndef ch_wall_h__
#define ch_wall_h__

/********************************************************************
	created:	2014/11/23
	created:	23:11:2014   10:20
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_wall.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_wall
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Architectural wall representation
*********************************************************************/

#include <gp_XYZ.hxx>
#include <TopTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopExp_Explorer.hxx>

#include "ch_arch_object.h"

class ch_wall : public ch_arch_object, public TopoDS_CompSolid
{
	Q_OBJECT

public:
	typedef TopoDS_CompSolid root_type;

	ch_wall();
	ch_wall(ch_wall const& other);
	~ch_wall();

	virtual ArchObjectType arch_type() const;
	virtual TopAbs_ShapeEnum topo_type() const;

	friend class ch_arch_builder;
	friend class ch_scene_builder;

signals:
	void canceled(ch_arch_object&);
	void constructed(ch_arch_object&);

public slots:
	void on_begin_construct(Handle_V3d_View const view, gp_Pnt2d const& p2D);
	void on_constructing(Handle_V3d_View const view, gp_Pnt2d const& p2D);
	void on_cancel_construct();
	void on_end_construct(Handle_V3d_View const view, gp_Pnt2d const& p2D);

private:
	gp_Pnt _start;
	gp_Pnt _end;
	gp_Vec _direction;
	gp_Trsf _rotation;

	void connect_all();
};

#endif // ch_wall_h__
