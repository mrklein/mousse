// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_to_mesh.hpp"
// Member Functions 
inline const mousse::polyMesh& mousse::meshToMesh::srcRegion() const
{
  return srcRegion_;
}
inline const mousse::polyMesh& mousse::meshToMesh::tgtRegion() const
{
  return tgtRegion_;
}
inline const mousse::labelListList&
mousse::meshToMesh::srcToTgtCellAddr() const
{
  return srcToTgtCellAddr_;
}
inline const mousse::labelListList&
mousse::meshToMesh::tgtToSrcCellAddr() const
{
  return tgtToSrcCellAddr_;
}
inline const mousse::scalarListList&
mousse::meshToMesh::srcToTgtCellWght() const
{
  return srcToTgtCellWght_;
}
inline const mousse::scalarListList&
mousse::meshToMesh::tgtToSrcCellWght() const
{
  return tgtToSrcCellWght_;
}
inline mousse::scalar mousse::meshToMesh::V() const
{
  return V_;
}
inline const mousse::PtrList<mousse::AMIPatchToPatchInterpolation>&
mousse::meshToMesh::patchAMIs() const
{
  return patchAMIs_;
}
