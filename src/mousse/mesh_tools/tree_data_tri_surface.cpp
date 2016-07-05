// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tree_data_tri_surface.hpp"
#include "tri_surface_tools.hpp"


template<>
mousse::volumeType mousse::treeDataPrimitivePatch<mousse::triSurface>::getVolumeType
(
  const indexedOctree<treeDataPrimitivePatch<triSurface> >& oc,
  const point& sample
) const
{
  // Find nearest face to sample
  pointIndexHit info = oc.findNearest(sample, sqr(GREAT));
  if (info.index() == -1) {
    FATAL_ERROR_IN
    (
      "treeDataPrimitivePatch::getSampleType"
      "(indexedOctree<treeDataPrimitivePatch>&, const point&)"
    )
    << "Could not find " << sample << " in octree."
    << abort(FatalError);
  }
  // Get actual intersection point on face
  label faceI = info.index();
  triSurfaceTools::sideType t = triSurfaceTools::surfaceSide
  (
    patch_,
    sample,
    faceI
  );
  if (t == triSurfaceTools::UNKNOWN) {
    return volumeType::UNKNOWN;
  } else if (t == triSurfaceTools::INSIDE) {
    return volumeType::INSIDE;
  } else if (t == triSurfaceTools::OUTSIDE) {
    return volumeType::OUTSIDE;
  } else {
    FATAL_ERROR_IN("treeDataPrimitivePatch<PatchType>::getVolumeType(..)")
      << "problem" << abort(FatalError);
    return volumeType::UNKNOWN;
  }
}

