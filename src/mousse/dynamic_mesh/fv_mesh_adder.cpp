// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "fv_mesh_adder.hpp"
#include "face_couple_info.hpp"
#include "fv_mesh.hpp"


// Private Member Functions 
//- Calculate map from new patch faces to old patch faces. -1 where
//  could not map.
mousse::labelList mousse::fvMeshAdder::calcPatchMap
(
  const label oldStart,
  const label oldSize,
  const labelList& oldToNew,
  const polyPatch& newPatch,
  const label unmappedValue
)
{
  labelList newToOld{newPatch.size(), unmappedValue};
  label newStart = newPatch.start();
  label newSize = newPatch.size();
  for (label i = 0; i < oldSize; i++) {
    label newFaceI = oldToNew[oldStart+i];
    if (newFaceI >= newStart && newFaceI < newStart+newSize) {
      newToOld[newFaceI-newStart] = i;
    }
  }
  return newToOld;
}


// Member Functions 
// Inplace add mesh1 to mesh0
mousse::autoPtr<mousse::mapAddedPolyMesh> mousse::fvMeshAdder::add
(
  fvMesh& mesh0,
  const fvMesh& mesh1,
  const faceCoupleInfo& coupleInfo,
  const bool validBoundary
)
{
  mesh0.clearOut();
  // Resulting merged mesh (polyMesh only!)
  autoPtr<mapAddedPolyMesh>
    mapPtr{polyMeshAdder::add(mesh0, mesh1, coupleInfo, validBoundary)};
  // Adjust the fvMesh part.
  const polyBoundaryMesh& patches = mesh0.boundaryMesh();
  fvBoundaryMesh& fvPatches = const_cast<fvBoundaryMesh&>(mesh0.boundary());
  fvPatches.setSize(patches.size());
  FOR_ALL(patches, patchI) {
    fvPatches.set(patchI, fvPatch::New(patches[patchI], fvPatches));
  }
  // Do the mapping of the stored fields
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  fvMeshAdder::MapVolFields<scalar>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapVolFields<vector>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapVolFields<sphericalTensor>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapVolFields<symmTensor>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapVolFields<tensor>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapSurfaceFields<scalar>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapSurfaceFields<vector>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapSurfaceFields<sphericalTensor>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapSurfaceFields<symmTensor>(mapPtr, mesh0, mesh1);
  fvMeshAdder::MapSurfaceFields<tensor>(mapPtr, mesh0, mesh1);
  return mapPtr;
}

