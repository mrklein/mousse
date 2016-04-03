// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_coupled_poly_patch.hpp"
#include "poly_mesh.hpp"
#include "time.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(regionCoupledPolyPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, regionCoupledPolyPatch, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(polyPatch, regionCoupledPolyPatch, dictionary);

}


// Constructors 
mousse::regionCoupledPolyPatch::regionCoupledPolyPatch
(
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch{name, size, start, index, bm, patchType},
  regionCoupledBase{static_cast<const polyPatch&>(*this)}
{}


mousse::regionCoupledPolyPatch::regionCoupledPolyPatch
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm,
  const word& patchType
)
:
  polyPatch{name, dict, index, bm, patchType},
  regionCoupledBase{*this, dict}
{}


mousse::regionCoupledPolyPatch::regionCoupledPolyPatch
(
  const regionCoupledPolyPatch& pp,
  const polyBoundaryMesh& bm
)
:
  polyPatch{pp, bm},
  regionCoupledBase{*this, pp}
{}


mousse::regionCoupledPolyPatch::regionCoupledPolyPatch
(
  const regionCoupledPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const label newSize,
  const label newStart
)
:
  polyPatch{pp, bm, index, newSize, newStart},
  regionCoupledBase{*this, pp}
{}


mousse::regionCoupledPolyPatch::regionCoupledPolyPatch
(
  const regionCoupledPolyPatch& pp,
  const polyBoundaryMesh& bm,
  const label index,
  const labelUList& mapAddressing,
  const label newStart
)
:
  polyPatch{pp, bm, index, mapAddressing, newStart},
  regionCoupledBase{*this, pp}
{}


// Destructor 
mousse::regionCoupledPolyPatch::~regionCoupledPolyPatch()
{
  regionCoupledBase::clearGeom();
}


// Member Functions 
void mousse::regionCoupledPolyPatch::initGeometry(PstreamBuffers& pBufs)
{
  polyPatch::initGeometry(pBufs);
}


void mousse::regionCoupledPolyPatch::initMovePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  polyPatch::initMovePoints(pBufs, p);
}


void mousse::regionCoupledPolyPatch::movePoints
(
  PstreamBuffers& pBufs,
  const pointField& p
)
{
  polyPatch::movePoints(pBufs, p);
  regionCoupledBase::clearGeom();
}


void mousse::regionCoupledPolyPatch::initUpdateMesh(PstreamBuffers& pBufs)
{
  polyPatch::initUpdateMesh(pBufs);
}


void mousse::regionCoupledPolyPatch::updateMesh(PstreamBuffers& pBufs)
{
  polyPatch::updateMesh(pBufs);
  regionCoupledBase::clearGeom();
}


void mousse::regionCoupledPolyPatch::write(Ostream& os) const
{
  polyPatch::write(os);
  regionCoupledBase::write(os);
}

