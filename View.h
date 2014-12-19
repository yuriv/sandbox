#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QAction>
#include <QList>

#include <Geom_Axis1Placement.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Plane.hxx>
#include <AIS_Line.hxx>
#include <AIS_Point.hxx>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <AIS_Axis.hxx>
#include <TopTools_ListOfShape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Solid.hxx>
#include <StdSelect_FaceFilter.hxx>
#include <Standard_PrimitiveTypes.hxx>
#include <gp_Quaternion.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools.hxx>

#include "ch_arch_collector.h"
#include "ch_arch_builder.h"
#include "ch_scene_builder.h"

class TopoDS_Shape;
class QRubberBand;
class AIS_Point;
class AIS_Line;
class AIS_Shape;
class Geom_CartesianPoint;
class AIS_InteractiveContext;
class V3d_View;

//class COMMONSAMPLE_EXPORT View: public QWidget
class View: public QWidget
{
    Q_OBJECT
protected:
    enum CurrentAction3d { CurAction3d_Nothing, CurAction3d_DynamicZooming,
			               CurAction3d_WindowZooming, CurAction3d_DynamicPanning,
			               CurAction3d_GlobalPanning, CurAction3d_DynamicRotation, 
						   CurAction3d_DrawPoint, CurAction3d_DrawLine, CurAction3d_DrawWall };

public:
    enum ViewAction { ViewFitAllId, ViewFitAreaId, ViewZoomId, ViewPanId, ViewGlobalPanId,
                      ViewFrontId, ViewBackId, ViewTopId, ViewBottomId, ViewLeftId, ViewRightId,
                      ViewAxoId, ViewRotationId, ViewResetId, ViewHlrOffId, ViewHlrOnId, 
					  ViewDrawPoint, ViewDrawLine, ViewDrawWall };
    enum RaytraceAction { ToolRaytracingId, ToolShadowsId, ToolReflectionsId, ToolAntialiasingId };

    View( Handle(AIS_InteractiveContext) theContext, QWidget* parent );

    ~View();

    virtual void                  init();
    bool                          dump( Standard_CString theFile );
    QList<QAction*>*              getViewActions();
    QList<QAction*>*              getRaytraceActions();
    void                          noActiveActions();
    bool                          isShadingMode();

    void                          EnableRaytracing();
    void                          DisableRaytracing();

    void                          SetRaytracedShadows (bool theState);
    void                          SetRaytracedReflections (bool theState);
    void                          SetRaytracedAntialiasing (bool theState);

    bool                          IsRaytracingMode() const { return myIsRaytracing; }
    bool                          IsShadowsEnabled() const { return myIsShadowsEnabled; }
    bool                          IsReflectionsEnabled() const { return myIsReflectionsEnabled; }
    bool                          IsAntialiasingEnabled() const { return myIsAntialiasingEnabled; }

    static QString                GetMessages( int type,TopAbs_ShapeEnum aSubShapeType,
                                               TopAbs_ShapeEnum aShapeType );
    static QString                GetShapeType( TopAbs_ShapeEnum aShapeType );

    Standard_EXPORT static void   OnButtonuseraction( int ExerciceSTEP,
						                              Handle(AIS_InteractiveContext)& );
    Standard_EXPORT static void   DoSelection( int Id,
                                               Handle(AIS_InteractiveContext)& );
    Standard_EXPORT static void   OnSetSelectionMode( Handle(AIS_InteractiveContext)&,
                                                      Standard_Integer&,
						                              TopAbs_ShapeEnum& SelectionMode,
						                              Standard_Boolean& );
	virtual QPaintEngine*          paintEngine() const;
protected:
    virtual void                  paintEvent( QPaintEvent* );
    virtual void                  resizeEvent( QResizeEvent* );
    virtual void                  mousePressEvent( QMouseEvent* );
    virtual void                  mouseReleaseEvent(QMouseEvent* );
    virtual void                  mouseMoveEvent( QMouseEvent* );

    virtual void                  addItemInPopup( QMenu* );

    Handle(V3d_View)&                     getView();
    Handle(AIS_InteractiveContext)&       getContext();
    void                                  activateCursor( const CurrentAction3d );
    void                                  Popup( const int x, const int y );
    CurrentAction3d                       getCurrentMode();

    virtual void                          onLButtonDown( const int nFlags, const QPoint point );
    virtual void                          onMButtonDown( const int nFlags, const QPoint point );
    virtual void                          onRButtonDown( const int nFlags, const QPoint point );
    virtual void                          onLButtonUp( Qt::MouseButtons nFlags, const QPoint point );
    virtual void                          onMButtonUp( Qt::MouseButtons nFlags, const QPoint point );
    virtual void                          onRButtonUp( Qt::MouseButtons nFlags, const QPoint point );
    virtual void                          onMouseMove( Qt::MouseButtons nFlags, const QPoint point );

signals:
	void                          selectionChanged();

	void wall_begin_construct(Handle_V3d_View const view, gp_Pnt2d const& p2D);
	void wall_constructing(Handle_V3d_View const view, gp_Pnt2d const& p2D);
	void wall_cancel_construct();
	void wall_end_construct(Handle_V3d_View const view, gp_Pnt2d const& p2D);
	void scene_display(Handle_AIS_InteractiveContext, ch_arch_object&, bool);
	void scene_erase(Handle_AIS_InteractiveContext, ch_arch_object&);

public slots:
	void                          fitAll();
	void                          fitArea();
	void                          zoom();
	void                          pan();
	void                          globalPan();
	void                          front();
	void                          back();
	void                          top();
	void                          bottom();
	void                          left();
	void                          right();
	void                          axo();
	void                          rotation();
	void                          reset();
	void                          hlrOn();
	void                          hlrOff();
	void                          updateToggled( bool );
	void                          onBackground();
	void                          onEnvironmentMap();
	void                          onRaytraceAction();
	void draw_point();
	void draw_line();
	void draw_wall();

	void on_wall_canceled(ch_arch_object&);
	void on_wall_constructed(ch_arch_object&);

private:
    void                          initCursors();
    void                          initViewActions();
    void                          initRaytraceActions();
    void                          DragEvent( const int x, const int y, const int TheState );
    void                          InputEvent( const int x, const int y );
    void                          MoveEvent( const int x, const int y );
    void                          MultiMoveEvent( const int x, const int y );
    void                          MultiDragEvent( const int x, const int y, const int TheState );
    void                          MultiInputEvent( const int x, const int y );
    void                          DrawRectangle( const int MinX, const int MinY,
                                                 const int MaxX, const int MaxY, const bool Draw );
	static Standard_Boolean ConvertToPlane(Handle(V3d_View) const myView,
											const Standard_Real Xs, 
											const Standard_Real Ys,
											Standard_Real& X,
											Standard_Real& Y,
											Standard_Real& Z,
											Standard_Boolean usePrecision);

private:
    bool                            myIsRaytracing;
    bool                            myIsShadowsEnabled;
    bool                            myIsReflectionsEnabled;
    bool                            myIsAntialiasingEnabled;

    bool                            myFirst;
    bool		                        myDrawRect;           // set when a rect is used for selection or magnify 
    Handle(V3d_View)                myView;
    Handle(AIS_InteractiveContext)  myContext;
    CurrentAction3d                 myCurrentMode;
    Standard_Integer                myXmin;
    Standard_Integer                myYmin;
    Standard_Integer                myXmax;
    Standard_Integer                myYmax;
    Quantity_Factor                 myCurZoom;
    Standard_Boolean                myHlrModeIsOn;
    QList<QAction*>*                myViewActions;
    QList<QAction*>*                myRaytraceActions;
    QMenu*                          myBackMenu;
    QRubberBand*                    myRectBand; //!< selection rectangle rubber band

	ch_arch_collector _coll;
	// Draw Wall
	struct arch_wall
	{
		arch_wall(Standard_Real length = 10, Standard_Real width = 10, Standard_Real height = 300) 
			: _length(length), _width(width), _height(height), _times_pressed(0), _wall(0), _shape(0), _vertices_shape(0), _init_mode(1) {}
		virtual ~arch_wall() { if(_wall != 0) { delete _wall; _wall = 0; } }

		void make_wall(Handle(V3d_View) const myView, gp_Pnt const& corner, Standard_Real dx, Standard_Real dy, Standard_Real dz);
		//void make_wall(gp_Pnt const& corner1, gp_Pnt const& corner2);
		void construct_wall(Handle(AIS_InteractiveContext) ais_ctx, gp_Pnt const& corner, Standard_Real dx, Standard_Real dy, Standard_Real dz);

		Standard_Boolean _init_mode;
		TopTools_ListOfShape _vertices;
		TopTools_ListOfShape _edges;
		TopTools_ListOfShape _wires;
		TopTools_ListOfShape _faces;

		TopTools_IndexedMapOfShape _objects;

		gp_Vec _old_dir;

		BRepPrimAPI_MakeBox* _wall;
		Handle(AIS_Shape) _shape;
		Handle(AIS_Shape) _vertices_shape;
		TopTools_ListOfShape _list;
		gp_Pnt _start;
		Standard_Real _length;
		Standard_Real _width;
		Standard_Real _height;
		size_t _times_pressed;
	} _wall;

	// Draw 2d line
	size_t _times_pressed;
	Handle(Geom_CartesianPoint) _start_point;
	Handle(Geom_CartesianPoint) _end_point;
	Handle(AIS_Line) _line;

	// Draw 2d point
	Handle(AIS_Point) _point;
};

#endif


