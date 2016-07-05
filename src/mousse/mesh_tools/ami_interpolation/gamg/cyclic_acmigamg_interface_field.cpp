// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_acmigamg_interface_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ldu_matrix.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cyclicACMIGAMGInterfaceField, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  cyclicACMIGAMGInterfaceField,
  lduInterface
);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  cyclicACMIGAMGInterfaceField,
  lduInterfaceField
);

}


// Constructors 
mousse::cyclicACMIGAMGInterfaceField::cyclicACMIGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const lduInterfaceField& fineInterface
)
:
  GAMGInterfaceField{GAMGCp, fineInterface},
  cyclicACMIInterface_{refCast<const cyclicACMIGAMGInterface>(GAMGCp)},
  doTransform_{false},
  rank_{0}
{
  const cyclicAMILduInterfaceField& p =
    refCast<const cyclicAMILduInterfaceField>(fineInterface);
  doTransform_ = p.doTransform();
  rank_ = p.rank();
}


mousse::cyclicACMIGAMGInterfaceField::cyclicACMIGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const bool doTransform,
  const int rank
)
:
  GAMGInterfaceField{GAMGCp, doTransform, rank},
  cyclicACMIInterface_{refCast<const cyclicACMIGAMGInterface>(GAMGCp)},
  doTransform_{doTransform},
  rank_{rank}
{}


// Desstructor
mousse::cyclicACMIGAMGInterfaceField::~cyclicACMIGAMGInterfaceField()
{}


// Member Functions 
void mousse::cyclicACMIGAMGInterfaceField::updateInterfaceMatrix
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
  {
    cyclicACMIInterface_.neighbPatch().interfaceInternalField(psiInternal)
  };
  // Transform according to the transformation tensors
  transformCoupleField(pnf, cmpt);
  if (cyclicACMIInterface_.owner()) {
    pnf = cyclicACMIInterface_.AMI().interpolateToSource(pnf);
  } else {
    pnf = cyclicACMIInterface_.neighbPatch().AMI().interpolateToTarget(pnf);
  }
  const labelUList& faceCells = cyclicACMIInterface_.faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}

