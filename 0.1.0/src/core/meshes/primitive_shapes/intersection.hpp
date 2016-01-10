// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::intersection
// Description
//   mousse::intersection
// SourceFiles
//   intersection.cpp
#ifndef intersection_hpp_
#define intersection_hpp_
#include "scalar.hpp"
#include "named_enum.hpp"
namespace mousse
{
class intersection
{
private:
  // Static data
    //- Relative tolerance for point in a plane.
    //  The tolerance is relative to the object size.
    //  Used to determine if a half-ray from a point in plane of triangle
    // intersects the triangle or not.
    static scalar planarTol_;
public:
  enum direction
  {
    VECTOR,
    CONTACT_SPHERE
  };
  enum algorithm
  {
    FULL_RAY,        // Intersecting with full ray
    HALF_RAY,        // Intersecting with half ray
    VISIBLE          // Intersecting with the visible side
  };
  // Static Member Functions
    //- Direction names
    static const NamedEnum<direction, 2> directionNames_;
    //- Projection algorithm names
    static const NamedEnum<algorithm, 3> algorithmNames_;
    //- Return planar tolerance
    static scalar planarTol()
    {
      return planarTol_;
    }
    //- Set the planar tolerance, returning the previous value
    static scalar setPlanarTol(const scalar t)
    {
      if (t < -VSMALL)
      {
        FATAL_ERROR_IN
        (
          "scalar intersection::setPlanarTol(const scalar t)"
        )   << "Negative planar tolerance.  This is not allowed."
          << abort(FatalError);
      }
      scalar oldTol = planarTol_;
      planarTol_ = t;
      return oldTol;
    }
};
}  // namespace mousse
#endif
