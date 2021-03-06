// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh_tools.hpp"


// Adds patch if not yet there. Returns patchID.
mousse::label mousse::fvMeshTools::addPatch
(
  fvMesh& mesh,
  const polyPatch& patch,
  const dictionary& patchFieldDict,
  const word& defaultPatchFieldType,
  const bool validBoundary
)
{
  polyBoundaryMesh& polyPatches =
    const_cast<polyBoundaryMesh&>(mesh.boundaryMesh());
  label patchI = polyPatches.findPatchID(patch.name());
  if (patchI != -1) {
    // Already there
    return patchI;
  }
  // Append at end unless there are processor patches
  label insertPatchI = polyPatches.size();
  label startFaceI = mesh.nFaces();
  if (!isA<processorPolyPatch>(patch)) {
    FOR_ALL(polyPatches, patchI) {
      const polyPatch& pp = polyPatches[patchI];
      if (isA<processorPolyPatch>(pp)) {
        insertPatchI = patchI;
        startFaceI = pp.start();
        break;
      }
    }
  }
  // Below is all quite a hack. Feel free to change once there is a better
  // mechanism to insert and reorder patches.
  // Clear local fields and e.g. polyMesh parallelInfo.
  mesh.clearOut();
  label sz = polyPatches.size();
  fvBoundaryMesh& fvPatches = const_cast<fvBoundaryMesh&>(mesh.boundary());
  // Add polyPatch at the end
  polyPatches.setSize(sz+1);
  polyPatches.set
    (
      sz,
      patch.clone(polyPatches, insertPatchI, 0, startFaceI)
    );
  fvPatches.setSize(sz+1);
  fvPatches.set
    (
      sz,
      fvPatch::New(polyPatches[sz], mesh.boundary())
    );
  addPatchFields<volScalarField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<scalar>::zero
    );
  addPatchFields<volVectorField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<vector>::zero
    );
  addPatchFields<volSphericalTensorField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<sphericalTensor>::zero
    );
  addPatchFields<volSymmTensorField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<symmTensor>::zero
    );
  addPatchFields<volTensorField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<tensor>::zero
    );
  // Surface fields
  addPatchFields<surfaceScalarField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<scalar>::zero
    );
  addPatchFields<surfaceVectorField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<vector>::zero
    );
  addPatchFields<surfaceSphericalTensorField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<sphericalTensor>::zero
    );
  addPatchFields<surfaceSymmTensorField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<symmTensor>::zero
    );
  addPatchFields<surfaceTensorField>
    (
      mesh,
      patchFieldDict,
      defaultPatchFieldType,
      pTraits<tensor>::zero
    );
  // Create reordering list
  // patches before insert position stay as is
  labelList oldToNew{sz+1};
  for (label i = 0; i < insertPatchI; i++) {
    oldToNew[i] = i;
  }
  // patches after insert position move one up
  for (label i = insertPatchI; i < sz; i++) {
    oldToNew[i] = i+1;
  }
  // appended patch gets moved to insert position
  oldToNew[sz] = insertPatchI;
  // Shuffle into place
  polyPatches.reorder(oldToNew, validBoundary);
  fvPatches.reorder(oldToNew);
  reorderPatchFields<volScalarField>(mesh, oldToNew);
  reorderPatchFields<volVectorField>(mesh, oldToNew);
  reorderPatchFields<volSphericalTensorField>(mesh, oldToNew);
  reorderPatchFields<volSymmTensorField>(mesh, oldToNew);
  reorderPatchFields<volTensorField>(mesh, oldToNew);
  reorderPatchFields<surfaceScalarField>(mesh, oldToNew);
  reorderPatchFields<surfaceVectorField>(mesh, oldToNew);
  reorderPatchFields<surfaceSphericalTensorField>(mesh, oldToNew);
  reorderPatchFields<surfaceSymmTensorField>(mesh, oldToNew);
  reorderPatchFields<surfaceTensorField>(mesh, oldToNew);
  return insertPatchI;
}


void mousse::fvMeshTools::setPatchFields
(
  fvMesh& mesh,
  const label patchI,
  const dictionary& patchFieldDict
)
{
  setPatchFields<volScalarField>(mesh, patchI, patchFieldDict);
  setPatchFields<volVectorField>(mesh, patchI, patchFieldDict);
  setPatchFields<volSphericalTensorField>(mesh, patchI, patchFieldDict);
  setPatchFields<volSymmTensorField>(mesh, patchI, patchFieldDict);
  setPatchFields<volTensorField>(mesh, patchI, patchFieldDict);
  setPatchFields<surfaceScalarField>(mesh, patchI, patchFieldDict);
  setPatchFields<surfaceVectorField>(mesh, patchI, patchFieldDict);
  setPatchFields<surfaceSphericalTensorField>(mesh, patchI, patchFieldDict);
  setPatchFields<surfaceSymmTensorField>(mesh, patchI, patchFieldDict);
  setPatchFields<surfaceTensorField>(mesh, patchI, patchFieldDict);
}


void mousse::fvMeshTools::zeroPatchFields(fvMesh& mesh, const label patchI)
{
  setPatchFields<volScalarField>(mesh, patchI, pTraits<scalar>::zero);
  setPatchFields<volVectorField>(mesh, patchI, pTraits<vector>::zero);
  setPatchFields<volSphericalTensorField>(mesh, patchI,
                                          pTraits<sphericalTensor>::zero);
  setPatchFields<volSymmTensorField>(mesh, patchI, pTraits<symmTensor>::zero);
  setPatchFields<volTensorField>(mesh, patchI, pTraits<tensor>::zero);
  setPatchFields<surfaceScalarField>(mesh, patchI, pTraits<scalar>::zero);
  setPatchFields<surfaceVectorField>(mesh, patchI, pTraits<vector>::zero);
  setPatchFields<surfaceSphericalTensorField>(mesh, patchI,
                                              pTraits<sphericalTensor>::zero);
  setPatchFields<surfaceSymmTensorField>(mesh, patchI,
                                         pTraits<symmTensor>::zero);
  setPatchFields<surfaceTensorField>(mesh, patchI, pTraits<tensor>::zero);
}


// Deletes last patch
void mousse::fvMeshTools::trimPatches(fvMesh& mesh, const label nPatches)
{
  // Clear local fields and e.g. polyMesh globalMeshData.
  mesh.clearOut();
  polyBoundaryMesh& polyPatches =
    const_cast<polyBoundaryMesh&>(mesh.boundaryMesh());
  fvBoundaryMesh& fvPatches = const_cast<fvBoundaryMesh&>(mesh.boundary());
  if (polyPatches.empty()) {
    FATAL_ERROR_IN("fvMeshTools::trimPatches(fvMesh&, const label)")
      << "No patches in mesh"
      << abort(FatalError);
  }
  label nFaces = 0;
  for (label patchI = nPatches; patchI < polyPatches.size(); patchI++) {
    nFaces += polyPatches[patchI].size();
  }
  reduce(nFaces, sumOp<label>());
  if (nFaces) {
    FATAL_ERROR_IN("fvMeshTools::trimPatches(fvMesh&, const label)")
      << "There are still " << nFaces
      << " faces in " << polyPatches.size()-nPatches
      << " patches to be deleted" << abort(FatalError);
  }
  // Remove actual patches
  polyPatches.setSize(nPatches);
  fvPatches.setSize(nPatches);
  trimPatchFields<volScalarField>(mesh, nPatches);
  trimPatchFields<volVectorField>(mesh, nPatches);
  trimPatchFields<volSphericalTensorField>(mesh, nPatches);
  trimPatchFields<volSymmTensorField>(mesh, nPatches);
  trimPatchFields<volTensorField>(mesh, nPatches);
  trimPatchFields<surfaceScalarField>(mesh, nPatches);
  trimPatchFields<surfaceVectorField>(mesh, nPatches);
  trimPatchFields<surfaceSphericalTensorField>(mesh, nPatches);
  trimPatchFields<surfaceSymmTensorField>(mesh, nPatches);
  trimPatchFields<surfaceTensorField>(mesh, nPatches);
}


void mousse::fvMeshTools::reorderPatches
(
  fvMesh& mesh,
  const labelList& oldToNew,
  const label nNewPatches,
  const bool validBoundary
)
{
  polyBoundaryMesh& polyPatches = const_cast<polyBoundaryMesh&>(mesh.boundaryMesh());
  fvBoundaryMesh& fvPatches = const_cast<fvBoundaryMesh&>(mesh.boundary());
  // Shuffle into place
  polyPatches.reorder(oldToNew, validBoundary);
  fvPatches.reorder(oldToNew);
  reorderPatchFields<volScalarField>(mesh, oldToNew);
  reorderPatchFields<volVectorField>(mesh, oldToNew);
  reorderPatchFields<volSphericalTensorField>(mesh, oldToNew);
  reorderPatchFields<volSymmTensorField>(mesh, oldToNew);
  reorderPatchFields<volTensorField>(mesh, oldToNew);
  reorderPatchFields<surfaceScalarField>(mesh, oldToNew);
  reorderPatchFields<surfaceVectorField>(mesh, oldToNew);
  reorderPatchFields<surfaceSphericalTensorField>(mesh, oldToNew);
  reorderPatchFields<surfaceSymmTensorField>(mesh, oldToNew);
  reorderPatchFields<surfaceTensorField>(mesh, oldToNew);
  // Remove last.
  trimPatches(mesh, nNewPatches);
}

