// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_surface_integrate.cpp
#ifndef fvc_surface_integrate_hpp_
#define fvc_surface_integrate_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  void surfaceIntegrate
  (
    Field<Type>&,
    const GeometricField<Type, fvsPatchField, surfaceMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> >
  surfaceIntegrate
  (
    const GeometricField<Type, fvsPatchField, surfaceMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> >
  surfaceIntegrate
  (
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > surfaceSum
  (
    const GeometricField<Type, fvsPatchField, surfaceMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > surfaceSum
  (
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_surface_integrate.cpp"
#endif
#endif
