// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uncorrected_sn_grad.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


namespace mousse {
namespace fv {

// Destructor 
template<class Type>
uncorrectedSnGrad<Type>::~uncorrectedSnGrad()
{}


// Member Functions 
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
uncorrectedSnGrad<Type>::correction
(
  const GeometricField<Type, fvPatchField, volMesh>&
) const
{
  NOT_IMPLEMENTED
  (
    "uncorrectedSnGrad<Type>::correction"
    "(const GeometricField<Type, fvPatchField, volMesh>&)"
  );

  return tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>{NULL};
}

}  // namespace fv
}  // namespace mousse

