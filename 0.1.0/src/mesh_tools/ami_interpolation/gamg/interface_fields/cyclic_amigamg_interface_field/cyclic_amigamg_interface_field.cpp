// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_amigamg_interface_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ldu_matrix.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(cyclicAMIGAMGInterfaceField, 0);
  addToRunTimeSelectionTable
  (
    GAMGInterfaceField,
    cyclicAMIGAMGInterfaceField,
    lduInterface
  );
  addToRunTimeSelectionTable
  (
    GAMGInterfaceField,
    cyclicAMIGAMGInterfaceField,
    lduInterfaceField
  );
}
// Constructors 
mousse::cyclicAMIGAMGInterfaceField::cyclicAMIGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const lduInterfaceField& fineInterface
)
:
  GAMGInterfaceField(GAMGCp, fineInterface),
  cyclicAMIInterface_(refCast<const cyclicAMIGAMGInterface>(GAMGCp)),
  doTransform_(false),
  rank_(0)
{
  const cyclicAMILduInterfaceField& p =
    refCast<const cyclicAMILduInterfaceField>(fineInterface);
  doTransform_ = p.doTransform();
  rank_ = p.rank();
}
mousse::cyclicAMIGAMGInterfaceField::cyclicAMIGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const bool doTransform,
  const int rank
)
:
  GAMGInterfaceField(GAMGCp, doTransform, rank),
  cyclicAMIInterface_(refCast<const cyclicAMIGAMGInterface>(GAMGCp)),
  doTransform_(doTransform),
  rank_(rank)
{}
// Desstructor
mousse::cyclicAMIGAMGInterfaceField::~cyclicAMIGAMGInterfaceField()
{}
// Member Functions 
void mousse::cyclicAMIGAMGInterfaceField::updateInterfaceMatrix
(
  scalarField& result,
  const scalarField& psiInternal,
  const scalarField& coeffs,
  const direction cmpt,
  const Pstream::commsTypes
) const
{
  // Get neighbouring field
  scalarField pnf
  (
    cyclicAMIInterface_.neighbPatch().interfaceInternalField(psiInternal)
  );
  // Transform according to the transformation tensors
  transformCoupleField(pnf, cmpt);
  if (cyclicAMIInterface_.owner())
  {
    pnf = cyclicAMIInterface_.AMI().interpolateToSource(pnf);
  }
  else
  {
    pnf = cyclicAMIInterface_.neighbPatch().AMI().interpolateToTarget(pnf);
  }
  const labelUList& faceCells = cyclicAMIInterface_.faceCells();
  forAll(faceCells, elemI)
  {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}
