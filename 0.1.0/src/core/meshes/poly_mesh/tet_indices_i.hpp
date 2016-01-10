// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
mousse::label mousse::tetIndices::cell() const
{
  return cellI_;
}
mousse::label mousse::tetIndices::face() const
{
  return faceI_;
}
mousse::label mousse::tetIndices::faceBasePt() const
{
  return faceBasePtI_;
}
mousse::label mousse::tetIndices::facePtA() const
{
  return facePtAI_;
}
mousse::label mousse::tetIndices::facePtB() const
{
  return facePtBI_;
}
mousse::label mousse::tetIndices::tetPt() const
{
  return tetPtI_;
}
mousse::tetPointRef mousse::tetIndices::tet(const polyMesh& mesh) const
{
  const pointField& pPts = mesh.points();
  const faceList& pFaces = mesh.faces();
  const vectorField& pC = mesh.cellCentres();
  const mousse::face& f = pFaces[faceI_];
  return tetPointRef
  (
    pC[cellI_],
    pPts[f[faceBasePtI_]],
    pPts[f[facePtAI_]],
    pPts[f[facePtBI_]]
  );
}
mousse::tetPointRef mousse::tetIndices::oldTet(const polyMesh& mesh) const
{
  const pointField& oldPPts = mesh.oldPoints();
  const faceList& pFaces = mesh.faces();
  // We need to reconstruct the old Cc from oldPoints (it isn't
  // stored)
  point oldC = mesh.cells()[cellI_].centre
  (
    oldPPts,
    pFaces
  );
  const mousse::face& f = pFaces[faceI_];
  return tetPointRef
  (
    oldC,
    oldPPts[f[faceBasePtI_]],
    oldPPts[f[facePtAI_]],
    oldPPts[f[facePtBI_]]
  );
}
mousse::triPointRef mousse::tetIndices::faceTri(const polyMesh& mesh) const
{
  const pointField& pPts = mesh.points();
  const faceList& pFaces = mesh.faces();
  const mousse::face& f = pFaces[faceI_];
  return triPointRef
  (
    pPts[f[faceBasePtI_]],
    pPts[f[facePtAI_]],
    pPts[f[facePtBI_]]
  );
}
mousse::triFace mousse::tetIndices::faceTriIs(const polyMesh& mesh) const
{
  const faceList& pFaces = mesh.faces();
  const mousse::face& f = pFaces[faceI_];
  return triFace
  (
    f[faceBasePtI_],
    f[facePtAI_],
    f[facePtBI_]
  );
}
mousse::triPointRef mousse::tetIndices::oldFaceTri(const polyMesh& mesh) const
{
  const pointField& oldPPts = mesh.oldPoints();
  const faceList& pFaces = mesh.faces();
  const mousse::face& f = pFaces[faceI_];
  return triPointRef
  (
    oldPPts[f[faceBasePtI_]],
    oldPPts[f[facePtAI_]],
    oldPPts[f[facePtBI_]]
  );
}
mousse::label& mousse::tetIndices::cell()
{
  return cellI_;
}
mousse::label& mousse::tetIndices::face()
{
  return faceI_;
}
mousse::label& mousse::tetIndices::faceBasePt()
{
  return faceBasePtI_;
}
mousse::label& mousse::tetIndices::facePtA()
{
  return facePtAI_;
}
mousse::label& mousse::tetIndices::facePtB()
{
  return facePtBI_;
}
mousse::label& mousse::tetIndices::tetPt()
{
  return tetPtI_;
}
// Member Operators 
inline bool mousse::tetIndices::operator==(const mousse::tetIndices& rhs) const
{
  return
  (
    cell() == rhs.cell()
  && face() == rhs.face()
  && faceBasePt() == rhs.faceBasePt()
  && facePtA() == rhs.facePtA()
  && facePtB() == rhs.facePtB()
  && tetPt() == rhs.tetPt()
  );
}
inline bool mousse::tetIndices::operator!=(const mousse::tetIndices& rhs) const
{
  return !(*this == rhs);
}
