#ifndef ch_scene_widget_h__
#define ch_scene_widget_h__

/********************************************************************
	created:	2014/11/29
	created:	29:11:2014   10:21
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_scene_widget.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_scene_widget
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Displays and interacts scene
*********************************************************************/

#include <QWidget>

#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>

#include "ch_scene_object.h"

class ch_scene_widget : public ch_scene_object, QWidget
{
public:
	ch_scene_widget(Handle(AIS_InteractiveContext) _context, QWidget* parent = 0);
	~ch_scene_widget();

	Handle(V3d_View) const view() const;
	Handle(AIS_InteractiveContext) const context() const;

private:
	Handle(V3d_View) _view;
	Handle(AIS_InteractiveContext) _context;
};

#endif // ch_scene_widget_h__