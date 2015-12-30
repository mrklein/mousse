// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(regionCoupledFvPatch, 0);
  addToRunTimeSelectionTable(fvPatch, regionCoupledFvPatch, polyPatch);
}
// Member Functions 
mousse::tmp<mousse::labelField> mousse::regionCoupledFvPatch::interfaceInternalField
(
  const labelUList& internalData
) const
{
  return patchInternalField(internalData);
}
mousse::tmp<mousse::labelField> mousse::regionCoupledFvPatch::internalFieldTransfer
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
    /*
    WarningIn
    (
      "regionCoupledFvPatch::internalFieldTransfer"
      "( const Pstream::commsTypes, const labelUList&)"
      " the internal field can not be transfered "
      " as the neighbFvPatch are in different meshes "
    );
    */
    return tmp<labelField>(new labelField(iF.size(), 0));
  }
  return tmp<labelField>(NULL);
}
