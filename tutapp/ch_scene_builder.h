#ifndef ch_scene_builder_h__
#define ch_scene_builder_h__

/********************************************************************
	created:	2014/11/28
	created:	28:11:2014   13:46
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_scene_builder.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_scene_builder
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Displays scene of architectural objects
*********************************************************************/

#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include "ch_arch_all.h"

#include "ch_scene.h"
#include "ch_scene_object.h"

class ch_scene_builder : public ch_scene_object
{
	Q_OBJECT

public:
	ch_scene_builder();
	~ch_scene_builder();

	void display(Handle_AIS_InteractiveContext context, ch_arch_object& obj, bool disp);

public slots:
	void on_display(Handle_AIS_InteractiveContext context, ch_arch_object& obj, bool disp);
	void on_erase(Handle_AIS_InteractiveContext context, ch_arch_object& obj);

private:
	void display_module(Handle_AIS_InteractiveContext context, ch_module& module, bool disp);
	void display_wall_face(Handle_AIS_InteractiveContext context, ch_wall_face& wf, bool disp);
	void display_wall(Handle_AIS_InteractiveContext context, ch_wall& wall, bool disp);

	boost::mutex _mtx;
};

#endif // ch_scene_builder_h__
