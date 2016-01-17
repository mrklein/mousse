// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_patch_mapper.hpp"
#include "point_patch.hpp"
#include "map_poly_mesh.hpp"
#include "face_mapper.hpp"
#include "demand_driven_data.hpp"
// Private Member Functions
void mousse::pointPatchMapper::calcAddressing() const
{
  if
  (
    directAddrPtr_
  || interpolationAddrPtr_
  || weightsPtr_
  )
  {
    FATAL_ERROR_IN
    (
      "void pointPatchMapper::calcAddressing() const"
    )
    << "Addressing already calculated"
    << abort(FatalError);
  }
  hasUnmapped_ = false;
  if (direct())
  {
    // Direct mapping.
    directAddrPtr_ = new labelList(mpm_.patchPointMap()[patch_.index()]);
    labelList& addr = *directAddrPtr_;
    FOR_ALL(addr, i)
    {
      if (addr[i] < 0)
      {
        hasUnmapped_ = true;
      }
    }
  }
  else
  {
    // Interpolative mapping.
    // NOTE: Incorrect:
    // FOR NOW only takes first patch point instead of averaging all
    // patch points. Problem is we don't know what points were in the patch
    // for points that were merged.
    interpolationAddrPtr_ = new labelListList(size());
    labelListList& addr = *interpolationAddrPtr_;
    weightsPtr_ = new scalarListList(addr.size());
    scalarListList& w = *weightsPtr_;
    const labelList& ppm = mpm_.patchPointMap()[patch_.index()];
    FOR_ALL(ppm, i)
    {
      if (ppm[i] >= 0)
      {
        addr[i] = labelList(1, ppm[i]);
        w[i] = scalarList(1, 1.0);
      }
      else
      {
        // Inserted point.
        ///// Map from point0 (arbitrary choice)
        //addr[i] = labelList(1, label(0));
        //w[i] = scalarList(1, 1.0);
        hasUnmapped_ = true;
      }
    }
  }
}
void mousse::pointPatchMapper::clearOut()
{
  deleteDemandDrivenData(directAddrPtr_);
  deleteDemandDrivenData(interpolationAddrPtr_);
  deleteDemandDrivenData(weightsPtr_);
  hasUnmapped_ = false;
}
// Constructors
// Construct from components
mousse::pointPatchMapper::pointPatchMapper
(
  const pointPatch& patch,
  const pointMapper& pointMap,
  const mapPolyMesh& mpm
)
:
  pointPatchFieldMapper{},
  patch_{patch},
  pointMapper_{pointMap},
  mpm_{mpm},
  sizeBeforeMapping_{
    patch_.index() < mpm_.oldPatchNMeshPoints().size()
      ? mpm_.oldPatchNMeshPoints()[patch_.index()]
      : 0
  },
  hasUnmapped_{false},
  directAddrPtr_{NULL},
  interpolationAddrPtr_{NULL},
  weightsPtr_{NULL}
{}

// Destructor
mousse::pointPatchMapper::~pointPatchMapper()
{
  clearOut();
}

// Member Functions
const mousse::labelUList& mousse::pointPatchMapper::directAddressing() const
{
  if (!direct())
  {
    FATAL_ERROR_IN
    (
      "const labelUList& pointPatchMapper::directAddressing() const"
    )
    << "Requested direct addressing for an interpolative mapper."
    << abort(FatalError);
  }
  if (!directAddrPtr_)
  {
    calcAddressing();
  }
  return *directAddrPtr_;
}
const mousse::labelListList& mousse::pointPatchMapper::addressing() const
{
  if (direct())
  {
    FATAL_ERROR_IN
    (
      "const labelListList& pointPatchMapper::addressing() const"
    )
    << "Requested interpolative addressing for a direct mapper."
    << abort(FatalError);
  }
  if (!interpolationAddrPtr_)
  {
    calcAddressing();
  }
  return *interpolationAddrPtr_;
}
const mousse::scalarListList& mousse::pointPatchMapper::weights() const
{
  if (direct())
  {
    FATAL_ERROR_IN
    (
      "const scalarListList& pointPatchMapper::weights() const"
    )
    << "Requested interpolative weights for a direct mapper."
    << abort(FatalError);
  }
  if (!weightsPtr_)
  {
    calcAddressing();
  }
  return *weightsPtr_;
}

