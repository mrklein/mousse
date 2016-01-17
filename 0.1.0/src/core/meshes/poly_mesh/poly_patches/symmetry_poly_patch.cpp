// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "symmetry_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(symmetryPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, symmetryPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, symmetryPolyPatch, dictionary);

}

// Constructors
mousse::symmetryPolyPatch::symmetryPolyPatch
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
{}
mousse::symmetryPolyPatch::symmetryPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch(name, dict, index, bm, patchType)
{}
mousse::symmetryPolyPatch::symmetryPolyPatch
(
  const symmetryPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  polyPatch(pp, bm)
{}
mousse::symmetryPolyPatch::symmetryPolyPatch
(
  const symmetryPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  polyPatch(pp, bm, index, newSize, newStart)
{}
mousse::symmetryPolyPatch::symmetryPolyPatch
(
  const symmetryPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  polyPatch(pp, bm, index, mapAddressing, newStart)
{}
