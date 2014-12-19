#include "ch_scene.h"
#include "ch_scene_builder.h"

reference<ch_scene_builder> ch_scene::_scene_builder;

ch_scene_builder* ch_scene::scene_builder()
{
	if(_scene_builder.is_null())
	{
		_scene_builder = new ch_scene_builder;
	}

	return _scene_builder;
}
