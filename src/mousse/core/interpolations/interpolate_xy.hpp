#ifndef CORE_INTERPOLATIONS_INTERPOLATE_XY_HPP_
#define CORE_INTERPOLATIONS_INTERPOLATE_XY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar.hpp"
#include "primitive_fields_fwd.hpp"


namespace mousse {

template<class Type>
Field<Type> interpolateXY
(
  const scalarField& xNew,
  const scalarField& xOld,
  const Field<Type>& yOld
);


template<class Type>
Type interpolateXY
(
  const scalar x,
  const scalarField& xOld,
  const Field<Type>& yOld
);

}  // namespace mousse

#include "interpolate_xy.ipp"

#endif
