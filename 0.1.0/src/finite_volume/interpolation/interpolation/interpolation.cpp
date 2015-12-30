// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation.hpp"
#include "vol_fields.hpp"
#include "poly_mesh.hpp"
#include "calculated_point_patch_fields.hpp"
namespace mousse
{
// Constructor
template<class Type>
interpolation<Type>::interpolation
(
  const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
  psi_(psi),
  pMesh_(psi.mesh()),
  pMeshPoints_(pMesh_.points()),
  pMeshFaces_(pMesh_.faces()),
  pMeshFaceCentres_(pMesh_.faceCentres()),
  pMeshFaceAreas_(pMesh_.faceAreas())
{}
}  // namespace mousse
#   include "interpolation_new.cpp"
