// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_wall_gamg_interface_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ldu_matrix.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(regionCoupledWallGAMGInterfaceField, 0);
  addToRunTimeSelectionTable
  (
    GAMGInterfaceField,
    regionCoupledWallGAMGInterfaceField,
    lduInterface
  );
  addToRunTimeSelectionTable
  (
    GAMGInterfaceField,
    regionCoupledWallGAMGInterfaceField,
    lduInterfaceField
  );
}
// Constructors 
mousse::regionCoupledWallGAMGInterfaceField::regionCoupledWallGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const lduInterfaceField& fineInterface
)
:
  GAMGInterfaceField(GAMGCp, fineInterface),
  regionCoupledGAMGInterface_
  (
    refCast<const regionCoupledWallGAMGInterface>(GAMGCp)
  )
{}
mousse::regionCoupledWallGAMGInterfaceField::regionCoupledWallGAMGInterfaceField
(
  const GAMGInterface& GAMGCp,
  const bool doTransform,
  const int rank
)
:
  GAMGInterfaceField(GAMGCp, doTransform, rank),
  regionCoupledGAMGInterface_
  (
    refCast<const regionCoupledWallGAMGInterface>(GAMGCp)
  )
{}
// Desstructor
mousse::regionCoupledWallGAMGInterfaceField::~regionCoupledWallGAMGInterfaceField
()
{}
