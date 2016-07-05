#ifndef CORE_MESHES_PRIMITIVE_MESH_PRIMITIVE_MESH_TOOLS_HPP_
#define CORE_MESHES_PRIMITIVE_MESH_PRIMITIVE_MESH_TOOLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "packed_bool_list.hpp"


namespace mousse {

class primitiveMeshTools
{
public:
  //- Generate non-orthogonality field (internal faces only)
  static tmp<scalarField> faceOrthogonality
  (
    const primitiveMesh& mesh,
    const vectorField& fAreas,
    const vectorField& cellCtrs
  );
  //- Generate face pyramid volume fields
  static void facePyramidVolume
  (
    const primitiveMesh& mesh,
    const pointField& points,
    const vectorField& cellCtrs,
    scalarField& ownPyrVol,
    scalarField& neiPyrVol
  );
  //- Generate skewness field
  static tmp<scalarField> faceSkewness
  (
    const primitiveMesh& mesh,
    const pointField& points,
    const vectorField& fCtrs,
    const vectorField& fAreas,
    const vectorField& cellCtrs
  );
  //- Generate cell openness and cell ascpect ratio field
  static void cellClosedness
  (
    const primitiveMesh& mesh,
    const Vector<label>& meshD,
    const vectorField& areas,
    const scalarField& vols,
    scalarField& openness,
    scalarField& aratio
  );
  //- Generate face concavity field. Returns per face the (sin of the)
  //  most concave angle between two consecutive edges
  static tmp<scalarField> faceConcavity
  (
    const scalar maxSin,
    const primitiveMesh& mesh,
    const pointField& p,
    const vectorField& faceAreas
  );
  //- Generate face flatness field. Compares the individual triangles'
  //  normals against the face average normal. Between 0 (fully warped)
  //  and 1 (fully flat)
  static tmp<scalarField> faceFlatness
  (
    const primitiveMesh& mesh,
    const pointField& p,
    const vectorField& fCtrs,
    const vectorField& faceAreas
  );
  //- Generate edge alignment field. Is per face the minimum aligned edge
  //  (does not use edge addressing)
  static tmp<scalarField> edgeAlignment
  (
    const primitiveMesh& mesh,
    const Vector<label>& directions,
    const pointField& p
  );
  //- Generate cell determinant field
  static tmp<scalarField> cellDeterminant
  (
    const primitiveMesh& mesh,
    const Vector<label>& directions,
    const vectorField& faceAreas,
    const PackedBoolList& internalOrCoupledFace
  );
  // Helpers: single face check
    //- Skewness of single face
    static scalar faceSkewness
    (
      const primitiveMesh& mesh,
      const pointField& p,
      const vectorField& fCtrs,
      const vectorField& fAreas,
      const label faceI,
      const point& ownCc,
      const point& neiCc
    );
    //- Skewness of single boundary face
    static scalar boundaryFaceSkewness
    (
      const primitiveMesh& mesh,
      const pointField& p,
      const vectorField& fCtrs,
      const vectorField& fAreas,
      const label faceI,
      const point& ownCc
    );
    //- Orthogonality of single face
    static scalar faceOrthogonality
    (
      const point& ownCc,
      const point& neiCc,
      const vector& s
    );
};

}  // namespace mousse

#endif
