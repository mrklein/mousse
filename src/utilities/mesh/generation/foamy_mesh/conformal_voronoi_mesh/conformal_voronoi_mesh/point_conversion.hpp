#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_POINT_CONVERSION_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_POINT_CONVERSION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointConversion
// Description
//   Conversion functions between point (mousse::) and Point (CGAL::)

#include "point.hpp"


#ifdef CGAL_INEXACT
  // Define Point to be contiguous for inexact (double storage) kernel
  typedef const mousse::point& pointFromPoint;
  typedef const CGAL::Triangulation_vertex_base_3<K>::Point& PointFrompoint;
#else
  typedef mousse::point pointFromPoint;
  typedef CGAL::Triangulation_vertex_base_3<K>::Point PointFrompoint;
#endif


namespace mousse {

#ifdef CGAL_INEXACT
  template<typename Point>
  inline pointFromPoint topoint(const Point& P)
  {
    return reinterpret_cast<pointFromPoint>(P);
  }

  inline PointFrompoint toPoint(const mousse::point& p)
  {
    return reinterpret_cast<PointFrompoint>(p);
  }

#else
  template<typename Point>
  inline pointFromPoint topoint(const Point& P)
  {
    return mousse::point
    (
      CGAL::to_double(P.x()),
      CGAL::to_double(P.y()),
      CGAL::to_double(P.z())
    );
  }

  inline PointFrompoint toPoint(const mousse::point& p)
  {
    return PointFrompoint(p.x(), p.y(), p.z());
  }
#endif

  //- Specialisation for indexedVertex.
  template<>
  inline pointFromPoint topoint<CGAL::indexedVertex<K>>
  (
    const CGAL::indexedVertex<K>& P
  )
  {
    return topoint(P.point());
  }


  //- Specialisation for mousse::point. Used only as a dummy.
  template<>
  inline pointFromPoint topoint<mousse::point>
  (
    const mousse::point& P
  )
  {
    return P;
  }

}  // namespace mousse

#endif

