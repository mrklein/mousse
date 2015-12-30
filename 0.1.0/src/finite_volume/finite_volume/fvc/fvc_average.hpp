// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_average.cpp
#ifndef fvc_average_hpp_
#define fvc_average_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  //- Area-weighted average a surfaceField creating a volField
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > average
  (
    const GeometricField<Type, fvsPatchField, surfaceMesh>&
  );
  //- Area-weighted average a surfaceField creating a volField
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > average
  (
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
  );
  //- Area-weighted average a surfaceField creating a volField
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > average
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  //- Area-weighted average a surfaceField creating a volField
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > average
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_average.cpp"
#endif
#endif
