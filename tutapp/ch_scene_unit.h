#ifndef ch_scene_unit_h__
#define ch_scene_unit_h__

/********************************************************************
	created:	2014/11/28
	created:	28:11:2014   14:17
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_scene_unit.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_scene_unit
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Represents logical per physical units ratio
*********************************************************************/

#include <qscreen.h>

#include "ch_scene_object.h"

class ch_scene_unit : public ch_scene_object
{
	Q_OBJECT

public:
	ch_scene_unit();
	~ch_scene_unit();

signals:
	void dimension_changed(DimensionSystem ds);
	void accuracy_changed(UnitAccuracy ua);
	void unit_changed(UnitKind uk);

public slots:
	void on_DPI_changed(qreal dpi);

private:
	//QScreen _screen;

	void connect_all();
};

#endif // ch_scene_unit_h__