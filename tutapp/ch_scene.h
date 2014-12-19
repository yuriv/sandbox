#ifndef ch_scene_h__
#define ch_scene_h__

/********************************************************************
	created:	2014/11/28
	created:	28:11:2014   13:44
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_scene.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_scene
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Scene auxiliary utilities
*********************************************************************/

#include <QString>

#include "ch_handle.h"

enum DimensionSystem { dsMetric, dsImperial };
enum UnitKind { ukMillimeter, ukCentimeter, ukMeter, ukKilometer, ukDS_Separator,
				ukInch, ukFoot, ukYard, ukChain, ukFurlong, ukMile, ukLeague, ukLightYear, ukParsec };
enum UnitAccuracy { ua1000, ua10000, ua100000, ua1000000, ua10000000, ua100000000 };

//QString ds_literal[] = { "Metric", "Imperial" };
//QString uk_literal[] = { "Millimeters", "Centimeters", "Meters", "Kilometers", "",
//							"Inches", "Foots", "Yards", "Chains", "Furlongs", "Miles", "Leagues", "Light Years", "Parsecs" };
//size_t accuracy_divider[] =
//{
//	1000, 10000, 100000, 1000000, 10000000, 100000000
//};

class ch_scene_builder;

struct ch_scene
{
	static ch_scene_builder* scene_builder();

private:
	static reference<ch_scene_builder> _scene_builder;
};
#endif // ch_scene_h__