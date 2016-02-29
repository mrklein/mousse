#ifndef FINITE_VOLUME_FINITE_VOLUME_FVM_FVM_D2DT2_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVM_FVM_D2DT2_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvm_d2dt2.cpp
#include "vol_fields_fwd.hpp"
#include "fv_matrix.hpp"
namespace mousse
{
namespace fvm
{
  template<class Type>
  tmp<fvMatrix<Type> > d2dt2
  (
    const dimensionedScalar&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > d2dt2
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<fvMatrix<Type> > d2dt2
  (
    const volScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvm_d2dt2.cpp"
#endif
#endif
