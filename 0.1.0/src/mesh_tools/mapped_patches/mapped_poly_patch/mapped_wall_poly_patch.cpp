// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_wall_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "mapped_poly_patch.hpp"
namespace mousse
{
  defineTypeNameAndDebug(mappedWallPolyPatch, 0);
  addToRunTimeSelectionTable(polyPatch, mappedWallPolyPatch, word);
  addToRunTimeSelectionTable
  (
    polyPatch,
    mappedWallPolyPatch,
    dictionary
  );
}
// Constructors 
mousse::mappedWallPolyPatch::mappedWallPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  wallPolyPatch(name, size, start, index, bm, patchType),
  mappedPatchBase(static_cast<const polyPatch&>(*this))
{
  //  mapped is not constraint type so add mapped group explicitly
  if (findIndex(inGroups(), mappedPolyPatch::typeName) == -1)
  {
    inGroups().append(mappedPolyPatch::typeName);
  }
}
mousse::mappedWallPolyPatch::mappedWallPolyPatch
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
  wallPolyPatch(name, size, start, index, bm, typeName),
  mappedPatchBase
  (
    static_cast<const polyPatch&>(*this),
    sampleRegion,
    mode,
    samplePatch,
    offset
  )
{}
mousse::mappedWallPolyPatch::mappedWallPolyPatch
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
  wallPolyPatch(name, size, start, index, bm, typeName),
  mappedPatchBase
  (
    static_cast<const polyPatch&>(*this),
    sampleRegion,
    mode,
    samplePatch,
    offset
  )
{}
mousse::mappedWallPolyPatch::mappedWallPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  wallPolyPatch(name, dict, index, bm, patchType),
  mappedPatchBase(*this, dict)
{
  //  mapped is not constraint type so add mapped group explicitly
  if (findIndex(inGroups(), mappedPolyPatch::typeName) == -1)
  {
    inGroups().append(mappedPolyPatch::typeName);
  }
}
mousse::mappedWallPolyPatch::mappedWallPolyPatch
(
  const mappedWallPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  wallPolyPatch(pp, bm),
  mappedPatchBase(*this, pp)
{}
mousse::mappedWallPolyPatch::mappedWallPolyPatch
(
  const mappedWallPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  wallPolyPatch(pp, bm, index, newSize, newStart),
  mappedPatchBase(*this, pp)
{}
mousse::mappedWallPolyPatch::mappedWallPolyPatch
(
  const mappedWallPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  wallPolyPatch(pp, bm, index, mapAddressing, newStart),
  mappedPatchBase(*this, pp, mapAddressing)
{}
// Destructor 
mousse::mappedWallPolyPatch::~mappedWallPolyPatch()
{
  mappedPatchBase::clearOut();
}
// Member Functions 
void mousse::mappedWallPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
  wallPolyPatch::initGeometry(pBufs);
}
void mousse::mappedWallPolyPatch::calcGeometry(PstreamBuffers& pBufs)
{
  wallPolyPatch::calcGeometry(pBufs);
  mappedPatchBase::clearOut();
}
void mousse::mappedWallPolyPatch::initMovePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  wallPolyPatch::initMovePoints(pBufs, p);
}
void mousse::mappedWallPolyPatch::movePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  wallPolyPatch::movePoints(pBufs, p);
  mappedPatchBase::clearOut();
}
void mousse::mappedWallPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  wallPolyPatch::initUpdateMesh(pBufs);
}
void mousse::mappedWallPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
  wallPolyPatch::updateMesh(pBufs);
  mappedPatchBase::clearOut();
}
void mousse::mappedWallPolyPatch::write(Ostream& os) const
{
  wallPolyPatch::write(os);
  mappedPatchBase::write(os);
}
