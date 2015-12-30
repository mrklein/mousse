// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_flux.cpp
#ifndef fvc_flux_hpp_
#define fvc_flux_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
  (
    const surfaceScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
  (
    const tmp<surfaceScalarField>&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
  (
    const surfaceScalarField&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
  (
    const tmp<surfaceScalarField>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
  (
    const surfaceScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
  (
    const tmp<surfaceScalarField>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
  (
    const surfaceScalarField&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
  (
    const tmp<surfaceScalarField>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_flux.cpp"
#endif
#endif
