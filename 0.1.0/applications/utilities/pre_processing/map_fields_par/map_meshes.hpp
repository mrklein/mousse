#ifndef UTILITIES_PRE_PROCESSING_MAP_FIELDS_PAR_MAP_MESHES_HPP_
#define UTILITIES_PRE_PROCESSING_MAP_FIELDS_PAR_MAP_MESHES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "map_vol_fields.hpp"
#include "map_lagrangian.hpp"
#include "un_mapped.hpp"
#include "point_mesh.hpp"
namespace mousse
{
template<template<class> class CombineOp>
void MapMesh
(
  const meshToMesh& interp,
  const HashSet<word>& selectedFields,
  const bool noLagrangian
)
{
  {
    const polyMesh& meshSource = interp.srcRegion();
    // Search for list of objects for this time
    IOobjectList objects(meshSource, meshSource.time().timeName());
    // Map volFields
    // ~~~~~~~~~~~~~
    MapVolFields<scalar>
    (
      objects,
      selectedFields,
      interp,
      CombineOp<scalar>()
    );
    MapVolFields<vector>
    (
      objects,
      selectedFields,
      interp,
      CombineOp<vector>()
    );
    MapVolFields<sphericalTensor>
    (
      objects,
      selectedFields,
      interp,
      CombineOp<sphericalTensor>()
    );
    MapVolFields<symmTensor>
    (
      objects,
      selectedFields,
      interp,
      CombineOp<symmTensor>()
    );
    MapVolFields<tensor>
    (
      objects,
      selectedFields,
      interp,
      CombineOp<tensor>()
    );
  }
  {
    const polyMesh& meshTarget = interp.tgtRegion();
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
  if (!noLagrangian)
  {
    mapLagrangian(interp);
  }
}
}  // namespace mousse
#endif
