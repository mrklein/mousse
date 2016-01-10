// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tetPoints
// Description
//   Tet storage. Null constructable (unfortunately tetrahedron<point, point>
//   is not)
// SourceFiles
#ifndef tet_points_hpp_
#define tet_points_hpp_
#include "tetrahedron.hpp"
#include "fixed_list.hpp"
#include "tree_bound_box.hpp"
namespace mousse
{
class tetPoints
:
  public FixedList<point, 4>
{
public:
  // Constructors
    //- Construct null
    inline tetPoints()
    {}
    //- Construct from four points
    inline tetPoints
    (
      const point& a,
      const point& b,
      const point& c,
      const point& d
    )
    {
      operator[](0) = a;
      operator[](1) = b;
      operator[](2) = c;
      operator[](3) = d;
    }
  // Member Functions
    //- Return the tetrahedron
    inline tetPointRef tet() const
    {
      return tetPointRef
      (
        operator[](0),
        operator[](1),
        operator[](2),
        operator[](3)
      );
    }
    //- Calculate the bounding box
    inline treeBoundBox bounds() const
    {
      treeBoundBox bb(operator[](0), operator[](0));
      for (label i = 1; i < size(); i++)
      {
        bb.min() = min(bb.min(), operator[](i));
        bb.max() = max(bb.max(), operator[](i));
      }
      return bb;
    }
};
}  // namespace mousse
#endif
