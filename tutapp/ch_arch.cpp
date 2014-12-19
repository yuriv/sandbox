#include "ch_arch.h"
#include "ch_arch_builder.h"

#include <gp_Pln.hxx>
#include <gp_Lin.hxx>
#include <gp_Ax1.hxx>
#include <gp_Dir.hxx>
#include <IntAna_IntConicQuad.hxx>
#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>

// 1 unit = 1 inch
Standard_Real ch_arch::_wall_face_width = 40;
Standard_Real ch_arch::_wall_width = 100;
Standard_Real ch_arch::_wall_height = 1000;
Standard_Real ch_arch::_resolution = gp::Resolution();
Standard_Real ch_arch::_confusion = Precision::Confusion();
Standard_Real ch_arch::_accuracy = 1;
Standard_Real ch_arch::_angular_snap = ((M_PI/180)*10);
Standard_Real ch_arch::_grid_snap = 1;

reference<ch_arch_builder> ch_arch::_arch_builder;

/*!
     ConvertToPlane convert 2d window position to 3d point on privileged plane.
*/
Standard_Boolean ch_arch::to_3D(Handle(V3d_View) const view,
								gp_Pnt2d const& p2D, gp_Pnt& p3D,
								Standard_Boolean usePrecision)
{     
	Standard_Real Xp = p2D.X(), Yp = p2D.Y();
	Standard_Real Xv, Yv, Zv;
	Standard_Real Vx, Vy, Vz;
	gp_Pln aPlane(view->Viewer()->PrivilegedPlane());
	view->Convert( Xp, Yp, Xv, Yv, Zv );
	view->Proj( Vx, Vy, Vz );
	gp_Lin aLine(gp_Pnt(Xv, Yv, Zv), gp_Dir(Vx, Vy, Vz));
	IntAna_IntConicQuad theIntersection (aLine, aPlane, Precision::Angular());
	if (theIntersection.IsDone())
	{
		if (!theIntersection.IsParallel())
		{
			if (theIntersection.NbPoints() > 0)
			{
				p3D = theIntersection.Point(1);
				Standard_Real X = p3D.X();
				Standard_Real Y = p3D.Y();
				Standard_Real Z = p3D.Z();
				if (usePrecision)
				{
					Standard_Real myPrecision = 1.0;
					X = (X < 0. ? -1 : (X > 0. ? 1 : 0.)) * floor((abs(X)) / myPrecision) * myPrecision;
					Y = (Y < 0. ? -1 : (Y > 0. ? 1 : 0.)) * floor((abs(Y)) / myPrecision) * myPrecision;
					Z = (Z < 0. ? -1 : (Z > 0. ? 1 : 0.)) * floor((abs(Z)) / myPrecision) * myPrecision;
				}
				return Standard_True;
			}
		}
	}
	return Standard_False;
}

void ch_arch::rot_3D(Handle(V3d_View) const view, gp_Pnt const& start, gp_Pnt const& end, gp_Vec& old_dir, gp_Trsf& tr, Standard_Real ang_snap)
{
	Standard_Real Vx, Vy, Vz;
	gp_Vec new_dir(start, end);
	view->Proj(Vx, Vy, Vz);
	gp_Ax1 ax(start, gp_Dir(Vx, Vy, Vz));
	Standard_Real angle = old_dir.AngleWithRef(new_dir, gp_Vec(Vx, Vy, Vz));
	tr.SetRotation(ax, int(angle/ang_snap)*ang_snap);
}

reference<ch_arch_builder> ch_arch::arch_builder()
{
	if(_arch_builder.is_null())
	{
		_arch_builder = new ch_arch_builder;
	}

	return _arch_builder;
}
