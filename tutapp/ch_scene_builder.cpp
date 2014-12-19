#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include "ch_scene_builder.h"

ch_scene_builder::ch_scene_builder()
{

}

ch_scene_builder::~ch_scene_builder()
{

}

void ch_scene_builder::display(Handle_AIS_InteractiveContext context, ch_arch_object& obj, bool disp)
{
	switch(obj.arch_type())
	{
	case aotModule:
		display_module(context, static_cast<ch_module&>(obj), disp);
		break;
	case aotWallFace:
		display_wall_face(context, static_cast<ch_wall_face&>(obj), disp);
		break;
	case aotWall:
		display_wall(context, static_cast<ch_wall&>(obj), disp);
		break;
	default:
		break;
	}
}

void ch_scene_builder::on_display(Handle_AIS_InteractiveContext context, ch_arch_object& obj, bool disp)
{
	display(context, obj, disp);
}

void ch_scene_builder::on_erase(Handle_AIS_InteractiveContext context, ch_arch_object& obj)
{
	context->Remove(obj._shape, 1);
}

void ch_scene_builder::display_module(Handle_AIS_InteractiveContext context, ch_module& module, bool disp)
{
	context->Remove(module._shape, 0);
	module._shape = new AIS_Shape(module);
	if(disp)
	{
		context->Display(module._shape, 1);
	}
	else
	{
		context->Redisplay(module._shape, 1);
	}
} 

void ch_scene_builder::display_wall_face(Handle_AIS_InteractiveContext context, ch_wall_face& wf, bool disp)
{
	context->Remove(wf._shape, 0);
	wf._shape = new AIS_Shape(wf);
	if(disp)
	{
		context->Display(wf._shape, 1);
	}
	else
	{
		context->Redisplay(wf._shape, 1);
	}
}

void ch_scene_builder::display_wall(Handle_AIS_InteractiveContext context, ch_wall& wall, bool disp)
{
	//context->SetDisplayMode(AIS_Shaded);
	context->ActivateStandardMode(TopAbs_FACE);
	context->ActivateStandardMode(TopAbs_VERTEX);
	if(wall._shape.IsNull())
	{
		wall._shape = new AIS_Shape(wall);
		context->Display(wall._shape, 1);
	}
	else
	{
		BRepBuilderAPI_Transform xform(wall, wall._rotation);
		wall._shape->Set(xform.Shape());
		context->Redisplay(wall._shape, 1);
	}
}
