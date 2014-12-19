#ifndef ch_arch_h__
#define ch_arch_h__

/********************************************************************
	created:	2014/11/23
	created:	23:11:2014   12:14
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_arch.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_arch
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Architectural/geometric utilities
*********************************************************************/

#include <assert.h>

#include <V3d_View.hxx>

#include "ch_handle.h"

enum ArchObjectType { aotArchObject, aotModule, aotWallFace, aotWallBase, aotWall, aotBorder, aotUnknown };

class ch_arch_builder;

struct ch_arch 
{
	static Standard_Boolean to_3D(Handle(V3d_View) const view,
									gp_Pnt2d const& p2D, gp_Pnt& p3D,
									Standard_Boolean usePrecision);

	static void rot_3D(Handle(V3d_View) const view, 
						gp_Pnt const& start, gp_Pnt const& end, 
						gp_Vec& old_dir, gp_Trsf& tr, Standard_Real ang_snap = _angular_snap);

	static reference<ch_arch_builder> arch_builder();

	static Standard_Real _confusion;
	static Standard_Real _resolution;
	static Standard_Real _accuracy;
	static Standard_Real _angular_snap;
	static Standard_Real _grid_snap;
	static Standard_Real _wall_face_width;
	static Standard_Real _wall_width;
	static Standard_Real _wall_height;

private:
	static reference<ch_arch_builder> _arch_builder;
};

#endif // ch_arch_h__
