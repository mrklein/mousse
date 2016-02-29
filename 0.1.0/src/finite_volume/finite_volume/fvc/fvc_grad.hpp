#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_grad.cpp
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp
  <
    GeometricField
    <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
  > grad
  (
    const GeometricField<Type, fvsPatchField, surfaceMesh>&
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
  > grad
  (
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
  > grad
  (
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
  > grad
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word& name
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
  > grad
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
  > grad
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_grad.cpp"
#endif
#endif
