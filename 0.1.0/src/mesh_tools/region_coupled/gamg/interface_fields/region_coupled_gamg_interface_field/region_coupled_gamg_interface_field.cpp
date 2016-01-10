// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_gamg_interface_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ldu_matrix.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(regionCoupledGAMGInterfaceField, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  regionCoupledGAMGInterfaceField,
  lduInterface
);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGInterfaceField,
  regionCoupledGAMGInterfaceField,
  lduInterfaceField
);
}
// Constructors 
mousse::regionCoupledGAMGInterfaceField::regionCoupledGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const lduInterfaceField& fineInterface
)
:
  GAMGInterfaceField(GAMGCp, fineInterface),
  regionCoupledGAMGInterface_
  (
    refCast<const regionCoupledGAMGInterface>(GAMGCp)
  )
{}
mousse::regionCoupledGAMGInterfaceField::regionCoupledGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const bool doTransform,
  const int rank
)
:
  GAMGInterfaceField(GAMGCp, doTransform, rank),
  regionCoupledGAMGInterface_
  (
    refCast<const regionCoupledGAMGInterface>(GAMGCp)
  )
{}
// Desstructor
mousse::regionCoupledGAMGInterfaceField::~regionCoupledGAMGInterfaceField()
{}
