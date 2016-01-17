// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
inline mousse::label mousse::CV2D::insertPoint
(
  const point2D& p,
  const label type
)
{
  unsigned int nVert = number_of_vertices();
  return insertPoint(toPoint(p), nVert, type);
}
inline mousse::label mousse::CV2D::insertPoint
(
  const point2D& p,
  const label index,
  const label type
)
{
  return insertPoint(toPoint(p), index, type);
}
inline mousse::label mousse::CV2D::insertPoint
(
  const Point& p,
  const label index,
  const label type
)
{
  unsigned int nVert = number_of_vertices();
  Vertex_handle vh = insert(p);
  if (nVert == number_of_vertices())
  {
    WarningInFunction
      << "Failed to insert point " << toPoint2D(p) << endl;
  }
  else
  {
    vh->index() = index;
    vh->type() = type;
  }
  return vh->index();
}
inline bool mousse::CV2D::insertMirrorPoint
(
  const point2D& nearSurfPt,
  const point2D& surfPt
)
{
  point2D mirrorPoint(2*surfPt - nearSurfPt);
  if (qSurf_.outside(toPoint3D(mirrorPoint)))
  {
    insertPoint(mirrorPoint, Vb::MIRROR_POINT);
    return true;
  }
  else
  {
    return false;
  }
}
inline void mousse::CV2D::insertPointPair
(
  const scalar ppDist,
  const point2D& surfPt,
  const vector2D& n
)
{
  vector2D ppDistn = ppDist*n;
  label master = insertPoint
  (
    surfPt - ppDistn,
    number_of_vertices() + 1
  );
  insertPoint(surfPt + ppDistn, master);
}
// Member Functions 
inline const mousse::cv2DControls& mousse::CV2D::meshControls() const
{
  return controls_;
}
inline const mousse::point2D& mousse::CV2D::toPoint2D(const point& p) const
{
  return reinterpret_cast<const point2D&>(p);
}
inline const mousse::point2DField mousse::CV2D::toPoint2D(const pointField& p) const
{
  point2DField temp(p.size());
  forAll(temp, pointI)
  {
    temp[pointI] = point2D(p[pointI].x(), p[pointI].y());
  }
  return temp;
}
inline mousse::point mousse::CV2D::toPoint3D(const point2D& p) const
{
  return point(p.x(), p.y(), z_);
}
#ifdef CGAL_INEXACT
inline mousse::CV2D::point2DFromPoint mousse::CV2D::toPoint2D(const Point& P) const
{
  return reinterpret_cast<point2DFromPoint>(P);
}
inline mousse::CV2D::PointFromPoint2D mousse::CV2D::toPoint(const point2D& p) const
{
  return reinterpret_cast<PointFromPoint2D>(p);
}
#else
inline mousse::CV2D::point2DFromPoint mousse::CV2D::toPoint2D(const Point& P) const
{
  return point2D(CGAL::to_double(P.x()), CGAL::to_double(P.y()));
}
inline mousse::CV2D::PointFromPoint2D mousse::CV2D::toPoint(const point2D& p) const
{
  return Point(p.x(), p.y());
}
#endif
inline mousse::point mousse::CV2D::toPoint3D(const Point& P) const
{
  return point(CGAL::to_double(P.x()), CGAL::to_double(P.y()), z_);
}
inline void mousse::CV2D::movePoint(const Vertex_handle& vh, const Point& P)
{
  int i = vh->index();
  int t = vh->type();
  remove(vh);
  Vertex_handle newVh = insert(P);
  newVh->index() = i;
  newVh->type() = t;
  // label i = vh->index();
  // move(vh, P);
  // vh->index() = i;
  //vh->set_point(P);
  //fast_restore_Delaunay(vh);
}
// Friend Functions 
inline bool mousse::boundaryTriangle(const CV2D::Face_handle fc)
{
  return boundaryTriangle
  (
    *fc->vertex(0),
    *fc->vertex(1),
    *fc->vertex(2)
  );
}
inline bool mousse::outsideTriangle(const CV2D::Face_handle fc)
{
  return outsideTriangle
  (
    *fc->vertex(0),
    *fc->vertex(1),
    *fc->vertex(2)
  );
}
