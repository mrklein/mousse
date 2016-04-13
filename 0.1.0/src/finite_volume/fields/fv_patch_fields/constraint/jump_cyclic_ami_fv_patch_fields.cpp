// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "jump_cyclic_ami_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"

namespace mousse {

// Static Data Members
MAKE_PATCH_FIELDS_TYPE_NAME(jumpCyclicAMI);

// Member Functions 
template<>
void mousse::jumpCyclicAMIFvPatchField<scalar>::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField& psiInternal,
  const scalarField& coeffs,
  const direction cmpt,
  const Pstream::commsTypes
) const
{
  const labelUList& nbrFaceCells =
    this->cyclicAMIPatch().cyclicAMIPatch().neighbPatch().faceCells();
  scalarField pnf{psiInternal, nbrFaceCells};
  pnf = this->cyclicAMIPatch().interpolate(pnf);
  // only apply jump to original field
  if (&psiInternal == &this->internalField()) {
    Field<scalar> jf{this->jump()};
    if (!this->cyclicAMIPatch().owner()) {
      jf *= -1.0;
    }
    pnf -= jf;
  }
  // Transform according to the transformation tensors
  this->transformCoupleField(pnf, cmpt);
  // Multiply the field by coefficients and add into the result
  const labelUList& faceCells = this->cyclicAMIPatch().faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}

}  // namespace mousse

