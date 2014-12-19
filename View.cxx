#if !defined WNT
#define QT_CLEAN_NAMESPACE         /* avoid definition of INT32 and INT8 */
#endif

#include <iosfwd>

#include "View.h"
#include "ApplicationCommon.h"

#include <QApplication>
#include <QPainter>
#include <QMenu>
#include <QColorDialog>
#include <QCursor>
#include <QFileInfo>
#include <QFileDialog>
#include <QMouseEvent>
#include <QRubberBand>
#include <QMdiSubWindow>
#include <QStyleFactory>

#include <Visual3d_View.hxx>
#include <Graphic3d_ExportFormat.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Graphic3d_TextureEnv.hxx>
#include <Geom_CartesianPoint.hxx>
#include <gp_Pln.hxx>
#include <IntAna_IntConicQuad.hxx>
#include <ElCLib.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#if defined(_WIN32) || defined(__WIN32__)
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
#include <Cocoa_Window.hxx>
#else
#include <QX11Info>
#include <GL/glx.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/Xlib.h>
#include <Xw_Window.hxx>
#include <QColormap>
#endif

#include <math.h>

#include <Aspect_DisplayConnection.hxx>

/*!
     ConvertToPlane convert 2d window position to 3d point on privileged plane.
*/
Standard_Boolean View::ConvertToPlane(Handle(V3d_View) const myView, 
									    const Standard_Real Xs, 
										const Standard_Real Ys,
                                        Standard_Real& X,
                                        Standard_Real& Y,
                                        Standard_Real& Z,
                                        Standard_Boolean usePrecision)
{     
     Standard_Real Xp = Xs, Yp = Ys;
     Standard_Real Xv, Yv, Zv;
     Standard_Real Vx, Vy, Vz;
     gp_Pln aPlane(myView->Viewer()->PrivilegedPlane());
     myView->Convert( Xp, Yp, Xv, Yv, Zv );
     myView->Proj( Vx, Vy, Vz );
     gp_Lin aLine(gp_Pnt(Xv, Yv, Zv), gp_Dir(Vx, Vy, Vz));
     IntAna_IntConicQuad theIntersection (aLine, aPlane, Precision::Angular());
     if (theIntersection.IsDone())
     {
          if (!theIntersection.IsParallel())
          {
               if (theIntersection.NbPoints() > 0)
               {
                    gp_Pnt theSolution(theIntersection.Point(1));
                    X = theSolution.X();
                    Y = theSolution.Y();
                    Z = theSolution.Z();
                    if (usePrecision)
                    {
						Standard_Real myPrecision = 1.0;
                         X = (X < 0. ? -1 : (X > 0. ? 1 : 0.)) * floor((abs(X)) / myPrecision) * myPrecision;
                         Y = (Y < 0. ? -1 : (Y > 0. ? 1 : 0.)) * floor((abs(Y)) / myPrecision) * myPrecision;
                         Z = (Z < 0. ? -1 : (Z > 0. ? 1 : 0.)) * floor((abs(Z)) / myPrecision) * myPrecision;
                    }
                    return Standard_True;
               }
          }
     }
     return Standard_False;
}

void View::arch_wall::construct_wall(Handle(AIS_InteractiveContext) ais_ctx, gp_Pnt const& corner, Standard_Real dx, Standard_Real dy, Standard_Real dz)
{
	dy = dy == 0 ? 1 : dy;

	// Create geometric points
	gp_Pnt p0(corner);
	gp_Pnt p1(p0.X(), p0.Y(), p0.Z() + dz);
	gp_Pnt p2(p0.X(), p0.Y() + dy, p0.Z() + dz);
	gp_Pnt p3(p0.X(), p0.Y() + dy, p0.Z());
	gp_Pnt p4(p0.X() + dx, p0.Y() + dy, p0.Z());
	gp_Pnt p5(p0.X() + dx, p0.Y() + dy, p0.Z() + dz);
	gp_Pnt p6(p0.X() + dx, p0.Y(), p0.Z() + dz);
	gp_Pnt p7(p0.X() + dx, p0.Y(), p0.Z());

	// Create topological vertices
	TopoDS_Vertex v0 = BRepBuilderAPI_MakeVertex(p0);
	TopoDS_Vertex v1 = BRepBuilderAPI_MakeVertex(p1);
	TopoDS_Vertex v2 = BRepBuilderAPI_MakeVertex(p2);
	TopoDS_Vertex v3 = BRepBuilderAPI_MakeVertex(p3);
	TopoDS_Vertex v4 = BRepBuilderAPI_MakeVertex(p4);
	TopoDS_Vertex v5 = BRepBuilderAPI_MakeVertex(p5);
	TopoDS_Vertex v6 = BRepBuilderAPI_MakeVertex(p6);
	TopoDS_Vertex v7 = BRepBuilderAPI_MakeVertex(p7);

	_vertices.Clear();
	_vertices.Append(v0);
	_vertices.Append(v1);
	_vertices.Append(v2);
	_vertices.Append(v3);
	_vertices.Append(v4);
	_vertices.Append(v5);
	_vertices.Append(v6);
	_vertices.Append(v7);

	// Create topological edges
	TopoDS_Edge e0 = BRepBuilderAPI_MakeEdge(v0, v1);
	TopoDS_Edge e1 = BRepBuilderAPI_MakeEdge(v1, v2);
	TopoDS_Edge e2 = BRepBuilderAPI_MakeEdge(v2, v3);
	TopoDS_Edge e3 = BRepBuilderAPI_MakeEdge(v3, v0);
	TopoDS_Edge e4 = BRepBuilderAPI_MakeEdge(v4, v5);
	TopoDS_Edge e5 = BRepBuilderAPI_MakeEdge(v5, v6);
	TopoDS_Edge e6 = BRepBuilderAPI_MakeEdge(v6, v7);
	TopoDS_Edge e7 = BRepBuilderAPI_MakeEdge(v7, v4);
	TopoDS_Edge e8 = BRepBuilderAPI_MakeEdge(v2, v3);
	TopoDS_Edge e9 = BRepBuilderAPI_MakeEdge(v3, v4);
	TopoDS_Edge e10 = BRepBuilderAPI_MakeEdge(v4, v5);
	TopoDS_Edge e11 = BRepBuilderAPI_MakeEdge(v5, v2);
	TopoDS_Edge e12 = BRepBuilderAPI_MakeEdge(v0, v1);
	TopoDS_Edge e13 = BRepBuilderAPI_MakeEdge(v1, v6);
	TopoDS_Edge e14 = BRepBuilderAPI_MakeEdge(v6, v7);
	TopoDS_Edge e15 = BRepBuilderAPI_MakeEdge(v7, v0);
	TopoDS_Edge e16 = BRepBuilderAPI_MakeEdge(v1, v2);
	TopoDS_Edge e17 = BRepBuilderAPI_MakeEdge(v2, v5);
	TopoDS_Edge e18 = BRepBuilderAPI_MakeEdge(v5, v6);
	TopoDS_Edge e19 = BRepBuilderAPI_MakeEdge(v6, v1);
	TopoDS_Edge e20 = BRepBuilderAPI_MakeEdge(v0, v3);
	TopoDS_Edge e21 = BRepBuilderAPI_MakeEdge(v3, v4);
	TopoDS_Edge e22 = BRepBuilderAPI_MakeEdge(v4, v7);
	TopoDS_Edge e23 = BRepBuilderAPI_MakeEdge(v7, v0);

	_edges.Clear();
	_edges.Append(e0);
	_edges.Append(e1);
	_edges.Append(e2);
	_edges.Append(e3);
	_edges.Append(e4);
	_edges.Append(e5);
	_edges.Append(e6);
	_edges.Append(e7);
	_edges.Append(e8);
	_edges.Append(e9);
	_edges.Append(e10);
	_edges.Append(e11);
	_edges.Append(e12);
	_edges.Append(e13);
	_edges.Append(e14);
	_edges.Append(e15);
	_edges.Append(e16);
	_edges.Append(e17);
	_edges.Append(e18);
	_edges.Append(e19);
	_edges.Append(e20);
	_edges.Append(e21);
	_edges.Append(e22);
	_edges.Append(e23);

	// Create topological wire
	TopoDS_Wire w0 = BRepBuilderAPI_MakeWire(e0, e1, e2, e3);
	TopoDS_Wire w1 = BRepBuilderAPI_MakeWire(e4, e5, e6, e7);
	TopoDS_Wire w2 = BRepBuilderAPI_MakeWire(e8, e9, e10, e11);
	TopoDS_Wire w3 = BRepBuilderAPI_MakeWire(e12, e13, e14, e15);
	TopoDS_Wire w4 = BRepBuilderAPI_MakeWire(e16, e17, e18, e19);
	TopoDS_Wire w5 = BRepBuilderAPI_MakeWire(e20, e21, e22, e23);

	_wires.Clear();
	_wires.Append(w0);
	_wires.Append(w1);
	_wires.Append(w2);
	_wires.Append(w3);
	_wires.Append(w4);
	_wires.Append(w5);

	// Create topological faces
	TopoDS_Face f0 = BRepBuilderAPI_MakeFace(w0);
	TopoDS_Face f1 = BRepBuilderAPI_MakeFace(w1);
	TopoDS_Face f2 = BRepBuilderAPI_MakeFace(w2);
	TopoDS_Face f3 = BRepBuilderAPI_MakeFace(w3);
	TopoDS_Face f4 = BRepBuilderAPI_MakeFace(w4);
	TopoDS_Face f5 = BRepBuilderAPI_MakeFace(w5);

	_faces.Clear();
	_faces.Append(f0);
	_faces.Append(f1);
	_faces.Append(f2);
	_faces.Append(f3);
	_faces.Append(f4);
	_faces.Append(f5);

	ais_ctx->ActivateStandardMode(TopAbs_FACE);
	ais_ctx->ActivateStandardMode(TopAbs_VERTEX);

	//BRep_Builder builder;
	//TopoDS_Compound comp;
	//builder.MakeCompound(comp);
	//builder.Add(comp, v0);
	//builder.Add(comp, v1);
	//builder.Add(comp, v2);
	//builder.Add(comp, v3);
	//builder.Add(comp, v4);
	//builder.Add(comp, v5);
	//builder.Add(comp, v6);
	//builder.Add(comp, v7);

	//BRepBuilderAPI_Transform xform(comp, tr);
	
	//ais_ctx->Remove(_vertices_shape, 0);
	//_vertices_shape = new AIS_Shape(xform.Shape());
	//ais_ctx->Display(_vertices_shape, 0);

	{
		BRepBuilderAPI_Sewing sewing;
		sewing.SetNonManifoldMode(Standard_False);
		sewing.Add(f0);
		sewing.Add(f1);
		sewing.Add(f2);
		sewing.Add(f3);
		sewing.Add(f4);
		sewing.Add(f5);
		sewing.Perform();

		BRep_Builder builder;
		TopoDS_Solid solid;
		builder.MakeSolid(solid);
		TopoDS_Shell sewed = TopoDS::Shell(sewing.SewedShape());
		builder.Add(solid, sewed);
		builder.Add(solid, v0);
		builder.Add(solid, v1);
		builder.Add(solid, v2);
		builder.Add(solid, v3);
		builder.Add(solid, v4);
		builder.Add(solid, v5);
		builder.Add(solid, v6);
		builder.Add(solid, v7);

		//ais_ctx->SetDisplayMode(AIS_Shaded);
		ais_ctx->SetDisplayMode(AIS_WireFrame);

		gp_Vec w(_start, gp_Pnt(_start.X() + dx, _start.Y() + dy, _start.Z()/* + dz*/));
		gp_Quaternion qua(_old_dir, w);
		gp_Trsf tr;
		tr.SetRotation(qua);
		BRepBuilderAPI_Transform xform(solid, tr);

		ais_ctx->Remove(_shape, 0);
		_shape = new AIS_Shape(xform.Shape());
		ais_ctx->SetMaterial(_shape, Graphic3d_NOM_PLASTIC, 0);
		ais_ctx->Display(_shape, 1);
	}
}

void View::arch_wall::make_wall(Handle(V3d_View) const myView, gp_Pnt const& corner, Standard_Real dx, Standard_Real dy, Standard_Real dz)
{
	if(_wall != 0)
	{
		delete _wall;
	}

	Standard_Real xv, yv, zv;
	ConvertToPlane(myView, dx, dy, xv, yv, zv, Standard_False);
	_wall = new BRepPrimAPI_MakeBox(corner, gp_Pnt(corner.X() + dx, corner.Y() + dy, dz));

	_wall->Build();

	for(TopTools_ListIteratorOfListOfShape it(_wall->Generated(_wall->Solid())); it.More(); it.Next())
	{
		switch(it.Value().ShapeType())
		{
		case TopAbs_COMPOUND:
			qDebug("TopAbs_COMPOUND\n");
			break;
		case TopAbs_COMPSOLID:
			qDebug("TopAbs_COMPSOLID\n");
			break;
		case TopAbs_SOLID:
			qDebug("TopAbs_SOLID\n");
			break;
		case TopAbs_SHELL:
			qDebug("TopAbs_SHELL\n");
			break;
		case TopAbs_FACE:
			qDebug("TopAbs_FACE\n");
			break;
		case TopAbs_WIRE:
			qDebug("TopAbs_WIRE\n");
			break;
		case TopAbs_EDGE:
			qDebug("TopAbs_EDGE\n");
			break;
		case TopAbs_VERTEX:
			qDebug("TopAbs_VERTEX\n");
			break;
		case TopAbs_SHAPE:
			qDebug("TopAbs_SHAPE\n");
			break;
		default:
			qDebug("Unknown\n");
			break;
		}
	}
}


// the key for multi selection :
#define MULTISELECTIONKEY Qt::ShiftModifier

// the key for shortcut ( use to activate dynamic rotation, panning )
#define CASCADESHORTCUTKEY Qt::ControlModifier

// for elastic bean selection
#define ValZWMin 1

static QCursor* defCursor     = NULL;
static QCursor* handCursor    = NULL;
static QCursor* panCursor     = NULL;
static QCursor* globPanCursor = NULL;
static QCursor* zoomCursor    = NULL;
static QCursor* rotCursor     = NULL;

View::View( Handle(AIS_InteractiveContext) theContext, QWidget* parent )
: QWidget( parent ),
myIsRaytracing( false ),
myIsShadowsEnabled (true),
myIsReflectionsEnabled (true),
myIsAntialiasingEnabled (false),
myViewActions( 0 ),
myRaytraceActions( 0 ),
myBackMenu( NULL ),
_wall(0) 
{
#if !defined(_WIN32) && !defined(__WIN32__) && (!defined(__APPLE__) || defined(MACOSX_USE_GLX))
  //XSynchronize( x11Display(),true ); // it is possible to use QApplication::syncX();
  XSynchronize( x11Info().display(),true ); // it is possible to use QApplication::syncX();
#endif
    myFirst = true;
    myContext = theContext;
	_wall._length = 1;
    //if (theRT)
    //  myContext->SetDisplayMode( AIS_DisplayMode::AIS_Shaded, 1 );

    myXmin = 0;
    myYmin = 0;
    myXmax = 0;
    myYmax = 0;
    myCurZoom = 0;
    myRectBand = 0;

	_times_pressed = 0;

	setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

#if !defined(_WIN32) && !defined(__WIN32__) && (!defined(__APPLE__) || defined(MACOSX_USE_GLX))
    XVisualInfo* pVisualInfo;
    if ( x11Info().display() )
    {
        /* Initialization with the default VisualID */
        Visual *v = DefaultVisual( x11Info().display(), DefaultScreen( x11Info().display() ) );
        int visualID = XVisualIDFromVisual( v );

        /*  Here we use the settings from Optimizer_ViewInfo::TxglCreateWindow() */
        int visualAttr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 1, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
			                 GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER, None };
        pVisualInfo = ::glXChooseVisual( x11Info().display(), DefaultScreen( x11Info().display() ), visualAttr );

        if ( isVisible() )
            hide();

        XSetWindowAttributes a;

        Window p = RootWindow( x11Info().display(), DefaultScreen( x11Info().display() ) );
        a.colormap = XCreateColormap( x11Info().display(), RootWindow( x11Info().display(), pVisualInfo->screen ),
                                      pVisualInfo->visual, AllocNone );
	
	QColor color = palette().color( backgroundRole() );
        QColormap colmap = QColormap::instance();
        a.background_pixel = colmap.pixel( color );
        a.border_pixel = colmap.pixel( Qt::black );
        if ( parentWidget() )
            p = parentWidget()->winId();

        Window w = XCreateWindow( x11Info().display(), p,  x(), y(), width(), height(),
			                      0, pVisualInfo->depth, InputOutput,  pVisualInfo->visual,
			                      CWBackPixel | CWBorderPixel | CWColormap, &a );
        Window *cmw;
        Window *cmwret;
        int count;
        if ( XGetWMColormapWindows( x11Info().display(), topLevelWidget()->winId(), &cmwret, &count ) )
        {
            cmw = new Window[count+1];
            memcpy( (char *)cmw, (char *)cmwret, sizeof(Window)*count );
            XFree( (char *)cmwret );
            int i;
            for ( i = 0; i < count; i++ )
            {
	            if ( cmw[i] == winId() )  /* replace old window */
                {
	                cmw[i] = w;
	                break;
	            }
            }
            if ( i >= count )			 /* append new window */
	            cmw[count++] = w;
        }
        else
        {
            count = 1;
            cmw = new Window[count];
            cmw[0] = w;
        }
        /* Creating new window (with good VisualID) for this widget */
        create(w);
        XSetWMColormapWindows( x11Info().display(), topLevelWidget()->winId(), cmw, count );
        delete [] cmw;

        if ( isVisible() )
            show();
        if ( pVisualInfo )
            XFree( (char *)pVisualInfo );
        XFlush( x11Info().display() );
    }
#endif
    myCurrentMode = CurAction3d_Nothing;
    myHlrModeIsOn = Standard_False;
    setMouseTracking( true );
    
    if( myFirst )
    {
        init();
        myFirst = false;
    }
    initViewActions();
    initCursors();

	connect(this, SIGNAL(scene_display(Handle_AIS_InteractiveContext, ch_arch_object&, bool)), 
			ch_scene::scene_builder(), SLOT(on_display(Handle_AIS_InteractiveContext, ch_arch_object&, bool)));
	connect(this, SIGNAL(scene_erase(Handle_AIS_InteractiveContext, ch_arch_object&)), 
			ch_scene::scene_builder(), SLOT(on_erase(Handle_AIS_InteractiveContext, ch_arch_object&)));

	setBackgroundRole( QPalette::NoRole );//NoBackground );
	// set focus policy to threat QContextMenuEvent from keyboard  
	setFocusPolicy( Qt::StrongFocus );
	setAttribute( Qt::WA_PaintOnScreen );
	setAttribute( Qt::WA_NoSystemBackground );
}

View::~View()
{
  myContext->CloseLocalContext();

  delete myBackMenu;
}

void View::init()
{
  if ( myView.IsNull() )
    myView = myContext->CurrentViewer()->CreateView();
  
#if defined(_WIN32) || defined(__WIN32__)
  Aspect_Handle aWindowHandle = (Aspect_Handle )winId();
  Handle(WNT_Window) hWnd = new WNT_Window (aWindowHandle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
  NSView* aViewHandle = (NSView* )winId();
  Handle(Cocoa_Window) hWnd = new Cocoa_Window (aViewHandle);
#else
  Window aWindowHandle = (Window )winId();
  Handle(Aspect_DisplayConnection) aDispConnection = myContext->CurrentViewer()->Driver()->GetDisplayConnection();
  Handle(Xw_Window) hWnd = new Xw_Window (aDispConnection, aWindowHandle);
#endif // WNT

  myView->SetWindow (hWnd);
  if ( !hWnd->IsMapped() )
  {
    hWnd->Map();
  }
  myView->SetBackgroundColor (Quantity_NOC_BLACK);
  myView->MustBeResized();

  if (myIsRaytracing)
    myView->SetRaytracingMode();

  myView->ZBufferTriedronSetup(Quantity_NOC_RED,Quantity_NOC_GREEN,Quantity_NOC_BLUE1,0.8,0.05,12);
  myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER,Quantity_NOC_WHITE,0.1,V3d_ZBUFFER);
  myView->SetProj( V3d_Zpos );

  //Standard_Real Vx, Vy, Vz;
  //myView->Proj(Vx, Vy, Vz);
  gp_Ax3 Ax3(gp_Pnt(), gp_Dir(0, 0, 1));

  //gp_Ax3 Ax3(myView->Viewer()->PrivilegedPlane().Location(), myView->Viewer()->PrivilegedPlane().Axis().Direction());
  myView->Viewer()->SetPrivilegedPlane(Ax3);
  myView->Viewer()->SetRectangularGridValues(0, 0, 50, 50, 0);
  myView->Viewer()->SetRectangularGridGraphicValues(1000, 1000, 0);
  myView->Viewer()->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);

  myContext->OpenLocalContext();
}

void View::paintEvent( QPaintEvent *  )
{
//  QApplication::syncX();
    if( myFirst )
    {
        init();
        myFirst = false;
    }
    myView->Redraw();
}

void View::resizeEvent( QResizeEvent * )
{
//  QApplication::syncX();
    if( !myView.IsNull() )
    {
        myView->MustBeResized();
    }
}

void View::fitAll()
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void View::fitArea()
{
    myCurrentMode = CurAction3d_WindowZooming;
}

void View::zoom()
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void View::pan()
{
    myCurrentMode = CurAction3d_DynamicPanning;
}

void View::rotation()
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void View::draw_point()
{
	myCurrentMode = CurAction3d_DrawPoint;
}

void View::draw_line()
{
	myCurrentMode = CurAction3d_DrawLine;
}

void View::draw_wall()
{
	reference<ch_arch_object> wall(new ch_wall());

	connect(this, SIGNAL(wall_begin_construct(Handle_V3d_View const, gp_Pnt2d const&)), 
			wall, SLOT(on_begin_construct(Handle_V3d_View const, gp_Pnt2d const&)));
	connect(this, SIGNAL(wall_constructing(Handle_V3d_View const, gp_Pnt2d const&)), 
			wall, SLOT(on_constructing(Handle_V3d_View const, gp_Pnt2d const&)));
	connect(this, SIGNAL(wall_cancel_construct()),
			wall, SLOT(on_cancel_construct()));
	connect(this, SIGNAL(wall_end_construct(Handle_V3d_View const, gp_Pnt2d const&)),
			wall, SLOT(on_end_construct(Handle_V3d_View const, gp_Pnt2d const&)));
	connect(wall, SIGNAL(canceled(ch_arch_object&)), this, SLOT(on_wall_canceled(ch_arch_object&)));
	connect(wall, SIGNAL(constructed(ch_arch_object&)), this, SLOT(on_wall_constructed(ch_arch_object&)));

	_coll.add(wall);

	myCurrentMode = CurAction3d_DrawWall;
}

void View::on_wall_canceled(ch_arch_object& obj)
{
	emit scene_erase(myContext, obj);
	_coll.remove(ch_arch_object::ref_type(&obj));
}

void View::on_wall_constructed(ch_arch_object& obj)
{
	emit scene_display(myContext, obj, true);
}

void View::globalPan()
{
    // save the current zoom value
    myCurZoom = myView->Scale();
    // Do a Global Zoom
    myView->FitAll();
    // Set the mode
    myCurrentMode = CurAction3d_GlobalPanning;
}

void View::front()
{
    myView->SetProj( V3d_Xpos );
}

void View::back()
{
    myView->SetProj( V3d_Xneg );
}

void View::top()
{
    myView->SetProj( V3d_Zpos );
}

void View::bottom()
{
    myView->SetProj( V3d_Zneg );
}

void View::left()
{
    myView->SetProj( V3d_Ypos );
}

void View::right()
{
    myView->SetProj( V3d_Yneg );
}

void View::axo()
{
    myView->SetProj( V3d_XposYnegZpos );
}

void View::reset()
{
    myView->Reset();
}

void View::hlrOff()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    myHlrModeIsOn = Standard_False;
    myView->SetComputedMode (myHlrModeIsOn);
    QApplication::restoreOverrideCursor();
}

void View::hlrOn()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    myHlrModeIsOn = Standard_True;
    myView->SetComputedMode (myHlrModeIsOn);
    QApplication::restoreOverrideCursor();
}

void View::SetRaytracedShadows (bool theState)
{
  if (theState)
    myView->EnableRaytracedShadows();
  else
    myView->DisableRaytracedShadows();

  myIsShadowsEnabled = theState;

  myContext->UpdateCurrentViewer();
}

void View::SetRaytracedReflections (bool theState)
{
  if (theState)
    myView->EnableRaytracedReflections();
  else
    myView->DisableRaytracedReflections();

  myIsReflectionsEnabled = theState;

  myContext->UpdateCurrentViewer();
}

void View::onRaytraceAction()
{
  QAction* aSentBy = (QAction*)sender();
  
  if (aSentBy == myRaytraceActions->at (ToolRaytracingId))
  {
    bool aState = myRaytraceActions->at (ToolRaytracingId)->isChecked(); 

    QApplication::setOverrideCursor (Qt::WaitCursor);
    if (aState)
      EnableRaytracing();
    else
      DisableRaytracing();
    QApplication::restoreOverrideCursor();
  }

  if (aSentBy == myRaytraceActions->at (ToolShadowsId))
  {
    bool aState = myRaytraceActions->at (ToolShadowsId)->isChecked(); 
    SetRaytracedShadows (aState);
  }

  if (aSentBy == myRaytraceActions->at (ToolReflectionsId))
  {
    bool aState = myRaytraceActions->at (ToolReflectionsId)->isChecked();
    SetRaytracedReflections (aState);
  }

  if (aSentBy == myRaytraceActions->at (ToolAntialiasingId))
  {
    bool aState = myRaytraceActions->at (ToolAntialiasingId)->isChecked();
    SetRaytracedAntialiasing (aState);
  }
}

void View::SetRaytracedAntialiasing (bool theState)
{
  if (theState)
    myView->EnableRaytracedAntialiasing();
  else
    myView->DisableRaytracedAntialiasing();

  myIsAntialiasingEnabled = theState;

  myContext->UpdateCurrentViewer();
}

void View::EnableRaytracing()
{
  if (!myIsRaytracing)
    myView->SetRaytracingMode();

  myIsRaytracing = true;

  myContext->UpdateCurrentViewer();
}

void View::DisableRaytracing()
{
  if (myIsRaytracing)
    myView->SetRasterizationMode();

  myIsRaytracing = false;

  myContext->UpdateCurrentViewer();
}

void View::updateToggled( bool isOn )
{
    QAction* sentBy = (QAction*)sender();

    if( !isOn )
        return;

    for ( int i = ViewFitAllId; i < ViewHlrOffId; i++ )
    {
        QAction* anAction = myViewActions->at( i );
        if ( ( anAction == myViewActions->at( ViewFitAreaId ) ) ||
             ( anAction == myViewActions->at( ViewZoomId ) ) ||
             ( anAction == myViewActions->at( ViewPanId ) ) ||
             ( anAction == myViewActions->at( ViewGlobalPanId ) ) ||
             ( anAction == myViewActions->at( ViewRotationId ) ) )
        {
            if ( anAction && ( anAction != sentBy ) )
            {
				      anAction->setCheckable( true );
				      anAction->setChecked( false );
            }
            else
            {
	          if ( sentBy == myViewActions->at( ViewFitAreaId ) )
	            setCursor( *handCursor );
	          else if	( sentBy == myViewActions->at( ViewZoomId ) )
	            setCursor( *zoomCursor );
	          else if	( sentBy == myViewActions->at( ViewPanId ) )
	            setCursor( *panCursor );
	          else if	( sentBy == myViewActions->at( ViewGlobalPanId ) )
	            setCursor( *globPanCursor );
	          else if ( sentBy == myViewActions->at( ViewRotationId ) )
	            setCursor( *rotCursor );
              else
	            setCursor( *defCursor );
	         
            sentBy->setCheckable( false );
            }
        }
    }
}

void View::initCursors()
{
    if ( !defCursor )
        defCursor = new QCursor( Qt::ArrowCursor );
    if ( !handCursor )
        handCursor = new QCursor( Qt::PointingHandCursor );
    if ( !panCursor )
        panCursor = new QCursor( Qt::SizeAllCursor );
    if ( !globPanCursor )
        globPanCursor = new QCursor( Qt::CrossCursor );
    if ( !zoomCursor )
        zoomCursor = new QCursor( QPixmap( ApplicationCommonWindow::getResourceDir() + QString( "/" ) + QObject::tr( "ICON_CURSOR_ZOOM" ) ) );
    if ( !rotCursor )
        rotCursor = new QCursor( QPixmap( ApplicationCommonWindow::getResourceDir() + QString( "/" ) + QObject::tr( "ICON_CURSOR_ROTATE" ) ) );
}

QList<QAction*>* View::getViewActions()
{
    initViewActions();
    return myViewActions;
}

QList<QAction*>* View::getRaytraceActions()
{
    initRaytraceActions();
    return myRaytraceActions;
}

/*!
  Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* View::paintEngine() const
{
  return 0;
}

void View::initViewActions()
{
  if ( myViewActions )
    return;

  myViewActions = new QList<QAction*>();
  QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
  QAction* a;

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_FITALL") ), QObject::tr("MNU_FITALL"), this );
  a->setToolTip( QObject::tr("TBR_FITALL") );
  a->setStatusTip( QObject::tr("TBR_FITALL") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( fitAll() ) );
  myViewActions->insert(ViewFitAllId, a);

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_FITAREA") ), QObject::tr("MNU_FITAREA"), this );
  a->setToolTip( QObject::tr("TBR_FITAREA") );
  a->setStatusTip( QObject::tr("TBR_FITAREA") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( fitArea() ) );

  a->setCheckable( true );
  connect( a, SIGNAL( toggled( bool ) ) , this, SLOT( updateToggled( bool ) ) );
  myViewActions->insert( ViewFitAreaId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_ZOOM") ), QObject::tr("MNU_ZOOM"), this );
  a->setToolTip( QObject::tr("TBR_ZOOM") );
  a->setStatusTip( QObject::tr("TBR_ZOOM") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( zoom() ) );

  a->setCheckable( true );
  connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
  myViewActions->insert( ViewZoomId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_PAN") ), QObject::tr("MNU_PAN"), this );
  a->setToolTip( QObject::tr("TBR_PAN") );
  a->setStatusTip( QObject::tr("TBR_PAN") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( pan() ) );

  a->setCheckable( true );
  connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
  myViewActions->insert( ViewPanId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_GLOBALPAN") ), QObject::tr("MNU_GLOBALPAN"), this );
  a->setToolTip( QObject::tr("TBR_GLOBALPAN") );
  a->setStatusTip( QObject::tr("TBR_GLOBALPAN") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( globalPan() ) );

  a->setCheckable( true );
  connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
  myViewActions->insert( ViewGlobalPanId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_FRONT") ), QObject::tr("MNU_FRONT"), this );
  a->setToolTip( QObject::tr("TBR_FRONT") );
  a->setStatusTip( QObject::tr("TBR_FRONT") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( front() ) );
  myViewActions->insert( ViewFrontId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_BACK") ), QObject::tr("MNU_BACK"), this );
  a->setToolTip( QObject::tr("TBR_BACK") );
  a->setStatusTip( QObject::tr("TBR_BACK") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( back() ) );
  myViewActions->insert(ViewBackId, a);

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_TOP") ), QObject::tr("MNU_TOP"), this );
  a->setToolTip( QObject::tr("TBR_TOP") );
  a->setStatusTip( QObject::tr("TBR_TOP") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( top() ) );
  myViewActions->insert( ViewTopId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_BOTTOM") ), QObject::tr("MNU_BOTTOM"), this );
  a->setToolTip( QObject::tr("TBR_BOTTOM") );
  a->setStatusTip( QObject::tr("TBR_BOTTOM") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( bottom() ) );
  myViewActions->insert( ViewBottomId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_LEFT") ), QObject::tr("MNU_LEFT"), this );
  a->setToolTip( QObject::tr("TBR_LEFT") );
  a->setStatusTip( QObject::tr("TBR_LEFT") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( left() ) );
  myViewActions->insert( ViewLeftId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_RIGHT") ), QObject::tr("MNU_RIGHT"), this );
  a->setToolTip( QObject::tr("TBR_RIGHT") );
  a->setStatusTip( QObject::tr("TBR_RIGHT") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( right() ) );
  myViewActions->insert( ViewRightId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_AXO") ), QObject::tr("MNU_AXO"), this );
  a->setToolTip( QObject::tr("TBR_AXO") );
  a->setStatusTip( QObject::tr("TBR_AXO") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( axo() ) );
  myViewActions->insert( ViewAxoId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_ROTATION") ), QObject::tr("MNU_ROTATION"), this );
  a->setToolTip( QObject::tr("TBR_ROTATION") );
  a->setStatusTip( QObject::tr("TBR_ROTATION") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( rotation() ) );
  a->setCheckable( true );
  connect( a, SIGNAL( toggled(bool) ) , this, SLOT( updateToggled(bool) ) );
  myViewActions->insert( ViewRotationId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_RESET") ), QObject::tr("MNU_RESET"), this );
  a->setToolTip( QObject::tr("TBR_RESET") );
  a->setStatusTip( QObject::tr("TBR_RESET") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( reset() ) );
  myViewActions->insert( ViewResetId, a );

  QActionGroup* ag = new QActionGroup( this );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_HLROFF") ), QObject::tr("MNU_HLROFF"), this );
  a->setToolTip( QObject::tr("TBR_HLROFF") );
  a->setStatusTip( QObject::tr("TBR_HLROFF") );
  connect( a, SIGNAL( triggered() ) , this, SLOT( hlrOff() ) );
  a->setCheckable( true );
  ag->addAction(a);
  myViewActions->insert(ViewHlrOffId, a);

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_HLRON") ), QObject::tr("MNU_HLRON"), this );
  a->setToolTip( QObject::tr("TBR_HLRON") );
  a->setStatusTip( QObject::tr("TBR_HLRON") );
  connect( a, SIGNAL( triggered() ) ,this, SLOT( hlrOn() ) );
  a->setCheckable( true );
  ag->addAction(a);
  myViewActions->insert( ViewHlrOnId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_HLRON") ), QObject::tr("MNU_DRAW_PNT"), this );
  a->setToolTip( QObject::tr("DRAW_PNT") );
  a->setStatusTip( QObject::tr("DRAW_PNT") );
  connect( a, SIGNAL( triggered() ) ,this, SLOT( draw_point() ) );
  ag->addAction(a);
  myViewActions->insert( ViewDrawPoint, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_HLRON") ), QObject::tr("MNU_DRAW_LN"), this );
  a->setToolTip( QObject::tr("DRAW_LN") );
  a->setStatusTip( QObject::tr("DRAW_LN") );
  connect( a, SIGNAL( triggered() ) ,this, SLOT( draw_line() ) );
  ag->addAction(a);
  myViewActions->insert( ViewDrawLine, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_VIEW_HLRON") ), QObject::tr("MNU_DRAW_WL"), this );
  a->setToolTip( QObject::tr("DRAW_WL") );
  a->setStatusTip( QObject::tr("DRAW_WL") );
  connect( a, SIGNAL( triggered() ) ,this, SLOT( draw_wall() ) );
  ag->addAction(a);
  myViewActions->insert( ViewDrawWall, a );
}

void View::initRaytraceActions()
{
  if ( myRaytraceActions )
    return;

  myRaytraceActions = new QList<QAction*>();
  QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
  QAction* a;

  a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_RAYTRACING") ), QObject::tr("MNU_TOOL_RAYTRACING"), this );
  a->setToolTip( QObject::tr("TBR_TOOL_RAYTRACING") );
  a->setStatusTip( QObject::tr("TBR_TOOL_RAYTRACING") );
  a->setCheckable( true );
  a->setChecked( false );
  connect( a, SIGNAL( activated() ) , this, SLOT( onRaytraceAction() ) );
  myRaytraceActions->insert( ToolRaytracingId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_SHADOWS") ), QObject::tr("MNU_TOOL_SHADOWS"), this );
  a->setToolTip( QObject::tr("TBR_TOOL_SHADOWS") );
  a->setStatusTip( QObject::tr("TBR_TOOL_SHADOWS") );
  a->setCheckable( true );
  a->setChecked( true );
  connect( a, SIGNAL( activated() ) , this, SLOT( onRaytraceAction() ) );
  myRaytraceActions->insert( ToolShadowsId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_REFLECTIONS") ), QObject::tr("MNU_TOOL_REFLECTIONS"), this );
  a->setToolTip( QObject::tr("TBR_TOOL_REFLECTIONS") );
  a->setStatusTip( QObject::tr("TBR_TOOL_REFLECTIONS") );
  a->setCheckable( true );
  a->setChecked( true );
  connect( a, SIGNAL( activated() ) , this, SLOT( onRaytraceAction() ) );
  myRaytraceActions->insert( ToolReflectionsId, a );

  a = new QAction( QPixmap( dir+QObject::tr("ICON_TOOL_ANTIALIASING") ), QObject::tr("MNU_TOOL_ANTIALIASING"), this );
  a->setToolTip( QObject::tr("TBR_TOOL_ANTIALIASING") );
  a->setStatusTip( QObject::tr("TBR_TOOL_ANTIALIASING") );
  a->setCheckable( true );
  a->setChecked( false );
  connect( a, SIGNAL( activated() ) , this, SLOT( onRaytraceAction() ) );
  myRaytraceActions->insert( ToolAntialiasingId, a );
}

void View::mousePressEvent( QMouseEvent* e )
{
  if ( e->button() == Qt::LeftButton )
    onLButtonDown( ( e->buttons() | e->modifiers() ), e->pos() );
  else if ( e->button() == Qt::MidButton )
    onMButtonDown( e->buttons() | e->modifiers(), e->pos() );
  else if ( e->button() == Qt::RightButton )
    onRButtonDown( e->buttons() | e->modifiers(), e->pos() );
}

void View::mouseReleaseEvent(QMouseEvent* e)
{
  if ( e->button() == Qt::LeftButton )
    onLButtonUp( e->buttons(), e->pos() );
  else if ( e->button() == Qt::MidButton )
    onMButtonUp( e->buttons(), e->pos() );
  else if( e->button() == Qt::RightButton )
    onRButtonUp( e->buttons(), e->pos() );
}

void View::mouseMoveEvent(QMouseEvent* e)
{
  onMouseMove( e->buttons(), e->pos() );
}

void View::activateCursor( const CurrentAction3d mode )
{
  switch( mode )
  {
    case CurAction3d_DynamicPanning:
         setCursor( *panCursor );
         break;
    case CurAction3d_DynamicZooming:
         setCursor( *zoomCursor );
         break;
    case CurAction3d_DynamicRotation:
         setCursor( *rotCursor );
         break;
    case CurAction3d_GlobalPanning:
         setCursor( *globPanCursor );
         break;
    case CurAction3d_WindowZooming:
         setCursor( *handCursor );
         break;
	case CurAction3d_DrawPoint:
		break;
	case CurAction3d_DrawLine:
		break;
	case CurAction3d_DrawWall:
		break;
    case CurAction3d_Nothing:
    default:
         setCursor( *defCursor );
         break;
  }
}

void View::onLButtonDown( const int/*Qt::MouseButtons*/ nFlags, const QPoint point )
{
  //  save the current mouse coordinate in min
  myXmin = point.x();
  myYmin = point.y();
  myXmax = point.x();
  myYmax = point.y();

  if ( nFlags & CASCADESHORTCUTKEY )
  {
    myCurrentMode = CurAction3d_DynamicZooming;
  }
  else
  {
    switch ( myCurrentMode )
    {
      case CurAction3d_Nothing:
           if ( nFlags & MULTISELECTIONKEY )
	           MultiDragEvent( myXmax, myYmax, -1 );
           else
	           DragEvent( myXmax, myYmax, -1 );
           break;
      case CurAction3d_DynamicZooming:
           break;
      case CurAction3d_WindowZooming:
           break;
      case CurAction3d_DynamicPanning:
           break;
      case CurAction3d_GlobalPanning:
           break;
      case CurAction3d_DynamicRotation:
           if (myHlrModeIsOn)
           {
	           myView->SetComputedMode (Standard_False);
           }
           myView->StartRotation( point.x(), point.y() );
           break;
	  case CurAction3d_DrawPoint:
		  break;
	  case CurAction3d_DrawLine:
		  break;
	  case CurAction3d_DrawWall:
		  break;
	  default:
	         Standard_Failure::Raise( "incompatible Current Mode" );
           break;
    }
  }
  activateCursor( myCurrentMode );
}

void View::onMButtonDown( const int/*Qt::MouseButtons*/ nFlags, const QPoint /*point*/ )
{
  if ( nFlags & CASCADESHORTCUTKEY )
    myCurrentMode = CurAction3d_DynamicPanning;
  activateCursor( myCurrentMode );
}

void View::onRButtonDown( const int/*Qt::MouseButtons*/ nFlags, const QPoint point )
{
  if ( nFlags & CASCADESHORTCUTKEY )
  {
    if (myHlrModeIsOn)
    {
      myView->SetComputedMode (Standard_False);
    }
    myCurrentMode = CurAction3d_DynamicRotation;
    myView->StartRotation( point.x(), point.y() );
  }
  else
  {
//    Popup( point.x(), point.y() );
  }
  activateCursor( myCurrentMode );
}

void View::onLButtonUp( Qt::MouseButtons nFlags, const QPoint point )
{
    switch( myCurrentMode )
    {
        case CurAction3d_Nothing:
            if ( point.x() == myXmin && point.y() == myYmin )
            {
	            // no offset between down and up --> selectEvent
	            myXmax = point.x();
	            myYmax = point.y();
	            if ( nFlags & MULTISELECTIONKEY )
	                MultiInputEvent( point.x(), point.y() );
              else
	                InputEvent( point.x(), point.y() );
            }
            else
            {
	            DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
	            myXmax = point.x();
	            myYmax = point.y();
	            if ( nFlags & MULTISELECTIONKEY )
	                MultiDragEvent( point.x(), point.y(), 1 );
              else
	                DragEvent( point.x(), point.y(), 1 );
            }
            break;
        case CurAction3d_DynamicZooming:
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case CurAction3d_WindowZooming:
            DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );//,LongDash);
            myXmax = point.x();
            myYmax = point.y();
            if ( (abs( myXmin - myXmax ) > ValZWMin ) ||
                 (abs( myYmin - myYmax ) > ValZWMin ) )
	            myView->WindowFitAll( myXmin, myYmin, myXmax, myYmax );
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case CurAction3d_DynamicPanning:
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case CurAction3d_GlobalPanning :
            myView->Place( point.x(), point.y(), myCurZoom );
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
        case CurAction3d_DynamicRotation:
            myCurrentMode = CurAction3d_Nothing;
            noActiveActions();
            break;
		case CurAction3d_DrawPoint:
			{
				Standard_Real x = point.x();
				Standard_Real y = point.y();
				Standard_Real xv, yv, zv;
				if(ConvertToPlane(myView, x, y, xv, yv, zv, Standard_False) == Standard_True)
				{
					Handle(Geom_CartesianPoint) gp = new Geom_CartesianPoint(xv, yv, zv);
					_point = new AIS_Point(gp);
					_point->SetMarker(Aspect_TOM_PLUS);
					myContext->Display(_point, 1);
				}
				myCurrentMode = CurAction3d_Nothing;
				noActiveActions();
			}
			break;
		case CurAction3d_DrawLine:
			{
				Standard_Real x = point.x();
				Standard_Real y = point.y();
				Standard_Real xv, yv, zv;
				if(ConvertToPlane(myView, x, y, xv, yv, zv, Standard_False) == Standard_True)
				{
					++_times_pressed;
					if(_times_pressed > 1)
					{
						_line->SetPoints(_start_point, _end_point);
						myContext->Redisplay(_line, Standard_True);

						_times_pressed = 0;
						myCurrentMode = CurAction3d_Nothing;
						noActiveActions();
					}
					else
					{
						_start_point = new Geom_CartesianPoint(xv, yv, zv);
						_end_point = new Geom_CartesianPoint(xv, yv, zv);
						_line = new AIS_Line(_start_point, _end_point);
						_line->SetColor(Quantity_NOC_YELLOW4);
						_line->SetWidth(5);
						myContext->Display(_line, Standard_True);
					}
				}
			}
			break;
		case CurAction3d_DrawWall:
//			{
			if(_wall._times_pressed > 0)
			{
				emit wall_end_construct(myView, gp_Pnt2d(point.x(), point.y()));

				_wall._times_pressed = 0;
				myCurrentMode = CurAction3d_Nothing;
				noActiveActions();
			}
			else
			{
				emit wall_begin_construct(myView, gp_Pnt2d(point.x(), point.y()));

				++_wall._times_pressed;
			}

//				Standard_Real x = point.x();
//				Standard_Real y = point.y();
//				Standard_Real xv, yv, zv;
//				if(ConvertToPlane(myView, x, y, xv, yv, zv, Standard_False) == Standard_True)
//				{
//					if(_wall._times_pressed > 0)
//					{
//						_wall.construct_wall(myContext, _wall._start, _wall._width, _wall._length, _wall._height);
//						//_wall.make_wall(myView, _wall._start, _wall._width, _wall._length, _wall._height);
//						//Handle(AIS_Shape) shape = new AIS_Shape(_wall._wall->Shell());
//						//myContext->Redisplay(shape, 1);
//
//						_wall._times_pressed = 0;
//						_wall._init_mode = Standard_False;
//						myCurrentMode = CurAction3d_Nothing;
//						noActiveActions();
//					}
//					else
//					{
////						TopoDS_Compound comp;
////						_wall.construct_wall(myContext, gp_Pnt(xv, yv, zv), 50, 200, 300);
//						//Handle(AIS_Shape) sh = new AIS_Shape(comp);
//						//myContext->Display(sh, 1);
//						_wall._init_mode = Standard_True;
//						_wall._start = gp_Pnt(xv, yv, zv);
//						_wall._old_dir = gp_Vec(_wall._start, gp_Pnt(xv, yv, zv));
//						_wall.construct_wall(myContext, _wall._start, _wall._width, _wall._length, _wall._height);
////						_wall.make_wall(myView, _wall._start, _wall._width, _wall._length, _wall._height);
////						_wall._shape = new AIS_Shape(_wall._wall->Shell());
////						myContext->Display(_wall._shape, 1);
//						++_wall._times_pressed;
//						_wall._init_mode = Standard_False;
//					}
//				}
//			}
			break;
        default:
	        Standard_Failure::Raise(" incompatible Current Mode ");
            break;
    }
    activateCursor( myCurrentMode );
    ApplicationCommonWindow::getApplication()->onSelectionChanged();
}

void View::onMButtonUp( Qt::MouseButtons /*nFlags*/, const QPoint /*point*/ )
{
    myCurrentMode = CurAction3d_Nothing;
    activateCursor( myCurrentMode );
}

void View::onRButtonUp( Qt::MouseButtons /*nFlags*/, const QPoint point )
{
    if ( myCurrentMode == CurAction3d_Nothing )
        Popup( point.x(), point.y() );
    else
    {
		if(myCurrentMode == CurAction3d_DrawLine)
		{
			myContext->Remove(_line, Standard_True);
		}
		else if(myCurrentMode == CurAction3d_DrawWall)
		{
			emit wall_cancel_construct();
			_wall._times_pressed = 0;
		}

		QApplication::setOverrideCursor( Qt::WaitCursor );
        // reset tyhe good Degenerated mode according to the strored one
        //   --> dynamic rotation may have change it
        myView->SetComputedMode (myHlrModeIsOn);
        QApplication::restoreOverrideCursor();
        myCurrentMode = CurAction3d_Nothing;
    }
    activateCursor( myCurrentMode );
}

void View::onMouseMove( Qt::MouseButtons nFlags, const QPoint point )
{
    if ( nFlags & Qt::LeftButton || nFlags & Qt::RightButton || nFlags & Qt::MidButton )
    {
    switch ( myCurrentMode )
    {
        case CurAction3d_Nothing:
	        myXmax = point.x();
	        myYmax = point.y();
	        DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
	        if ( nFlags & MULTISELECTIONKEY )
	            MultiDragEvent( myXmax, myYmax, 0 );
          else
	          DragEvent( myXmax, myYmax, 0 );
	          DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_True );
            break;
        case CurAction3d_DynamicZooming:
	        myView->Zoom( myXmax, myYmax, point.x(), point.y() );
          myXmax = point.x();
          myYmax = point.y();
	        break;
        case CurAction3d_WindowZooming:
	        myXmax = point.x();
          myYmax = point.y();
	        DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_False );
	        DrawRectangle( myXmin, myYmin, myXmax, myYmax, Standard_True );
	        break;
        case CurAction3d_DynamicPanning:
	        myView->Pan( point.x() - myXmax, myYmax - point.y() );
	        myXmax = point.x();
	        myYmax = point.y();
	        break;
        case CurAction3d_GlobalPanning:
          break;
        case CurAction3d_DynamicRotation:
	        myView->Rotation( point.x(), point.y() );
	        myView->Redraw();
          break;
		case CurAction3d_DrawPoint:
			break;
		case CurAction3d_DrawLine:
			break;
		case CurAction3d_DrawWall:
			break;
        default:
	        Standard_Failure::Raise( "incompatible Current Mode" );
          break;
    }
    }
    else
    {
		if(myCurrentMode == CurAction3d_DrawLine && _times_pressed > 0)
		{
			Standard_Real x = point.x();
			Standard_Real y = point.y();
			Standard_Real xv, yv, zv;
			if(ConvertToPlane(myView, x, y, xv, yv, zv, Standard_False) == Standard_True)
			{
				_end_point->SetCoord(xv, yv, zv);
				_line->SetPoints(_start_point, _end_point);
				myContext->Redisplay(_line, Standard_True);
			}
		}
		else if(myCurrentMode == CurAction3d_DrawWall && _wall._times_pressed > 0)
		{
			emit wall_constructing(myView, gp_Pnt2d(point.x(), point.y()));

//			Standard_Real x = point.x();
//			Standard_Real y = point.y();
//			Standard_Real xv, yv, zv;
//			if(ConvertToPlane(myView, x, y, xv, yv, zv, Standard_False) == Standard_True)
//			{
//				//_wall._width = xv - _wall._start.X();
//				_wall._length = yv - _wall._start.Y();
//				//_wall._height = zv - _wall._start.Z();
//				xv -= _wall._start.X();
//				yv -= _wall._start.Y(); 
//				zv -= _wall._start.Z();
//				_wall._old_dir = gp_Vec(_wall._start, gp_Pnt(xv, yv, zv));
//				_wall.construct_wall(myContext, _wall._start, _wall._width, _wall._length, _wall._height);
////				_wall.make_wall(myView, _wall._start, _wall._width, _wall._length, _wall._height);
////				myContext->Remove(_wall._shape, 0);
////				_wall._shape = new AIS_Shape(_wall._wall->Shell());
////				myContext->Display(_wall._shape, 1);
//			}
		}

		myXmax = point.x();
        myYmax = point.y();
	    if ( nFlags & MULTISELECTIONKEY )
	        MultiMoveEvent( point.x(), point.y() );
      else
	        MoveEvent( point.x(), point.y() );
    }
}

void View::DragEvent( const int x, const int y, const int TheState )
{
    // TheState == -1  button down
    // TheState ==  0  move
    // TheState ==  1  button up

    static Standard_Integer theButtonDownX = 0;
    static Standard_Integer theButtonDownY = 0;

    if ( TheState == -1 )
    {
        theButtonDownX = x;
        theButtonDownY = y;
    }

    if ( TheState == 1 )
    {
        myContext->Select( theButtonDownX, theButtonDownY, x, y, myView );
        emit selectionChanged();
    }
}

void View::InputEvent( const int /*x*/, const int /*y*/ )
{
  myContext->Select();
  emit selectionChanged();
}

void View::MoveEvent( const int x, const int y )
{
  myContext->MoveTo( x, y, myView );
}

void View::MultiMoveEvent( const int x, const int y )
{
  myContext->MoveTo( x, y, myView );
}

void View::MultiDragEvent( const int x, const int y, const int TheState )
{
    static Standard_Integer theButtonDownX = 0;
    static Standard_Integer theButtonDownY = 0;

    if ( TheState == -1 )
    {
        theButtonDownX = x;
        theButtonDownY = y;
    }
    if ( TheState == 0 )
    {
        myContext->ShiftSelect( theButtonDownX, theButtonDownY, x, y, myView );
        emit selectionChanged();
    }
}

void View::MultiInputEvent( const int /*x*/, const int /*y*/ )
{
  myContext->ShiftSelect();
  emit selectionChanged();
}

void View::Popup( const int /*x*/, const int /*y*/ )
{
  ApplicationCommonWindow* stApp = ApplicationCommonWindow::getApplication();
  QMdiArea* ws = ApplicationCommonWindow::getWorkspace();
  QMdiSubWindow* w = ws->activeSubWindow();
  if ( myContext->NbSelected() )
  {
    QList<QAction*>* aList = stApp->getToolActions();
    QMenu* myToolMenu = new QMenu( 0 );
		myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolWireframeId ) );
		myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolShadingId ) );
		myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolColorId ) );
        
    QMenu* myMaterMenu = new QMenu( myToolMenu );

    QList<QAction*>* aMeterActions = ApplicationCommonWindow::getApplication()->getMaterialActions();
        
    QString dir = ApplicationCommonWindow::getResourceDir() + QString( "/" );
		myMaterMenu = myToolMenu->addMenu( QPixmap( dir+QObject::tr("ICON_TOOL_MATER")), QObject::tr("MNU_MATER") );
    for ( int i = 0; i < aMeterActions->size(); i++ )
		  myMaterMenu->addAction( aMeterActions->at( i ) );
       
    myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolTransparencyId ) );
		myToolMenu->addAction( aList->at( ApplicationCommonWindow::ToolDeleteId ) );
    addItemInPopup(myToolMenu);
		myToolMenu->exec( QCursor::pos() );
    delete myToolMenu;
  }
  else
  {
    if (!myBackMenu)
    {
      myBackMenu = new QMenu( 0 );

		  QAction* a = new QAction( QObject::tr("MNU_CH_BACK"), this );
		  a->setToolTip( QObject::tr("TBR_CH_BACK") );
      connect( a, SIGNAL( activated() ), this, SLOT( onBackground() ) );
		  myBackMenu->addAction( a );  
      addItemInPopup(myBackMenu);

      a = new QAction( QObject::tr("MNU_CH_ENV_MAP"), this );
		  a->setToolTip( QObject::tr("TBR_CH_ENV_MAP") );
      connect( a, SIGNAL( activated() ), this, SLOT( onEnvironmentMap() ) );
      a->setCheckable( true );
      a->setChecked( false );
		  myBackMenu->addAction( a );  
      addItemInPopup(myBackMenu);
    }

    myBackMenu->exec( QCursor::pos() );
  }
  if ( w )
    w->setFocus();
}

void View::addItemInPopup( QMenu* /*theMenu*/)
{
}

void View::DrawRectangle(const int MinX, const int MinY,
			 const int MaxX, const int MaxY, const bool Draw)
{ 
  static Standard_Integer StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
  static Standard_Boolean m_IsVisible;

  StoredMinX = (MinX < MaxX) ? MinX: MaxX ;
  StoredMinY = (MinY < MaxY) ? MinY: MaxY ;
  StoredMaxX = (MinX > MaxX) ? MinX: MaxX ;
  StoredMaxY = (MinY > MaxY) ? MinY: MaxY ;

  QRect aRect;
  aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));

  if ( !myRectBand ) 
  {
    myRectBand = new QRubberBand( QRubberBand::Rectangle, this );
    myRectBand->setStyle( QStyleFactory::create("windows") );
    myRectBand->setGeometry( aRect );
    myRectBand->show();

    /*QPalette palette;
    palette.setColor(myRectBand->foregroundRole(), Qt::white);
    myRectBand->setPalette(palette);*/
  }

  if ( m_IsVisible && !Draw ) // move or up  : erase at the old position
  {
    myRectBand->hide();
    delete myRectBand;
    myRectBand = 0;
    m_IsVisible = false;
  }

  if (Draw) // move : draw
  {
    //aRect.setRect( StoredMinX, StoredMinY, abs(StoredMaxX-StoredMinX), abs(StoredMaxY-StoredMinY));
    m_IsVisible = true;
    myRectBand->setGeometry( aRect );
    //myRectBand->show();
  }
}

void View::noActiveActions()
{
    for ( int i = ViewFitAllId; i < ViewHlrOffId ; i++ )
    {
        QAction* anAction = myViewActions->at( i );
        if( ( anAction == myViewActions->at( ViewFitAreaId ) ) ||
            ( anAction == myViewActions->at( ViewZoomId ) ) ||
            ( anAction == myViewActions->at( ViewPanId ) ) ||
            ( anAction == myViewActions->at( ViewGlobalPanId ) ) ||
            ( anAction == myViewActions->at( ViewRotationId ) ) )
        {
            setCursor( *defCursor );
			      anAction->setCheckable( true );
			      anAction->setChecked( false );
        }
    }
}

void View::onBackground()
{
    QColor aColor ;
    Standard_Real R1;
    Standard_Real G1;
    Standard_Real B1;
    myView->BackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    aColor.setRgb(R1*255,G1*255,B1*255);

    QColor aRetColor = QColorDialog::getColor(aColor);

    if( aRetColor.isValid() )
    {
        R1 = aRetColor.red()/255.;
        G1 = aRetColor.green()/255.;
        B1 = aRetColor.blue()/255.;
        myView->SetBackgroundColor(Quantity_TOC_RGB,R1,G1,B1);
    }
    myView->Redraw();
}

void View::onEnvironmentMap()
{
  if (myBackMenu->actions().at(1)->isChecked())
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                           tr("All Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.tga)"));

    Handle(Graphic3d_TextureEnv) aTexture = new Graphic3d_TextureEnv( fileName.toLatin1().data() );

    myView->SetTextureEnv (aTexture);
    myView->SetSurfaceDetail (V3d_TEX_ENVIRONMENT);
  }
  else
  {
    myView->SetSurfaceDetail (V3d_TEX_NONE);
  }
  
  myView->Redraw();
}

bool View::dump(Standard_CString theFile)
{
  myView->Redraw();
  QString ext = QFileInfo( QString( theFile ) ).completeSuffix();
  if ( !ext.compare("ps") || !ext.compare("eps") || !ext.compare("tex") || !ext.compare("pdf") || !ext.compare("svg") || !ext.compare("pgf") )
  {
    Graphic3d_ExportFormat exFormat;
    if ( !ext.compare("ps") )
      exFormat = Graphic3d_EF_PostScript;
    if ( !ext.compare("eps") )
      exFormat = Graphic3d_EF_EnhPostScript;
    if ( !ext.compare("tex") )
      exFormat = Graphic3d_EF_TEX;
    if ( !ext.compare("pdf") )
      exFormat = Graphic3d_EF_PDF;
    if ( !ext.compare("svg") )
      exFormat = Graphic3d_EF_SVG;
    if ( !ext.compare("pgf") )
      exFormat = Graphic3d_EF_PGF;
    try
    {
      myView->View()->Export( theFile, exFormat );
    }
    catch(...)
    {
      return false;
    }
    return true;
  }
  return myView->Dump(theFile);
}

Handle(V3d_View)& View::getView()
{
  return myView;
}

Handle(AIS_InteractiveContext)& View::getContext()
{
  return myContext;
}

View::CurrentAction3d View::getCurrentMode()
{
  return myCurrentMode;
}



