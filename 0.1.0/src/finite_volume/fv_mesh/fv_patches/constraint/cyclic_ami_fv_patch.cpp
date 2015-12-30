// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_ami_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
#include "transform.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(cyclicAMIFvPatch, 0);
  addToRunTimeSelectionTable(fvPatch, cyclicAMIFvPatch, polyPatch);
}
// Member Functions 
bool mousse::cyclicAMIFvPatch::coupled() const
{
  return Pstream::parRun() || (this->size() && neighbFvPatch().size());
}
void mousse::cyclicAMIFvPatch::makeWeights(scalarField& w) const
{
  if (coupled())
  {
    const cyclicAMIFvPatch& nbrPatch = neighbFvPatch();
    const scalarField deltas(nf() & coupledFvPatch::delta());
    tmp<scalarField> tnbrDeltas;
    if (applyLowWeightCorrection())
    {
      tnbrDeltas =
        interpolate
        (
          nbrPatch.nf() & nbrPatch.coupledFvPatch::delta(),
          scalarField(this->size(), 1.0)
        );
    }
    else
    {
      tnbrDeltas =
        interpolate(nbrPatch.nf() & nbrPatch.coupledFvPatch::delta());
    }
    const scalarField& nbrDeltas = tnbrDeltas();
    forAll(deltas, faceI)
    {
      scalar di = deltas[faceI];
      scalar dni = nbrDeltas[faceI];
      w[faceI] = dni/(di + dni);
    }
  }
  else
  {
    // Behave as uncoupled patch
    fvPatch::makeWeights(w);
  }
}
mousse::tmp<mousse::vectorField> mousse::cyclicAMIFvPatch::delta() const
{
  const cyclicAMIFvPatch& nbrPatch = neighbFvPatch();
  if (coupled())
  {
    const vectorField patchD(coupledFvPatch::delta());
    tmp<vectorField> tnbrPatchD;
    if (applyLowWeightCorrection())
    {
      tnbrPatchD =
        interpolate
        (
          nbrPatch.coupledFvPatch::delta(),
          vectorField(this->size(), vector::zero)
        );
    }
    else
    {
      tnbrPatchD = interpolate(nbrPatch.coupledFvPatch::delta());
    }
    const vectorField& nbrPatchD = tnbrPatchD();
    tmp<vectorField> tpdv(new vectorField(patchD.size()));
    vectorField& pdv = tpdv();
    // do the transformation if necessary
    if (parallel())
    {
      forAll(patchD, faceI)
      {
        const vector& ddi = patchD[faceI];
        const vector& dni = nbrPatchD[faceI];
        pdv[faceI] = ddi - dni;
      }
    }
    else
    {
      forAll(patchD, faceI)
      {
        const vector& ddi = patchD[faceI];
        const vector& dni = nbrPatchD[faceI];
        pdv[faceI] = ddi - transform(forwardT()[0], dni);
      }
    }
    return tpdv;
  }
  else
  {
    return coupledFvPatch::delta();
  }
}
mousse::tmp<mousse::labelField> mousse::cyclicAMIFvPatch::interfaceInternalField
(
  const labelUList& internalData
) const
{
  return patchInternalField(internalData);
}
mousse::tmp<mousse::labelField> mousse::cyclicAMIFvPatch::internalFieldTransfer
(
  const Pstream::commsTypes commsType,
  const labelUList& iF
) const
{
  return neighbFvPatch().patchInternalField(iF);
}
