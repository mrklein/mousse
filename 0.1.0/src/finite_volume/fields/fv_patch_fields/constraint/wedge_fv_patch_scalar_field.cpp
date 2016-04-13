// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wedge_fv_patch_field.hpp"
#include "vol_fields.hpp"


namespace mousse {

// Member Functions 
template<>
tmp<scalarField> wedgeFvPatchField<scalar>::snGrad() const
{
  return tmp<scalarField>{new scalarField{size(), 0.0}};
}


template<>
void wedgeFvPatchField<scalar>::evaluate(const Pstream::commsTypes)
{
  if (!updated()) {
    updateCoeffs();
  }
  this->operator==(patchInternalField());
}

}  // namespace mousse

