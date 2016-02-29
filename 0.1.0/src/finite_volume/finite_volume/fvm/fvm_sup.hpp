#ifndef FINITE_VOLUME_FINITE_VOLUME_FVM_FVM_SUP_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVM_FVM_SUP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvm_sup.cpp
#include "vol_fields_fwd.hpp"
#include "fv_matrix.hpp"
#include "zero_field.hpp"
namespace mousse
{
namespace fvm
{
 // Explicit source
    template<class Type>
    tmp<fvMatrix<Type> > Su
    (
      const DimensionedField<Type, volMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    tmp<fvMatrix<Type> > Su
    (
      const tmp<DimensionedField<Type, volMesh> >&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    tmp<fvMatrix<Type> > Su
    (
      const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    zeroField Su
    (
      const zero&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
  // Implicit source
    template<class Type>
    tmp<fvMatrix<Type> > Sp
    (
      const DimensionedField<scalar, volMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    tmp<fvMatrix<Type> > Sp
    (
      const tmp<DimensionedField<scalar, volMesh> >&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    tmp<fvMatrix<Type> > Sp
    (
      const tmp<volScalarField>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    tmp<fvMatrix<Type> > Sp
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    zeroField Sp
    (
      const zero&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
  // Implicit/Explicit source depending on sign of coefficient
    template<class Type>
    tmp<fvMatrix<Type> > SuSp
    (
      const DimensionedField<scalar, volMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    tmp<fvMatrix<Type> > SuSp
    (
      const tmp<DimensionedField<scalar, volMesh> >&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    tmp<fvMatrix<Type> > SuSp
    (
      const tmp<volScalarField>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    template<class Type>
    zeroField SuSp
    (
      const zero&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvm_sup.cpp"
#endif
#endif
