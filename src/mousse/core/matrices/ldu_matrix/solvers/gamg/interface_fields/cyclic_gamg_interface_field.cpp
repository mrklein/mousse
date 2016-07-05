// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_gamg_interface_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ldu_matrix.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cyclicGAMGInterfaceField, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  cyclicGAMGInterfaceField,
  lduInterface
);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  cyclicGAMGInterfaceField,
  lduInterfaceField
);

// Add under name cyclicSlip
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  cyclicGAMGInterfaceField,
  lduInterface,
  cyclicSlip
);

ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  cyclicGAMGInterfaceField,
  lduInterfaceField,
  cyclicSlip
);

}


// Constructors
mousse::cyclicGAMGInterfaceField::cyclicGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const lduInterfaceField& fineInterface
)
:
  GAMGInterfaceField{GAMGCp, fineInterface},
  cyclicInterface_{refCast<const cyclicGAMGInterface>(GAMGCp)},
  doTransform_{false},
  rank_{0}
{
  const cyclicLduInterfaceField& p =
    refCast<const cyclicLduInterfaceField>(fineInterface);
  doTransform_ = p.doTransform();
  rank_ = p.rank();
}


mousse::cyclicGAMGInterfaceField::cyclicGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const bool doTransform,
  const int rank
)
:
  GAMGInterfaceField{GAMGCp, doTransform, rank},
  cyclicInterface_{refCast<const cyclicGAMGInterface>(GAMGCp)},
  doTransform_{doTransform},
  rank_{rank}
{}


// Destructor
mousse::cyclicGAMGInterfaceField::~cyclicGAMGInterfaceField()
{}


// Member Functions
void mousse::cyclicGAMGInterfaceField::updateInterfaceMatrix
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
    cyclicInterface_.neighbPatch().interfaceInternalField(psiInternal)
  };
  transformCoupleField(pnf, cmpt);
  const labelUList& faceCells = cyclicInterface_.faceCells();
  FOR_ALL(faceCells, elemI) {
    result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
  }
}
