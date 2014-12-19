#ifndef ch_module_h__
#define ch_module_h__

/********************************************************************
	created:	2014/11/23
	created:	23:11:2014   12:38
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_module.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_module
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Architectural module topology
*********************************************************************/

#include "ch_arch_object.h"

#include <Graphic3d_NameOfMaterial.hxx>
#include <TopoDS_Shell.hxx>

class ch_module : public ch_arch_object, public TopoDS_Shell
{
	Q_OBJECT

public:
	typedef TopoDS_Shell root_type;

	ch_module(ch_arch_object* parent, gp_Pnt const& p1, gp_Pnt const& p2, TopAbs_State state = TopAbs_UNKNOWN);
	ch_module(ch_module const& other);
	~ch_module();

	void assign(gp_Pnt const& p1, gp_Pnt const& p2);
	void assign(ch_module const& other);

	virtual ArchObjectType arch_type() const;
	virtual TopAbs_ShapeEnum topo_type() const;

	friend class ch_arch_builder;
	friend class ch_scene_builder;

private:
	gp_Pnt _p1;
	gp_Pnt _p2;

	void connect_all();
};

#endif // ch_module_h__
