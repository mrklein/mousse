// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_laplacian.cpp
#ifndef fvc_laplacian_hpp_
#define fvc_laplacian_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "dimensioned_types.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const dimensioned<GType>&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const dimensioned<GType>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const dimensioned<GType>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const dimensioned<GType>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<GType, fvPatchField, volMesh>&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<GType, fvPatchField, volMesh>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<GType, fvPatchField, volMesh>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<GType, fvPatchField, volMesh>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<GType, fvsPatchField, surfaceMesh>&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<GType, fvsPatchField, surfaceMesh>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<GType, fvsPatchField, surfaceMesh>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const GeometricField<GType, fvsPatchField, surfaceMesh>&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
  template<class Type, class GType>
  tmp<GeometricField<Type, fvPatchField, volMesh> > laplacian
  (
    const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_laplacian.cpp"
#endif
#endif
