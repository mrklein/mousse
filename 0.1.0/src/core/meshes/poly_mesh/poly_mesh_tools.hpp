// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   poly_mesh_tools.cpp
#ifndef poly_mesh_tools_hpp_
#define poly_mesh_tools_hpp_
#include "poly_mesh.hpp"
#include "primitive_mesh_tools.hpp"
namespace mousse
{
class polyMeshTools
:
  public primitiveMeshTools
{
public:
  //- Generate orthogonality field. (1 for fully orthogonal, < 1 for
  //  non-orthogonal)
  static tmp<scalarField> faceOrthogonality
  (
    const polyMesh& mesh,
    const vectorField& fAreas,
    const vectorField& cellCtrs
  );
  //- Generate skewness field
  static tmp<scalarField> faceSkewness
  (
    const polyMesh& mesh,
    const pointField& points,
    const vectorField& fCtrs,
    const vectorField& fAreas,
    const vectorField& cellCtrs
  );
  //- Generate interpolation factors field
  static tmp<scalarField> faceWeights
  (
    const polyMesh& mesh,
    const vectorField& fCtrs,
    const vectorField& fAreas,
    const vectorField& cellCtrs
  );
  //- Generate volume ratio field
  static tmp<scalarField> volRatio
  (
    const polyMesh& mesh,
    const scalarField& vol
  );
};
}  // namespace mousse
#endif
