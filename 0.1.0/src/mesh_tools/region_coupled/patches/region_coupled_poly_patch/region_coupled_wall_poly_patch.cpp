// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_wall_poly_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "poly_patch.hpp"
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(regionCoupledWallPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, regionCoupledWallPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  polyPatch,
  regionCoupledWallPolyPatch,
  dictionary
);
}
// Constructors 
mousse::regionCoupledWallPolyPatch::regionCoupledWallPolyPatch
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
  regionCoupledBase(static_cast<const polyPatch&>(*this))
{}
mousse::regionCoupledWallPolyPatch::regionCoupledWallPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  wallPolyPatch(name, dict, index, bm, patchType),
  regionCoupledBase(static_cast<const polyPatch&>(*this), dict)
{}
mousse::regionCoupledWallPolyPatch::regionCoupledWallPolyPatch
(
  const regionCoupledWallPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  wallPolyPatch(pp, bm),
  regionCoupledBase(*this, pp)
{}
mousse::regionCoupledWallPolyPatch::regionCoupledWallPolyPatch
(
  const regionCoupledWallPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  wallPolyPatch(pp, bm, index, newSize, newStart),
  regionCoupledBase(*this, pp)
{}
mousse::regionCoupledWallPolyPatch::regionCoupledWallPolyPatch
(
  const regionCoupledWallPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  wallPolyPatch(pp, bm, index, mapAddressing, newStart),
  regionCoupledBase(*this, pp)
{}
// Destructor 
mousse::regionCoupledWallPolyPatch::~regionCoupledWallPolyPatch()
{
  regionCoupledBase::clearGeom();
}
// Member Functions 
void mousse::regionCoupledWallPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
  wallPolyPatch::initGeometry(pBufs);
}
void mousse::regionCoupledWallPolyPatch::calcGeometry(PstreamBuffers& pBufs)
{
  wallPolyPatch::calcGeometry(pBufs);
  regionCoupledBase::clearGeom();
}
void mousse::regionCoupledWallPolyPatch::initMovePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  wallPolyPatch::initMovePoints(pBufs, p);
}
void mousse::regionCoupledWallPolyPatch::movePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  wallPolyPatch::movePoints(pBufs, p);
  regionCoupledBase::clearGeom();
}
void mousse::regionCoupledWallPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  wallPolyPatch::initUpdateMesh(pBufs);
}
void mousse::regionCoupledWallPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
  wallPolyPatch::updateMesh(pBufs);
  regionCoupledBase::clearGeom();
}
void mousse::regionCoupledWallPolyPatch::write(Ostream& os) const
{
  wallPolyPatch::write(os);
  regionCoupledBase::write(os);
}
