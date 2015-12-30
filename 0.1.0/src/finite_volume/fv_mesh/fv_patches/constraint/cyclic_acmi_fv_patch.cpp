// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_acmi_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
#include "transform.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(cyclicACMIFvPatch, 0);
  addToRunTimeSelectionTable(fvPatch, cyclicACMIFvPatch, polyPatch);
}
// Protected Member Functions 
void mousse::cyclicACMIFvPatch::updateAreas() const
{
  if (cyclicACMIPolyPatch_.updated())
  {
    // Set Sf and magSf for both sides' coupled and non-overlapping patches
    // owner couple
    const_cast<vectorField&>(Sf()) = patch().faceAreas();
    const_cast<scalarField&>(magSf()) = mag(patch().faceAreas());
    // owner non-overlapping
    const fvPatch& nonOverlapPatch = this->nonOverlapPatch();
    const_cast<vectorField&>(nonOverlapPatch.Sf()) =
      nonOverlapPatch.patch().faceAreas();
    const_cast<scalarField&>(nonOverlapPatch.magSf()) =
      mag(nonOverlapPatch.patch().faceAreas());
    // neighbour couple
    const cyclicACMIFvPatch& nbrACMI = neighbPatch();
    const_cast<vectorField&>(nbrACMI.Sf()) =
      nbrACMI.patch().faceAreas();
    const_cast<scalarField&>(nbrACMI.magSf()) =
      mag(nbrACMI.patch().faceAreas());
    // neighbour non-overlapping
    const fvPatch& nbrNonOverlapPatch = nbrACMI.nonOverlapPatch();
    const_cast<vectorField&>(nbrNonOverlapPatch.Sf()) =
      nbrNonOverlapPatch.patch().faceAreas();
    const_cast<scalarField&>(nbrNonOverlapPatch.magSf()) =
      mag(nbrNonOverlapPatch.patch().faceAreas());
    // set the updated flag
    cyclicACMIPolyPatch_.setUpdated(false);
  }
}
void mousse::cyclicACMIFvPatch::makeWeights(scalarField& w) const
{
  if (coupled())
  {
    const cyclicACMIFvPatch& nbrPatch = neighbFvPatch();
    const fvPatch& nbrPatchNonOverlap = nonOverlapPatch();
    const scalarField deltas(nf() & coupledFvPatch::delta());
    const scalarField nbrDeltas
    (
      interpolate
      (
        nbrPatch.nf() & nbrPatch.coupledFvPatch::delta(),
        nbrPatchNonOverlap.nf() & nbrPatchNonOverlap.delta()
      )
    );
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
// Member Functions 
bool mousse::cyclicACMIFvPatch::coupled() const
{
  return Pstream::parRun() || (this->size() && neighbFvPatch().size());
}
mousse::tmp<mousse::vectorField> mousse::cyclicACMIFvPatch::delta() const
{
  if (coupled())
  {
    const cyclicACMIFvPatch& nbrPatchCoupled = neighbFvPatch();
    const fvPatch& nbrPatchNonOverlap = nonOverlapPatch();
    const vectorField patchD(coupledFvPatch::delta());
    vectorField nbrPatchD
    (
      interpolate
      (
        nbrPatchCoupled.coupledFvPatch::delta(),
        nbrPatchNonOverlap.delta()
      )
    );
    const vectorField nbrPatchD0
    (
      interpolate
      (
        vectorField(nbrPatchCoupled.size(), vector::zero),
        nbrPatchNonOverlap.delta()()
      )
    );
    nbrPatchD -= nbrPatchD0;
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
mousse::tmp<mousse::labelField> mousse::cyclicACMIFvPatch::interfaceInternalField
(
  const labelUList& internalData
) const
{
  return patchInternalField(internalData);
}
mousse::tmp<mousse::labelField> mousse::cyclicACMIFvPatch::internalFieldTransfer
(
  const Pstream::commsTypes commsType,
  const labelUList& iF
) const
{
  return neighbFvPatch().patchInternalField(iF);
}
