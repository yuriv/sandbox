#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp.hxx>

#include "ch_arch_builder.h"

ch_arch_builder::ch_arch_builder()
{

}

ch_arch_builder::~ch_arch_builder()
{

}

void ch_arch_builder::build(ch_arch_object& obj)
{
	switch(obj.arch_type())
	{
	case aotModule:
		build_module(static_cast<ch_module&>(obj));
		break;
	case aotWallFace:
		build_wall_face(static_cast<ch_wall_face&>(obj));
		break;
	case aotWallBase:
		build_wall_base(static_cast<ch_wall_base&>(obj));
		break;
	case aotWall:
		build_wall(static_cast<ch_wall&>(obj));
		break;
	default:
		break;
	}
}

void ch_arch_builder::on_build(ch_arch_object& obj)
{
	build(obj);
}

void ch_arch_builder::on_wall_length_changed(ch_arch_object& obj, Standard_Real height)
{
	build(obj);
}

void ch_arch_builder::on_wall_width_changed(ch_arch_object& obj, Standard_Real height)
{
	build(obj);
}

void ch_arch_builder::on_wall_height_changed(ch_arch_object& obj, Standard_Real height)
{
	build(obj);
}

void ch_arch_builder::on_wall_LWH_changed(ch_arch_object& obj, Standard_Real length, Standard_Real width, Standard_Real height)
{
	build(obj);
}

void ch_arch_builder::build_module(ch_module& module)
{
	Standard_Real f_width = ch_arch::_wall_face_width;
	Standard_Real w_length = std::max(module._p1.Distance(module._p2), min_size().Y());
	Standard_Real w_width = ch_arch::_wall_width;
	Standard_Real w_height = ch_arch::_wall_height;	

	Standard_Real offset = (module.state() == TopAbs_IN) ? 0 : w_width - f_width;

	gp_Pnt p0(module._p1.X() + offset, module._p1.Y(), module._p2.Z());
	gp_Pnt p1(module._p1.X() + offset, module._p1.Y() + w_length, module._p2.Z());
	gp_Pnt p2(p1.X(), p1.Y(), p1.Z() + w_height);
	gp_Pnt p3(p0.X(), p0.Y(), p0.Z() + w_height);
	gp_Pnt p4(p0.X() + f_width, p0.Y(), p0.Z());
	gp_Pnt p5(p0.X() + f_width, p0.Y(), p0.Z() + w_height);
	gp_Pnt p6(p1.X() + f_width, p1.Y(), p1.Z() + w_height);
	gp_Pnt p7(p1.X() + f_width, p1.Y(), p1.Z());

	// Create topological vertices
	TopoDS_Vertex v0 = BRepBuilderAPI_MakeVertex(p0);
	TopoDS_Vertex v1 = BRepBuilderAPI_MakeVertex(p1);
	TopoDS_Vertex v2 = BRepBuilderAPI_MakeVertex(p2);
	TopoDS_Vertex v3 = BRepBuilderAPI_MakeVertex(p3);
	TopoDS_Vertex v4 = BRepBuilderAPI_MakeVertex(p4);
	TopoDS_Vertex v5 = BRepBuilderAPI_MakeVertex(p5);
	TopoDS_Vertex v6 = BRepBuilderAPI_MakeVertex(p6);
	TopoDS_Vertex v7 = BRepBuilderAPI_MakeVertex(p7);

	// Create topological edges
	TopoDS_Edge e0 = BRepBuilderAPI_MakeEdge(v0, v1);
	TopoDS_Edge e1 = BRepBuilderAPI_MakeEdge(v1, v2);
	TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(v2, v3);
	TopoDS_Edge e3 = BRepBuilderAPI_MakeEdge(v3, v0);
	TopoDS_Edge e4 = BRepBuilderAPI_MakeEdge(v4, v5);
	TopoDS_Edge e5 = BRepBuilderAPI_MakeEdge(v5, v6);
	TopoDS_Edge e6 = BRepBuilderAPI_MakeEdge(v6, v7);
	TopoDS_Edge e7 = BRepBuilderAPI_MakeEdge(v7, v4);
	TopoDS_Edge e8 = BRepBuilderAPI_MakeEdge(v0, v3);
	TopoDS_Edge e9 = BRepBuilderAPI_MakeEdge(v3, v5);
	TopoDS_Edge e10 = BRepBuilderAPI_MakeEdge(v5, v4);
	TopoDS_Edge e11 = BRepBuilderAPI_MakeEdge(v4, v0);
	TopoDS_Edge e12 = BRepBuilderAPI_MakeEdge(v1, v7);
	TopoDS_Edge e13 = BRepBuilderAPI_MakeEdge(v7, v6);
	TopoDS_Edge e14 = BRepBuilderAPI_MakeEdge(v6, v2);
	TopoDS_Edge e15 = BRepBuilderAPI_MakeEdge(v2, v1);
	TopoDS_Edge e16 = BRepBuilderAPI_MakeEdge(v0, v1);
	TopoDS_Edge e17 = BRepBuilderAPI_MakeEdge(v1, v7);
	TopoDS_Edge e18 = BRepBuilderAPI_MakeEdge(v7, v4);
	TopoDS_Edge e19 = BRepBuilderAPI_MakeEdge(v4, v0);
	TopoDS_Edge e20 = BRepBuilderAPI_MakeEdge(v3, v2);
	TopoDS_Edge e21 = BRepBuilderAPI_MakeEdge(v2, v6);
	TopoDS_Edge e22 = BRepBuilderAPI_MakeEdge(v6, v5);
	TopoDS_Edge e23 = BRepBuilderAPI_MakeEdge(v5, v3);

	// Create topological wires
	TopoDS_Wire w0 = BRepBuilderAPI_MakeWire(e0, e1, e2, e3);
	TopoDS_Wire w1 = BRepBuilderAPI_MakeWire(e4, e5, e6, e7);
	TopoDS_Wire w2 = BRepBuilderAPI_MakeWire(e8, e9, e10, e11);
	TopoDS_Wire w3 = BRepBuilderAPI_MakeWire(e12, e13, e14, e15);
	TopoDS_Wire w4 = BRepBuilderAPI_MakeWire(e16, e17, e18, e19);
	TopoDS_Wire w5 = BRepBuilderAPI_MakeWire(e20, e21, e22, e23);

	// Create topological faces
	TopoDS_Face f0 = BRepBuilderAPI_MakeFace(w0);
	TopoDS_Face f1 = BRepBuilderAPI_MakeFace(w1);
	TopoDS_Face f2 = BRepBuilderAPI_MakeFace(w2);
	TopoDS_Face f3 = BRepBuilderAPI_MakeFace(w3);
	TopoDS_Face f4 = BRepBuilderAPI_MakeFace(w4);
	TopoDS_Face f5 = BRepBuilderAPI_MakeFace(w5);

	BRepBuilderAPI_Sewing sewing;
	sewing.SetNonManifoldMode(Standard_False);
	sewing.Add(f0);
	sewing.Add(f1);
	sewing.Add(f2);
	sewing.Add(f3);
	sewing.Add(f4);
	sewing.Add(f5);
	sewing.Perform();

	static_cast<ch_module::root_type&>(module) = TopoDS::Shell(sewing.SewedShape());
}

void ch_arch_builder::build_wall_face(ch_wall_face& wf)
{
	BRep_Builder builder;
	builder.MakeSolid(wf);

	std::for_each(std::begin(wf.childs()), std::end(wf.childs()), [&builder, &wf](ch_arch_object::ref_type& obj) {
		builder.Add(wf, *obj->downcast<ch_module>());
	});
}

void ch_arch_builder::build_wall_base(ch_wall_base& basis)
{
	gp_Pnt pnt1(basis._p1);
	gp_Pnt pnt2(basis._p2);
	Standard_Real f_width = ch_arch::_wall_face_width;
	Standard_Real w_length = std::max(pnt1.Distance(pnt2), min_size().Y());
	Standard_Real w_width = ch_arch::_wall_width;
	Standard_Real w_height = ch_arch::_wall_height;

	gp_Pnt p0(pnt1.X() + f_width, pnt1.Y(), pnt1.Z());
	gp_Pnt p1(pnt1.X() + f_width, pnt1.Y() + w_length, pnt2.Z());
	gp_Pnt p2(p1.X(), p1.Y(), p1.Z() + w_height);
	gp_Pnt p3(p0.X(), p0.Y(), p0.Z() + w_height);
	gp_Pnt p4(p0.X() + w_width - 2*f_width, p0.Y(), p0.Z());
	gp_Pnt p5(p0.X() + w_width - 2*f_width, p0.Y(), p0.Z() + w_height);
	gp_Pnt p6(p1.X() + w_width - 2*f_width, p1.Y(), p1.Z() + w_height);
	gp_Pnt p7(p1.X() + w_width - 2*f_width, p1.Y(), p1.Z());

	// Create topological vertices
	TopoDS_Vertex v0 = BRepBuilderAPI_MakeVertex(p0);
	TopoDS_Vertex v1 = BRepBuilderAPI_MakeVertex(p1);
	TopoDS_Vertex v2 = BRepBuilderAPI_MakeVertex(p2);
	TopoDS_Vertex v3 = BRepBuilderAPI_MakeVertex(p3);
	TopoDS_Vertex v4 = BRepBuilderAPI_MakeVertex(p4);
	TopoDS_Vertex v5 = BRepBuilderAPI_MakeVertex(p5);
	TopoDS_Vertex v6 = BRepBuilderAPI_MakeVertex(p6);
	TopoDS_Vertex v7 = BRepBuilderAPI_MakeVertex(p7);

	// Create topological edges
	TopoDS_Edge e0 = BRepBuilderAPI_MakeEdge(v0, v1);
	TopoDS_Edge e1 = BRepBuilderAPI_MakeEdge(v1, v2);
	TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(v2, v3);
	TopoDS_Edge e3 = BRepBuilderAPI_MakeEdge(v3, v0);
	TopoDS_Edge e4 = BRepBuilderAPI_MakeEdge(v4, v5);
	TopoDS_Edge e5 = BRepBuilderAPI_MakeEdge(v5, v6);
	TopoDS_Edge e6 = BRepBuilderAPI_MakeEdge(v6, v7);
	TopoDS_Edge e7 = BRepBuilderAPI_MakeEdge(v7, v4);
	TopoDS_Edge e8 = BRepBuilderAPI_MakeEdge(v0, v3);
	TopoDS_Edge e9 = BRepBuilderAPI_MakeEdge(v3, v5);
	TopoDS_Edge e10 = BRepBuilderAPI_MakeEdge(v5, v4);
	TopoDS_Edge e11 = BRepBuilderAPI_MakeEdge(v4, v0);
	TopoDS_Edge e12 = BRepBuilderAPI_MakeEdge(v1, v7);
	TopoDS_Edge e13 = BRepBuilderAPI_MakeEdge(v7, v6);
	TopoDS_Edge e14 = BRepBuilderAPI_MakeEdge(v6, v2);
	TopoDS_Edge e15 = BRepBuilderAPI_MakeEdge(v2, v1);
	TopoDS_Edge e16 = BRepBuilderAPI_MakeEdge(v0, v1);
	TopoDS_Edge e17 = BRepBuilderAPI_MakeEdge(v1, v7);
	TopoDS_Edge e18 = BRepBuilderAPI_MakeEdge(v7, v4);
	TopoDS_Edge e19 = BRepBuilderAPI_MakeEdge(v4, v0);
	TopoDS_Edge e20 = BRepBuilderAPI_MakeEdge(v3, v2);
	TopoDS_Edge e21 = BRepBuilderAPI_MakeEdge(v2, v6);
	TopoDS_Edge e22 = BRepBuilderAPI_MakeEdge(v6, v5);
	TopoDS_Edge e23 = BRepBuilderAPI_MakeEdge(v5, v3);

	// Create topological wires
	TopoDS_Wire w0 = BRepBuilderAPI_MakeWire(e0, e1, e2, e3);
	TopoDS_Wire w1 = BRepBuilderAPI_MakeWire(e4, e5, e6, e7);
	TopoDS_Wire w2 = BRepBuilderAPI_MakeWire(e8, e9, e10, e11);
	TopoDS_Wire w3 = BRepBuilderAPI_MakeWire(e12, e13, e14, e15);
	TopoDS_Wire w4 = BRepBuilderAPI_MakeWire(e16, e17, e18, e19);
	TopoDS_Wire w5 = BRepBuilderAPI_MakeWire(e20, e21, e22, e23);

	// Create topological faces
	TopoDS_Face f0 = BRepBuilderAPI_MakeFace(w0);
	TopoDS_Face f1 = BRepBuilderAPI_MakeFace(w1);
	TopoDS_Face f2 = BRepBuilderAPI_MakeFace(w2);
	TopoDS_Face f3 = BRepBuilderAPI_MakeFace(w3);
	TopoDS_Face f4 = BRepBuilderAPI_MakeFace(w4);
	TopoDS_Face f5 = BRepBuilderAPI_MakeFace(w5);

	BRepBuilderAPI_Sewing sewing;
	sewing.SetNonManifoldMode(Standard_False);
	sewing.Add(f0);
	sewing.Add(f1);
	sewing.Add(f2);
	sewing.Add(f3);
	sewing.Add(f4);
	sewing.Add(f5);
	sewing.Perform();

	BRep_Builder builder;
	builder.MakeSolid(basis);
	builder.Add(basis, TopoDS::Shell(sewing.SewedShape()));
}

void ch_arch_builder::build_wall(ch_wall& wall)
{
	BRep_Builder builder;
	builder.MakeCompSolid(wall);

	std::for_each(std::begin(wall.childs()), std::end(wall.childs()), [&builder, &wall](ch_arch_object::ref_type& obj) {
		if(obj->arch_type() == aotWallBase)
		{
			obj->downcast<ch_wall_base>()->assign(wall._start, wall._end);
			obj->downcast<ch_wall_base>()->make();
			builder.Add(wall, *obj->downcast<ch_wall_base>());
		}
		else if(obj->arch_type() == aotWallFace)
		{
			std::for_each(std::begin(obj->childs()), std::end(obj->childs()), [&wall](ch_arch_object::ref_type& mod) {
				mod->downcast<ch_module>()->assign(wall._start, wall._end);
			});
			obj->downcast<ch_wall_face>()->assign(wall._start, wall._end);
			obj->downcast<ch_wall_face>()->make();
			builder.Add(wall, *obj->downcast<ch_wall_face>());
		}
	});
//
//	ch_module modin(&wall._inner, wall._start, wall._end, TopAbs_IN);
//	ch_module modout(&wall._outer, wall._start, wall._end, TopAbs_OUT);
//
//	build_module(modin);
//	build_module(modout);
////	BRepTools::Write(modin._module, "m1.brep");
////	BRepTools::Write(modout._module, "m2.brep");
//
//	wall._inner.assign(modin);
//	wall._outer.assign(modout);
//	build_wall_face(wall._inner);
//	build_wall_face(wall._outer);
//
//	wall._basis.assign(wall._start, wall._end);
//	build_wall_base(wall._basis);

	//builder.Add(wall, wall._basis);
	//builder.Add(wall, wall._inner);
	//builder.Add(wall, wall._outer);

//	BRepTools::Write(wall._wall, "wall.brep");
}
