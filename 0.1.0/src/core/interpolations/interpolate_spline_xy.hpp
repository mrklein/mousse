// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   interpolate_spline_xy.cpp

#ifndef interpolate_spline_xy_hpp_
#define interpolate_spline_xy_hpp_

#include "scalar.hpp"
#include "primitive_fields_fwd.hpp"

namespace mousse
{

template<class Type>
Field<Type> interpolateSplineXY
(
  const scalarField& xNew,
  const scalarField& xOld,
  const Field<Type>& yOld
);

template<class Type>
Type interpolateSplineXY
(
  const scalar x,
  const scalarField& xOld,
  const Field<Type>& yOld
);

}  // namespace mousse
#ifdef NoRepository
#   include "interpolate_spline_xy.cpp"
#endif
#endif
