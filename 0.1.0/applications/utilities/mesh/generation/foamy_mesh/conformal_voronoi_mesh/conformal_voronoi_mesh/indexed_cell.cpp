// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "indexed_cell.hpp"
//  IOStream operators
template<class Gt, class Cb>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const InfoProxy<CGAL::indexedCell<Gt, Cb> >& p
)
{
  const CGAL::indexedCell<Gt, Cb>& iv = p.t_;
  os  << "Cell: ";
  if (iv.index_ == CGAL::indexedCell<Gt, Cb>::ctFar)
  {
    os  << "far";
  }
  else if (iv.index_ >= 0)
  {
    os  << iv.index_;
  }
  else if (iv.index_ == CGAL::indexedCell<Gt, Cb>::ctInternal)
  {
    os  << "internal";
  }
  else if (iv.index_ == CGAL::indexedCell<Gt, Cb>::ctSurface)
  {
    os  << "surface";
  }
  else if (iv.index_ == CGAL::indexedCell<Gt, Cb>::ctFeatureEdge)
  {
    os  << "featureEdge";
  }
  else if (iv.index_ == CGAL::indexedCell<Gt, Cb>::ctFeaturePoint)
  {
    os  << "featurePoint";
  }
  else
  {
    os  << "unassigned";
  }
  if (iv.parallelDualVertex())
  {
    os  << " (processor)";
  }
  else
  {
    os  << " (local)";
  }
  os  << " filterCount: " << iv.filterCount_ << nl;
  os  << "    " << iv.vertex(0)->info();
  os  << "    " << iv.vertex(1)->info();
  os  << "    " << iv.vertex(2)->info();
  os  << "    " << iv.vertex(3)->info();
  return os;
}
