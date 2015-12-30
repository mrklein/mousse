// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "transform_field.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(processorFvPatch, 0);
  addToRunTimeSelectionTable(fvPatch, processorFvPatch, polyPatch);
}
// Member Functions 
void mousse::processorFvPatch::makeWeights(scalarField& w) const
{
  if (Pstream::parRun())
  {
    // The face normals point in the opposite direction on the other side
    scalarField neighbFaceCentresCn
    (
      (
        procPolyPatch_.neighbFaceAreas()
       /(mag(procPolyPatch_.neighbFaceAreas()) + VSMALL)
      )
     & (
       procPolyPatch_.neighbFaceCentres()
      - procPolyPatch_.neighbFaceCellCentres())
    );
    w = neighbFaceCentresCn
     /((nf()&coupledFvPatch::delta()) + neighbFaceCentresCn);
  }
  else
  {
    w = 1.0;
  }
}
mousse::tmp<mousse::vectorField> mousse::processorFvPatch::delta() const
{
  if (Pstream::parRun())
  {
    // To the transformation if necessary
    if (parallel())
    {
      return
        coupledFvPatch::delta()
       - (
          procPolyPatch_.neighbFaceCentres()
         - procPolyPatch_.neighbFaceCellCentres()
        );
    }
    else
    {
      return
        coupledFvPatch::delta()
       - transform
        (
          forwardT(),
          (
            procPolyPatch_.neighbFaceCentres()
           - procPolyPatch_.neighbFaceCellCentres()
          )
        );
    }
  }
  else
  {
    return coupledFvPatch::delta();
  }
}
mousse::tmp<mousse::labelField> mousse::processorFvPatch::interfaceInternalField
(
  const labelUList& internalData
) const
{
  return patchInternalField(internalData);
}
void mousse::processorFvPatch::initInternalFieldTransfer
(
  const Pstream::commsTypes commsType,
  const labelUList& iF
) const
{
  send(commsType, patchInternalField(iF)());
}
mousse::tmp<mousse::labelField> mousse::processorFvPatch::internalFieldTransfer
(
  const Pstream::commsTypes commsType,
  const labelUList&
) const
{
  return receive<label>(commsType, this->size());
}
