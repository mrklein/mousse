// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coord_set.hpp"


// Static Member Data 
namespace mousse {

template<>
const char* mousse::NamedEnum
<
  mousse::coordSet::coordFormat,
  5
>::names[] =
{
  "xyz",
  "x",
  "y",
  "z",
  "distance"
};

}

const mousse::NamedEnum<mousse::coordSet::coordFormat, 5>
  mousse::coordSet::coordFormatNames_;


// Constructors 

//- Construct from components
mousse::coordSet::coordSet
(
  const word& name,
  const word& axis
)
:
  pointField{0},
  name_{name},
  axis_{coordFormatNames_[axis]},
  curveDist_{0}
{}


//- Construct from components
mousse::coordSet::coordSet
(
  const word& name,
  const word& axis,
  const List<point>& points,
  const scalarList& curveDist
)
:
  pointField{points},
  name_{name},
  axis_{coordFormatNames_[axis]},
  curveDist_{curveDist}
{}


// Member Functions 
bool mousse::coordSet::hasVectorAxis() const
{
  return axis_ == XYZ;
}


mousse::scalar mousse::coordSet::scalarCoord
(
  const label index
)   const
{
  const point& p = operator[](index);
  if (axis_ == X) {
    return p.x();
  } else if (axis_ == Y) {
    return p.y();
  } else if (axis_ == Z) {
    return p.z();
  } else if (axis_ == DISTANCE) {
    // Use distance to reference point
    return curveDist_[index];
  } else {
    FATAL_ERROR_IN
    (
      "coordSet::scalarCoord(const label)"
    )
    << "Illegal axis specification " << axis_
    << " for sampling line " << name_
    << exit(FatalError);
    return 0;
  }
}


mousse::point mousse::coordSet::vectorCoord(const label index) const
{
  const point& p = operator[](index);
  return p;
}


mousse::Ostream& mousse::coordSet::write(Ostream& os) const
{
  os << "name:" << name_ << " axis:" << axis_
    << endl
    << endl << "\t(coord)"
    << endl;
  FOR_ALL(*this, sampleI) {
    os << '\t' << operator[](sampleI) << endl;
  }
  return os;
}
