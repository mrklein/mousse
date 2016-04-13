// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation_cell.hpp"
#include "vol_fields.hpp"


namespace mousse {

// Constructor
template<class Type>
interpolationCell<Type>::interpolationCell
(
  const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
  interpolation<Type>{psi}
{}


// Member Functions 

template<class Type>
Type interpolationCell<Type>::interpolate
(
  const vector&,
  const label cellI,
  const label
) const
{
  return this->psi_[cellI];
}

}  // namespace mousse
