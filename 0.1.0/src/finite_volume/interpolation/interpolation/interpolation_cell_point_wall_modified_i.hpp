// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type>
inline Type mousse::interpolationCellPointWallModified<Type>::interpolate
(
  const cellPointWeightWallModified& cpw
) const
{
  const List<scalar>& weights = cpw.weights();
  const List<label>& faceVertices = cpw.faceVertices();
  Type t = this->psi_[cpw.cell()]*weights[0];
  t += this->psip_[faceVertices[0]]*weights[1];
  t += this->psip_[faceVertices[1]]*weights[2];
  t += this->psip_[faceVertices[2]]*weights[3];
  return t;
}
template<class Type>
inline Type mousse::interpolationCellPointWallModified<Type>::interpolate
(
  const vector& position,
  const label cellI,
  const label faceI
) const
{
  return interpolate
  (
    cellPointWeightWallModified
    (
      this->pMesh_,
      position,
      cellI,
      faceI
    )
  );
}
template<class Type>
inline Type mousse::interpolationCellPointWallModified<Type>::interpolate
(
  const vector& position,
  const tetIndices& tetIs,
  const label faceI
) const
{
  if (faceI >= 0)
  {
    if (faceI != tetIs.face())
    {
      FatalErrorIn
      (
        "inline Type "
        "mousse::interpolationCellPointWallModifie<Type>::interpolate"
        "("
          "const vector& position, "
          "const tetIndices& tetIs, "
          "const label faceI"
        ") const"
      )
        << "specified face " << faceI << " inconsistent with the face "
        << "stored by tetIndices: " << tetIs.face()
        << exit(FatalError);
    }
    const polyBoundaryMesh& bm = this->pMesh_.boundaryMesh();
    label patchI = bm.whichPatch(faceI);
    if (patchI != -1)
    {
      if (isA<wallPolyPatch>(bm[patchI]))
      {
        Type t = this->psi_[tetIs.cell()];
        return t;
      }
    }
  }
  // If the wall face selection did not return, then use the normal
  // interpolate method
  return interpolationCellPoint<Type>::interpolate
  (
    position,
    tetIs,
    faceI
  );
}
