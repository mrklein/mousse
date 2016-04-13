// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "jump_cyclic_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"


// Static Data Members
namespace mousse {

MAKE_PATCH_FIELDS_TYPE_NAME(jumpCyclic);

}


// Member Functions 
template<>
void mousse::jumpCyclicFvPatchField<mousse::scalar>::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField& psiInternal,
  const scalarField& coeffs,
  const direction cmpt,
  const Pstream::commsTypes
) const
{
  scalarField pnf{this->size()};
  const labelUList& nbrFaceCells =
    this->cyclicPatch().neighbFvPatch().faceCells();
  // only apply jump to original field
  if (&psiInternal == &this->internalField()) {
    Field<scalar> jf{this->jump()};
    if (!this->cyclicPatch().owner()) {
      jf *= -1.0;
    }
    FOR_ALL(*this, facei) {
      pnf[facei] = psiInternal[nbrFaceCells[facei]] - jf[facei];
    }
  } else {
    FOR_ALL(*this, facei) {
      pnf[facei] = psiInternal[nbrFaceCells[facei]];
    }
  }
  // Transform according to the transformation tensors
  this->transformCoupleField(pnf, cmpt);
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = this->cyclicPatch().faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}

