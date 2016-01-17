// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   complex_fields.cpp
#ifndef complex_fields_hpp_
#define complex_fields_hpp_
#include "complex.hpp"
#include "complex_vector.hpp"
#include "primitive_fields.hpp"
namespace mousse
{
typedef Field<complex> complexField;
complexField ComplexField(const UList<scalar>&, const UList<scalar>&);
complexField ReComplexField(const UList<scalar>&);
complexField ImComplexField(const UList<scalar>&);
scalarField Re(const UList<complex>&);
scalarField Im(const UList<complex>&);
scalarField ReImSum(const UList<complex>&);
typedef Field<complexVector> complexVectorField;
complexVectorField ComplexField(const UList<vector>&, const UList<vector>&);
complexVectorField ReComplexField(const UList<vector>&);
complexVectorField ImComplexField(const UList<vector>&);
vectorField Re(const UList<complexVector>&);
vectorField Im(const UList<complexVector>&);
vectorField ReImSum(const UList<complexVector>&);
complexVectorField operator^
(
  const UList<vector>&,
  const UList<complexVector>&
);
}  // namespace mousse
#endif
