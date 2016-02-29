#ifndef FINITE_VOLUME_FINITE_VOLUME_FVM_FVM_DDT_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVM_FVM_DDT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvm_ddt.cpp
#include "vol_fields_fwd.hpp"
#include "fv_matrix.hpp"
#include "one.hpp"
namespace mousse
{
namespace fvm
{
  template<class Type>
  tmp<fvMatrix<Type> > ddt
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > ddt
  (
    const one&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > ddt
  (
    const dimensionedScalar&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > ddt
  (
    const volScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > ddt
  (
    const volScalarField&,
    const volScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > ddt
  (
    const one&,
    const one&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > ddt
  (
    const one&,
    const volScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > ddt
  (
    const volScalarField&,
    const one&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvm_ddt.cpp"
#endif
#endif
