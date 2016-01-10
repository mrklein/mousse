// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "float_vector.hpp"
namespace mousse
{
// Static Data Members
template<>
const char* const floatVector::typeName = "floatVector";
template<>
const char* floatVector::componentNames[] = {"x", "y", "z"};
template<>
const floatVector floatVector::zero(0, 0, 0);
template<>
const floatVector floatVector::one(1, 1, 1);
template<>
const floatVector floatVector::max
(
  floatScalarVGREAT,
  floatScalarVGREAT,
  floatScalarVGREAT
);
template<>
const floatVector floatVector::min
(
  -floatScalarVGREAT,
  -floatScalarVGREAT,
  -floatScalarVGREAT
);
}  // namespace mousse
