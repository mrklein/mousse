// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_wall_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(regionCoupledWallFvPatch, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(fvPatch, regionCoupledWallFvPatch, polyPatch);
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
  const Pstream::commsTypes,
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
