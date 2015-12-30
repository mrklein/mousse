// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::STLpoint
// Description
//   A vertex point representation for STL files.
#ifndef stl_point_hpp_
#define stl_point_hpp_
#include "point.hpp"
#include "istream.hpp"
namespace mousse
{
class STLpoint
:
  public Vector<float>
{
public:
  // Constructors
    //- Construct null
    inline STLpoint()
    {}
    //- Construct from components
    inline STLpoint(float x, float y, float z)
    :
      Vector<float>(x, y, z)
    {}
    //- Construct from point
    inline STLpoint(const point& pt)
    :
      Vector<float>(float(pt.x()), float(pt.y()), float(pt.z()))
    {}
    //- Construct from istream
    inline STLpoint(Istream& is)
    :
      Vector<float>(is)
    {}
  // Member Operators
    //- Conversion to point
    inline operator point() const
    {
      return point(x(), y(), z());
    }
};
}  // namespace mousse
#endif
