// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_wall_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  defineTypeNameAndDebug(regionCoupledWallFvPatch, 0);
  addToRunTimeSelectionTable(fvPatch, regionCoupledWallFvPatch, polyPatch);
}
// Member Functions 
mousse::tmp<mousse::labelField> mousse::regionCoupledWallFvPatch::
interfaceInternalField
(
  const labelUList& internalData
) const
{
  return patchInternalField(internalData);
}
mousse::tmp<mousse::labelField> mousse::regionCoupledWallFvPatch::
internalFieldTransfer
(
  const Pstream::commsTypes commsType,
  const labelUList& iF
) const
{
  if (neighbFvPatch().sameRegion())
  {
    return neighbFvPatch().patchInternalField(iF);
  }
  else
  {
    return tmp<labelField>(new labelField(iF.size(), 0));
  }
  return tmp<labelField>(NULL);
}
