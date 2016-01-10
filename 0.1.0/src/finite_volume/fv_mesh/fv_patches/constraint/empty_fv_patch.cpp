// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "empty_fv_patch.hpp"
#include "fv_boundary_mesh.hpp"
#include "fv_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(emptyFvPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(fvPatch, emptyFvPatch, polyPatch);
// Constructors 
emptyFvPatch::emptyFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
:
  fvPatch(patch, bm),
  faceCells_
  (
    labelList::subList
    (
      boundaryMesh().mesh().faceOwner(), 0, patch.start()
    )
  )
{}
// Member Functions 
const labelUList& emptyFvPatch::faceCells() const
{
  return faceCells_;
}
}  // namespace mousse
