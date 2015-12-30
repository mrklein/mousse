// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvm_div.cpp
#ifndef fvm_div_hpp_
#define fvm_div_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "surface_interpolation_scheme.hpp"
#include "fv_matrix.hpp"
namespace mousse
{
namespace fvm
{
  template<class Type>
  tmp<fvMatrix<Type> > div
  (
    const surfaceScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp<fvMatrix<Type> > div
  (
    const tmp<surfaceScalarField>&,
    const GeometricField<Type, fvPatchField, volMesh>&,
    const word& name
  );
  template<class Type>
  tmp<fvMatrix<Type> > div
  (
    const surfaceScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > div
  (
    const tmp<surfaceScalarField>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvm_div.cpp"
#endif
#endif
