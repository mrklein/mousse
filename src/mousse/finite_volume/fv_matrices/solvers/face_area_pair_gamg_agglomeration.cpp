// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_area_pair_gamg_agglomeration.hpp"
#include "fv_mesh.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(faceAreaPairGAMGAgglomeration, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGAgglomeration,
  faceAreaPairGAMGAgglomeration,
  lduMesh
);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGAgglomeration,
  faceAreaPairGAMGAgglomeration,
  geometry
);

}


// Constructors 
mousse::faceAreaPairGAMGAgglomeration::faceAreaPairGAMGAgglomeration
(
  const lduMesh& mesh,
  const dictionary& controlDict
)
:
  pairGAMGAgglomeration{mesh, controlDict}
{
  const fvMesh& fvmesh = refCast<const fvMesh>(mesh);
  agglomerate
  (
    mesh,
    mag
    (
      cmptMultiply
      (
        fvmesh.Sf().internalField()/sqrt(fvmesh.magSf().internalField()),
        vector(1, 1.01, 1.02)
      )
    )
  );
}


mousse::faceAreaPairGAMGAgglomeration::faceAreaPairGAMGAgglomeration
(
  const lduMesh& mesh,
  const scalarField& /*cellVolumes*/,
  const vectorField& faceAreas,
  const dictionary& controlDict
)
:
  pairGAMGAgglomeration{mesh, controlDict}
{
  agglomerate
  (
    mesh,
    mag
    (
      cmptMultiply
      (
        faceAreas/sqrt(mag(faceAreas)),
        vector(1, 1.01, 1.02)
      )
    )
  );
}

