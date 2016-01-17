// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(wallPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, wallPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, wallPolyPatch, dictionary);

}

// Constructors
mousse::wallPolyPatch::wallPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch(name, size, start, index, bm, patchType)
{
  //  wall is not constraint type so add wall group explicitly
  if (findIndex(inGroups(), typeName) == -1)
  {
    inGroups().append(typeName);
  }
}
mousse::wallPolyPatch::wallPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch(name, dict, index, bm, patchType)
{
  //  wall is not constraint type so add wall group explicitly
  if (findIndex(inGroups(), typeName) == -1)
  {
    inGroups().append(typeName);
  }
}
mousse::wallPolyPatch::wallPolyPatch
(
  const wallPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  polyPatch(pp, bm)
{}
mousse::wallPolyPatch::wallPolyPatch
(
  const wallPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  polyPatch(pp, bm, index, newSize, newStart)
{}
mousse::wallPolyPatch::wallPolyPatch
(
  const wallPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  polyPatch(pp, bm, index, mapAddressing, newStart)
{}
