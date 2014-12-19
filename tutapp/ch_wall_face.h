#ifndef ch_wall_face_h__
#define ch_wall_face_h__

/********************************************************************
	created:	2014/11/23
	created:	23:11:2014   16:49
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_wall_face.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_wall_face
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Architectural wall face representation
*********************************************************************/

#include <deque>

#include <gp_Pnt.hxx>
#include <TopoDS_Solid.hxx>

#include "ch_module.h"

class ch_wall_face : public ch_arch_object, public TopoDS_Solid
{
	Q_OBJECT

public:
	typedef TopoDS_Solid root_type;

	ch_wall_face();
	ch_wall_face(ch_arch_object* parent, gp_Pnt const& start, gp_Pnt const& end, TopAbs_State state);
	ch_wall_face(ch_wall_face const& other);
	~ch_wall_face();

	void assign(gp_Pnt const& start, gp_Pnt const& end);
	void assign(ch_module* module);
	void assign(ch_wall_face const& other);

	void add(gp_Pnt const& start, gp_Pnt const& end);

	void clear();

	virtual ArchObjectType arch_type() const;
	virtual TopAbs_ShapeEnum topo_type() const;

	friend class ch_arch_builder;
	friend class ch_scene_builder;

private:
	void connect_all();
};

#endif // ch_wall_face_h__
