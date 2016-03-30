// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_cyclic_point_patch.hpp"
#include "point_boundary_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(processorCyclicPointPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  facePointPatch,
  processorCyclicPointPatch,
  polyPatch
);

// Constructors 
processorCyclicPointPatch::processorCyclicPointPatch
(
  const polyPatch& patch,
  const pointBoundaryMesh& bm
)
:
  processorPointPatch{patch, bm},
  procCycPolyPatch_{refCast<const processorCyclicPolyPatch>(patch)}
{}


// Destructor 
processorCyclicPointPatch::~processorCyclicPointPatch()
{}

}  // namespace mousse
