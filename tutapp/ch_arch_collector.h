#ifndef ch_arch_collector_h__
#define ch_arch_collector_h__

/********************************************************************
	created:	2014/11/29
	created:	29:11:2014   15:24
	filename: 	C:\prj\tools\QtOCC-0.11\tut\tutapp\ch_arch_collector.h
	file path:	C:\prj\tools\QtOCC-0.11\tut\tutapp
	file base:	ch_arch_collector
	file ext:	h
	author:		Yuri Volodine
	
	purpose:	collects different long live custom objects inherited 
				from ch_arch_object 
*********************************************************************/

#include <deque>

#include "ch_arch_object.h"

enum FilterCriteria { fcArchType, fcShapeType, fcCustom };

class ch_arch_collector
{
public:
	typedef reference<ch_arch_object> ref_type;
	typedef std::deque<ref_type> collector_type;

	ch_arch_collector();
	ch_arch_collector(ch_arch_collector const& other);
	~ch_arch_collector();

	ch_arch_collector& operator=(ch_arch_collector const& rhs);

	void assign(collector_type const& objects);
	void add(ch_arch_object* obj);
	void add(ref_type const& obj);
	void remove(ref_type const& obj);
	void clear();

	template <typename T, FilterCriteria FC>
	size_t filter_by(T fc, collector_type& res);

	collector_type& objects();
	collector_type const& objects() const;

private:
	collector_type _objects;
};

//template <>
//size_t ch_arch_collector::filter_by<ArchObjectType, fcCustom>(ArchObjectType fc, std::deque<handle<ch_arch_object>>& res)
//{
//	res = _objects;
//
//	return res.size();
//}
//
//template <>
//size_t ch_arch_collector::filter_by<TopAbs_ShapeEnum, fcShapeType>(TopAbs_ShapeEnum fc, std::deque<handle<ch_arch_object>>& res)
//{
//	std::for_each(std::begin(_objects), std::end(_objects),
//		[fc, &res](handle<ch_arch_object> obj){
//			if(fc == obj->ShapeType()) {
//				res.insert(std::end(res), obj);
//			}
//	});
//
//	return res.size();
//}
//
//template <>
//size_t ch_arch_collector::filter_by<ArchObjectType, fcArchType>(ArchObjectType fc, std::deque<handle<ch_arch_object>>& res)
//{
//	std::for_each(std::begin(_objects), std::end(_objects),
//		[fc, &res](handle<ch_arch_object> obj){
//			if(fc == obj->arch_type()) {
//				res.insert(std::end(res), obj);
//			}
//	});
//
//	return res.size();
//}
//
//template <>
//size_t ch_arch_collector::filter_by<QUuid, fcCustom>(QUuid fc, std::deque<handle<ch_arch_object>>& res)
//{
//	std::for_each(std::begin(_objects), std::end(_objects),
//		[fc, &res](handle<ch_arch_object> obj){
//			if(fc == obj->id()) {
//				res.insert(std::end(res), obj);
//			}
//	});
//
//	return res.size();
//}

#endif // ch_arch_collector_h__
