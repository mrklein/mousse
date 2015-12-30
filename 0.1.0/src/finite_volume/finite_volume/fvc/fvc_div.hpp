// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_div.cpp
#ifndef fvc_div_hpp_
#define fvc_div_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const GeometricField<Type, fvsPatchField, surfaceMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
  > div
  (
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
  > div
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word& name
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
  > div
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp
  <
    GeometricField
    <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
  > div
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const surfaceScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const tmp<surfaceScalarField>&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const surfaceScalarField&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const tmp<surfaceScalarField>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const surfaceScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const tmp<surfaceScalarField>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const surfaceScalarField&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > div
  (
    const tmp<surfaceScalarField>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_div.cpp"
#endif
#endif
