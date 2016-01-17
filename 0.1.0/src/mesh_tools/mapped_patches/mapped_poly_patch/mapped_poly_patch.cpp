// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "map_distribute.hpp"

namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(mappedPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, mappedPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, mappedPolyPatch, dictionary);
}

// Constructors
mousse::mappedPolyPatch::mappedPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch(name, size, start, index, bm, patchType),
  mappedPatchBase(static_cast<const polyPatch&>(*this))
{
  //  mapped is not constraint type so add mapped group explicitly
  if (findIndex(inGroups(), typeName) == -1)
  {
    inGroups().append(typeName);
  }
}
mousse::mappedPolyPatch::mappedPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const word& sampleRegion,
  const mappedPatchBase::sampleMode mode,
  const word& samplePatch,
  const vectorField& offset,
  const polyBoundaryMesh& bm
)
:
  polyPatch(name, size, start, index, bm, typeName),
  mappedPatchBase
  (
    static_cast<const polyPatch&>(*this),
    sampleRegion,
    mode,
    samplePatch,
    offset
  )
{}
mousse::mappedPolyPatch::mappedPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const word& sampleRegion,
  const mappedPatchBase::sampleMode mode,
  const word& samplePatch,
  const vector& offset,
  const polyBoundaryMesh& bm
)
:
  polyPatch(name, size, start, index, bm, typeName),
  mappedPatchBase
  (
    static_cast<const polyPatch&>(*this),
    sampleRegion,
    mode,
    samplePatch,
    offset
  )
{}
mousse::mappedPolyPatch::mappedPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch(name, dict, index, bm, patchType),
  mappedPatchBase(*this, dict)
{
  //  mapped is not constraint type so add mapped group explicitly
  if (findIndex(inGroups(), typeName) == -1)
  {
    inGroups().append(typeName);
  }
}
mousse::mappedPolyPatch::mappedPolyPatch
(
  const mappedPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  polyPatch(pp, bm),
  mappedPatchBase(*this, pp)
{}
mousse::mappedPolyPatch::mappedPolyPatch
(
  const mappedPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  polyPatch(pp, bm, index, newSize, newStart),
  mappedPatchBase(*this, pp)
{}
mousse::mappedPolyPatch::mappedPolyPatch
(
  const mappedPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  polyPatch(pp, bm, index, mapAddressing, newStart),
  mappedPatchBase(*this, pp, mapAddressing)
{}

// Destructor
mousse::mappedPolyPatch::~mappedPolyPatch()
{
  mappedPatchBase::clearOut();
}

// Member Functions
void mousse::mappedPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
  polyPatch::initGeometry(pBufs);
}
void mousse::mappedPolyPatch::calcGeometry(PstreamBuffers& pBufs)
{
  polyPatch::calcGeometry(pBufs);
  mappedPatchBase::clearOut();
}
void mousse::mappedPolyPatch::initMovePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  polyPatch::initMovePoints(pBufs, p);
}
void mousse::mappedPolyPatch::movePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  polyPatch::movePoints(pBufs, p);
  mappedPatchBase::clearOut();
}
void mousse::mappedPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  polyPatch::initUpdateMesh(pBufs);
}
void mousse::mappedPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
  polyPatch::updateMesh(pBufs);
  mappedPatchBase::clearOut();
}
void mousse::mappedPolyPatch::write(Ostream& os) const
{
  polyPatch::write(os);
  mappedPatchBase::write(os);
}
