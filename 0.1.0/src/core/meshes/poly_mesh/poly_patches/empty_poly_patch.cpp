// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "empty_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(emptyPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, emptyPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, emptyPolyPatch, dictionary);

}

// Constructors
mousse::emptyPolyPatch::emptyPolyPatch
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
mousse::emptyPolyPatch::emptyPolyPatch
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
mousse::emptyPolyPatch::emptyPolyPatch
(
  const emptyPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  polyPatch(pp, bm)
{}
mousse::emptyPolyPatch::emptyPolyPatch
(
  const emptyPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  polyPatch(pp, bm, index, newSize, newStart)
{}
mousse::emptyPolyPatch::emptyPolyPatch
(
  const emptyPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  polyPatch(pp, bm, index, mapAddressing, newStart)
{}
