// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type>
mousse::labelHashSet mousse::polyBoundaryMesh::findPatchIDs() const
{
  const polyBoundaryMesh& bm = *this;
  labelHashSet patchIDs(bm.size());
  FOR_ALL(bm, patchI) {
    if (isA<Type>(bm[patchI])) {
      patchIDs.insert(patchI);
    }
  }
  return patchIDs;
}

