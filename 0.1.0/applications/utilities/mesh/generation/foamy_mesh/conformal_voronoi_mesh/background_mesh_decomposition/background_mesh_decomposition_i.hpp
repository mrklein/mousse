// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
const mousse::fvMesh& mousse::backgroundMeshDecomposition::mesh() const
{
  return mesh_;
}
const mousse::indexedOctree<mousse::treeDataBPatch>&
mousse::backgroundMeshDecomposition::tree() const
{
  return bFTreePtr_();
}
const mousse::treeBoundBox&
mousse::backgroundMeshDecomposition::procBounds() const
{
  return allBackgroundMeshBounds_[Pstream::myProcNo()];
}
const mousse::labelList& mousse::backgroundMeshDecomposition::cellLevel() const
{
  return meshCutter_.cellLevel();
}
const mousse::labelList& mousse::backgroundMeshDecomposition::pointLevel() const
{
  return meshCutter_.pointLevel();
}
const mousse::decompositionMethod&
mousse::backgroundMeshDecomposition::decomposer() const
{
  return decomposerPtr_();
}
