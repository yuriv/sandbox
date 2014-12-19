#ifndef ch_arch_object_h__
#define ch_arch_object_h__

/********************************************************************
	created:	2014/11/23
	created:	23:11:2014   11:50
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_arch_object.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_arch_object
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	Base class for architectural objects
*********************************************************************/

#include <deque>

#include <QObject>
#include <QUuid>

#include <gp_XYZ.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>

#include "ch_arch.h"

// todo: change QObject for ch_object
class ch_arch_object : public QObject, public ch_managed
{
	Q_OBJECT

public:
	typedef reference<ch_arch_object> ref_type;
	typedef std::deque<ref_type> childs_coll_type;

	ch_arch_object(ch_arch_object* parent = 0, TopAbs_State state = TopAbs_UNKNOWN);
	ch_arch_object(ch_arch_object const& other);
	~ch_arch_object();

	ch_arch_object& operator=(ch_arch_object const& rhs);

	void id(QUuid const& id);
	QUuid id() const;

	void parent(ch_arch_object* parent);
	ch_arch_object* parent() const;

	void state(TopAbs_State state);
	TopAbs_State state() const;

	void length(Standard_Real length);
	Standard_Real length() const;
	void width(Standard_Real width);
	Standard_Real width() const;
	void height(Standard_Real height);
	Standard_Real height() const;
	void LWH(Standard_Real length, Standard_Real width, Standard_Real height);
	void LWH(Standard_Real& length, Standard_Real& width, Standard_Real& height) const;

	void min_size(gp_XYZ const& min_size);
	gp_XYZ min_size() const;

	void add(ch_arch_object* obj);
	void add(ref_type const& obj);
	template<typename Pred> 
	Standard_Integer find_if(childs_coll_type& objs, Standard_Boolean recursive, Pred& is_suitable);
	Standard_Integer find(ref_type const& obj, childs_coll_type& objs, Standard_Boolean recursive = true);
	Standard_Integer find(QUuid const& id, childs_coll_type& objs, Standard_Boolean recursive = true);
	Standard_Integer find_if(ArchObjectType type, childs_coll_type& objs, Standard_Boolean recursive = true);
	Standard_Integer find_if_not(ArchObjectType type, childs_coll_type& objs, Standard_Boolean recursive = true);
	template<typename Pred> 
	void remove_if(Standard_Boolean recursive, Pred& is_suitable);
	void remove(ref_type const& obj, Standard_Boolean recursive = true);
	void remove(QUuid const& id, Standard_Boolean recursive = true);
	void remove_if(ArchObjectType type, Standard_Boolean recursive = true);
	void remove_if_not(ArchObjectType type, Standard_Boolean recursive = true);
	void clear();

	childs_coll_type& childs();
	childs_coll_type const& childs() const;

	virtual void make();

	template<typename T> T* downcast();

	virtual ArchObjectType arch_type() const;
	virtual TopAbs_ShapeEnum topo_type() const;

	friend class ch_arch_builder;
	friend class ch_scene_builder;

signals:
	void build(ch_arch_object&);
	void length_changed(ch_arch_object&, Standard_Real);
	void width_changed(ch_arch_object&, Standard_Real);
	void height_changed(ch_arch_object&, Standard_Real);
	void LWH_changed(ch_arch_object&, Standard_Real length, Standard_Real width, Standard_Real height);

private:
	QUuid _id;
	ch_arch_object* _parent;
	TopAbs_State _state;

	Standard_Real _length;
	Standard_Real _width;
	Standard_Real _height;
	gp_XYZ _min_size;

	childs_coll_type _childs;

	Handle(AIS_Shape) _shape;
};

template<typename T> 
T* ch_arch_object::downcast() 
{ 
	return static_cast<T*>(this); 
}

template<typename Pred>
Standard_Integer ch_arch_object::find_if(childs_coll_type& objs, Standard_Boolean recursive, Pred& is_suitable)
{
	std::for_each(std::begin(_childs), std::end(_childs), [&objs, recursive, &is_suitable](ref_type& obj) {
		if(is_suitable(obj))
			objs.insert(std::end(objs), obj);
		if(recursive)
			obj->find_if(objs, recursive, is_suitable);
	});

	return objs.size();
}

template<typename Pred>
void ch_arch_object::remove_if(Standard_Boolean recursive, Pred& is_suitable)
{
	childs_coll_type& childs = _childs;

	std::for_each(std::end(_childs), std::begin(_childs), [&childs, recursive, &is_suitable](ref_type& obj) {
		if(recursive)
			obj->remove_if(recursive, is_suitable);
		if(is_suitable(obj))
		{
			auto it = std::find(std::begin(childs), std::end(childs), obj);
			if(it != std::end(childs))
			{
				childs.erase(it);
			}
		}
	});
}

#endif // ch_arch_object_h__
