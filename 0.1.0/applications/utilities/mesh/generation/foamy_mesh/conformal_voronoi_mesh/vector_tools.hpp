#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_VECTOR_TOOLS_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_VECTOR_TOOLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::vectorTools
// Description
//   Functions for analysing the relationships between vectors

#include "vector.hpp"
#include "unit_conversion.hpp"


namespace mousse {
namespace vectorTools {

//- Test if a and b are parallel: a^b = 0
//  Uses the cross product, so the tolerance is proportional to
//  the sine of the angle between a and b in radians
template<typename T>
  bool areParallel
  (
    const Vector<T>& a,
    const Vector<T>& b,
    const T& tolerance = SMALL
  )
  {
    return (mag(a ^ b) < tolerance) ? true : false;
  }

//- Test if a and b are orthogonal: a.b = 0
//  Uses the dot product, so the tolerance is proportional to
//  the cosine of the angle between a and b in radians
template<typename T>
  bool areOrthogonal
  (
    const Vector<T>& a,
    const Vector<T>& b,
    const T& tolerance = SMALL
  )
  {
    return (mag(a & b) < tolerance) ? true : false;
  }

//- Test if angle between a and b is acute: a.b > 0
template<typename T>
  bool areAcute
  (
    const Vector<T>& a,
    const Vector<T>& b
  )
  {
    return ((a & b) > 0) ? true : false;
  }


//- Test if angle between a and b is obtuse: a.b < 0
template<typename T>
  bool areObtuse
  (
    const Vector<T>& a,
    const Vector<T>& b
  )
  {
    return ((a & b) < 0) ? true : false;
  }


//- Calculate angle between a and b in radians
template<typename T>
  T cosPhi
  (
    const Vector<T>& a,
    const Vector<T>& b,
    const T& tolerance = SMALL
  )
  {
    scalar cosPhi = (a & b)/(mag(a)*mag(b) + tolerance);
    // Enforce bounding between -1 and 1
    return min(max(cosPhi, -1), 1);
  }


//- Calculate angle between a and b in radians
template<typename T>
  T radAngleBetween
  (
    const Vector<T>& a,
    const Vector<T>& b,
    const T& tolerance = SMALL
  )
  {
    scalar cosPhi = (a & b)/(mag(a)*mag(b) + tolerance);
    // Enforce bounding between -1 and 1
    return acos( min(max(cosPhi, -1), 1) );
  }


//- Calculate angle between a and b in degrees
template<typename T>
  T degAngleBetween
  (
    const Vector<T>& a,
    const Vector<T>& b,
    const T& tolerance = SMALL
  )
  {
    return radToDeg(radAngleBetween(a, b, tolerance));
  }

}  // namespace vectorTools
}  // namespace mousse

#endif

