// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_sn_grad.cpp
#ifndef fvc_sn_grad_hpp_
#define fvc_sn_grad_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > snGrad
  (
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > snGrad
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
    const word& name
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > snGrad
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > snGrad
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh> >&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_sn_grad.cpp"
#endif
#endif
