// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_mesh.hpp"
#include "transform.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(cyclicFvPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(fvPatch, cyclicFvPatch, polyPatch);
}
// Member Functions
void mousse::cyclicFvPatch::makeWeights(scalarField& w) const
{
  const cyclicFvPatch& nbrPatch = neighbFvPatch();
  const scalarField deltas(nf()&coupledFvPatch::delta());
  const scalarField nbrDeltas(nbrPatch.nf()&nbrPatch.coupledFvPatch::delta());
  FOR_ALL(deltas, facei)
  {
    scalar di = deltas[facei];
    scalar dni = nbrDeltas[facei];
    w[facei] = dni/(di + dni);
  }
}
mousse::tmp<mousse::vectorField> mousse::cyclicFvPatch::delta() const
{
  const vectorField patchD(coupledFvPatch::delta());
  const vectorField nbrPatchD(neighbFvPatch().coupledFvPatch::delta());
  tmp<vectorField> tpdv(new vectorField(patchD.size()));
  vectorField& pdv = tpdv();
  // To the transformation if necessary
  if (parallel())
  {
    FOR_ALL(patchD, facei)
    {
      vector ddi = patchD[facei];
      vector dni = nbrPatchD[facei];
      pdv[facei] = ddi - dni;
    }
  }
  else
  {
    FOR_ALL(patchD, facei)
    {
      vector ddi = patchD[facei];
      vector dni = nbrPatchD[facei];
      pdv[facei] = ddi - transform(forwardT()[0], dni);
    }
  }
  return tpdv;
}
mousse::tmp<mousse::labelField> mousse::cyclicFvPatch::interfaceInternalField
(
  const labelUList& internalData
) const
{
  return patchInternalField(internalData);
}
mousse::tmp<mousse::labelField> mousse::cyclicFvPatch::internalFieldTransfer
(
  const Pstream::commsTypes,
  const labelUList& iF
) const
{
  return neighbFvPatch().patchInternalField(iF);
}
