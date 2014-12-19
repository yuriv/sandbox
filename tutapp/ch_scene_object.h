#ifndef ch_scene_object_h__
#define ch_scene_object_h__

/********************************************************************
	created:	2014/11/28
	created:	28:11:2014   13:45
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_scene_object.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_scene_object
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Base class for scene objects
*********************************************************************/

#include <QObject>

#include "ch_scene.h"

class ch_scene_object : public QObject, public ch_managed
{
	Q_OBJECT

};

#endif // ch_scene_object_h__