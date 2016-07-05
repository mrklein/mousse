// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "indexed_vertex.hpp"
#include "point.hpp"
#include "istream.hpp"
#include "ostream.hpp"
#include "ostring_stream.hpp"
#include "istring_stream.hpp"


//  IOStream operators
mousse::Istream& mousse::operator>>
(
  Istream& is,
  CGAL::Point_3<baseK>& p
)
{
  mousse::point pt;
  is >> pt.x() >> pt.y() >> pt.z();
  p = CGAL::Point_3<baseK>(pt.x(), pt.y(), pt.z());
  return is;
}


mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const CGAL::Point_3<baseK>& p
)
{
  os << CGAL::to_double(p.x()) << ' '
    << CGAL::to_double(p.y()) << ' '
    << CGAL::to_double(p.z());
  return os;
}


template<class Gt, class Vb>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const CGAL::indexedVertex<Gt, Vb>& p
)
{
  os << p.point() << ' '
    << p.index() << ' '
    << static_cast<int>(p.type()) << ' '
    << p.procIndex() << ' '
    << p.alignment() << ' '
    << p.targetCellSize() << ' '
    << static_cast<int>(p.fixed());
  return os;
}


template<class Gt, class Vb>
mousse::Istream& mousse::operator>>
(
  Istream& is,
  CGAL::indexedVertex<Gt, Vb>& p
)
{
  is >> p.point() >> p.index();
  int type;
  is >> type;
  p.type() = static_cast<mousse::indexedVertexEnum::vertexType>(type);
  is >> p.procIndex() >> p.alignment() >> p.targetCellSize();
  int fixed;
  is >> fixed;
  p.fixed() = static_cast<bool>(fixed);
  return is;
}


template<class Gt, class Vb>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const InfoProxy<CGAL::indexedVertex<Gt, Vb>>& p
)
{
  const CGAL::indexedVertex<Gt, Vb>& iv = p.t_;
  const mousse::point pt(CGAL::to_double(iv.point().x()),
                         CGAL::to_double(iv.point().y()),
                         CGAL::to_double(iv.point().z()));
  string fixed(iv.vertexFixed_ ? string(" fixed, ") : string(" free, "));
  string referred
  (
    Pstream::myProcNo() == iv.processor_
    ? string(" (local)")
    : string(" (from " + name(iv.processor_) + ")")
  );
  os << iv.index_ << " "
    << CGAL::indexedVertex<Gt, Vb>::vertexTypeNames_[iv.type_]
    << " at:" << pt
    << " size:" << iv.targetCellSize_
    << " alignment:" << iv.alignment_
    << fixed
    << referred.c_str()
    << endl;
  return os;
}

