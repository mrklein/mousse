// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef map_meshes_hpp_
#define map_meshes_hpp_
#include "map_vol_fields.hpp"
#include "map_consistent_vol_fields.hpp"
#include "map_lagrangian.hpp"
#include "un_mapped.hpp"
namespace mousse
{
template<template<class> class CombineOp>
void MapConsistentMesh
(
  const fvMesh& meshSource,
  const fvMesh& meshTarget,
  const meshToMesh0::order& mapOrder
)
{
  // Create the interpolation scheme
  meshToMesh0 meshToMesh0Interp(meshSource, meshTarget);
  Info<< nl
    << "Consistently creating and mapping fields for time "
    << meshSource.time().timeName() << nl << endl;
  {
    // Search for list of objects for this time
    IOobjectList objects{meshSource, meshSource.time().timeName()};
    // Map volFields
    // ~~~~~~~~~~~~~
    MapConsistentVolFields<scalar>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<scalar>()
    );
    MapConsistentVolFields<vector>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<vector>()
    );
    MapConsistentVolFields<sphericalTensor>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<sphericalTensor>()
    );
    MapConsistentVolFields<symmTensor>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<symmTensor>()
    );
    MapConsistentVolFields<tensor>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<tensor>()
    );
  }
  {
    // Search for list of target objects for this time
    IOobjectList objects{meshTarget, meshTarget.time().timeName()};
    // Mark surfaceFields as unmapped
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    UnMapped<surfaceScalarField>(objects);
    UnMapped<surfaceVectorField>(objects);
    UnMapped<surfaceSphericalTensorField>(objects);
    UnMapped<surfaceSymmTensorField>(objects);
    UnMapped<surfaceTensorField>(objects);
    // Mark pointFields as unmapped
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    UnMapped<pointScalarField>(objects);
    UnMapped<pointVectorField>(objects);
    UnMapped<pointSphericalTensorField>(objects);
    UnMapped<pointSymmTensorField>(objects);
    UnMapped<pointTensorField>(objects);
  }
  mapLagrangian(meshToMesh0Interp);
}
template<template<class> class CombineOp>
void MapSubMesh
(
  const fvMesh& meshSource,
  const fvMesh& meshTarget,
  const HashTable<word>& patchMap,
  const wordList& cuttingPatches,
  const meshToMesh0::order& mapOrder
)
{
  // Create the interpolation scheme
  meshToMesh0 meshToMesh0Interp
  {
    meshSource,
    meshTarget,
    patchMap,
    cuttingPatches
  };
  Info<< nl
    << "Mapping fields for time " << meshSource.time().timeName()
    << nl << endl;
  {
    // Search for list of source objects for this time
    IOobjectList objects{meshSource, meshSource.time().timeName()};
    // Map volFields
    // ~~~~~~~~~~~~~
    MapVolFields<scalar>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<scalar>()
    );
    MapVolFields<vector>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<vector>()
    );
    MapVolFields<sphericalTensor>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<sphericalTensor>()
    );
    MapVolFields<symmTensor>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<symmTensor>()
    );
    MapVolFields<tensor>
    (
      objects,
      meshToMesh0Interp,
      mapOrder,
      CombineOp<tensor>()
    );
  }
  {
    // Search for list of target objects for this time
    IOobjectList objects(meshTarget, meshTarget.time().timeName());
    // Mark surfaceFields as unmapped
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    UnMapped<surfaceScalarField>(objects);
    UnMapped<surfaceVectorField>(objects);
    UnMapped<surfaceSphericalTensorField>(objects);
    UnMapped<surfaceSymmTensorField>(objects);
    UnMapped<surfaceTensorField>(objects);
    // Mark pointFields as unmapped
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    UnMapped<pointScalarField>(objects);
    UnMapped<pointVectorField>(objects);
    UnMapped<pointSphericalTensorField>(objects);
    UnMapped<pointSymmTensorField>(objects);
    UnMapped<pointTensorField>(objects);
  }
  mapLagrangian(meshToMesh0Interp);
}
template<template<class> class CombineOp>
void MapConsistentSubMesh
(
  const fvMesh& meshSource,
  const fvMesh& meshTarget,
  const meshToMesh0::order& mapOrder
)
{
  HashTable<word> patchMap;
  HashTable<label> cuttingPatchTable;
  FOR_ALL(meshTarget.boundary(), patchi)
  {
    if (!isA<processorFvPatch>(meshTarget.boundary()[patchi]))
    {
      patchMap.insert
      (
        meshTarget.boundary()[patchi].name(),
        meshTarget.boundary()[patchi].name()
      );
    }
    else
    {
      cuttingPatchTable.insert
      (
        meshTarget.boundaryMesh()[patchi].name(),
        -1
      );
    }
  }
  MapSubMesh<CombineOp>
  (
    meshSource,
    meshTarget,
    patchMap,
    cuttingPatchTable.toc(),
    mapOrder
  );
}
}  // namespace mousse
#endif
