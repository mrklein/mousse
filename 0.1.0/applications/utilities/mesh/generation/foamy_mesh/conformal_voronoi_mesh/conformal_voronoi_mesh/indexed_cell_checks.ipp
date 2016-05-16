// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "plane.hpp"
#include "tetrahedron.hpp"
#include "point_conversion.hpp"
#include "cgal_triangulation_3d_kernel.hpp"


template<typename Cell>
mousse::scalar mousse::foamyHexMeshChecks::coplanarTet
(
  Cell& c,
  const scalar tol
)
{
  tetPointRef tet
  {
    topoint(c->vertex(0)->point()),
    topoint(c->vertex(1)->point()),
    topoint(c->vertex(2)->point()),
    topoint(c->vertex(3)->point())
  };
  const scalar quality = tet.quality();
  if (quality < tol) {
    return quality;
  }
  return 0;
}


template<typename Cell>
bool mousse::foamyHexMeshChecks::closePoints
(
  Cell& c,
  const scalar tol
)
{
  for (label v = 0; v < 4; ++v) {
    for (label vA = v + 1; vA < 4; ++vA) {
      const auto& d = mag(topoint(c->vertex(v)->point())
                         - topoint(c->vertex(vA)->point()));
      if (d < tol) {
        return true;
      }
    }
  }
  return false;
}


template<typename Cell>
bool mousse::foamyHexMeshChecks::smallVolume
(
  Cell& c,
  const scalar tol
)
{
  CGAL::Tetrahedron_3<baseK> tet
  {
    c->vertex(0)->point(),
    c->vertex(1)->point(),
    c->vertex(2)->point(),
    c->vertex(3)->point()
  };
  if (tet.volume() < tol) {
    return true;
  }
  return false;
}

