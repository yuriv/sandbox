#ifndef ch_arch_builder_h__
#define ch_arch_builder_h__

/********************************************************************
	created:	2014/11/23
	created:	23:11:2014   16:51
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_arch_builder.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_arch_builder
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Builds topological shapes from architectural objects
*********************************************************************/

#include <memory>

#include <TopTools.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Builder.hxx>

#include "ch_arch_object.h"
#include "ch_module.h"
#include "ch_wall_face.h"
#include "ch_wall_base.h"
#include "ch_wall.h"

class ch_arch_builder : public ch_arch_object
{
	Q_OBJECT

public:
	ch_arch_builder();
	~ch_arch_builder();

	void build(ch_arch_object& obj);

public slots:
	void on_build(ch_arch_object& obj);
	void on_wall_length_changed(ch_arch_object& obj, Standard_Real length);
	void on_wall_width_changed(ch_arch_object& obj, Standard_Real width);
	void on_wall_height_changed(ch_arch_object& obj, Standard_Real height);
	void on_wall_LWH_changed(ch_arch_object& obj, Standard_Real length, Standard_Real width, Standard_Real height);

private:
	void build_module(ch_module& module);
	void build_wall_face(ch_wall_face& wf);
	void build_wall_base(ch_wall_base& basis);
	void build_wall(ch_wall& wall);
};

#endif // ch_arch_builder_h__
