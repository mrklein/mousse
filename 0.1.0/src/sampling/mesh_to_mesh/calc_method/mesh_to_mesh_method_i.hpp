// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

const mousse::polyMesh& mousse::meshToMeshMethod::src() const
{
  return src_;
}
const mousse::polyMesh& mousse::meshToMeshMethod::tgt() const
{
  return tgt_;
}
mousse::scalar mousse::meshToMeshMethod::V() const
{
  return V_;
}
