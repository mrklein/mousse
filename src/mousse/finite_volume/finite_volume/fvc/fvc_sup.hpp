#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_SUP_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_SUP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"


namespace mousse {
namespace fvc {

// Explicit source
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh>> Su
  (
    const GeometricField<Type, fvPatchField, volMesh>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );

  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh>> Su
  (
    const tmp<GeometricField<Type, fvPatchField, volMesh>>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );

// Implicit source
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh>> Sp
  (
    const volScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );

  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh>> Sp
  (
    const tmp<volScalarField>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );

  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh>> Sp
  (
    const dimensionedScalar&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
// Implicit/Explicit source depending on sign of coefficient
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh>> SuSp
  (
    const volScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );

  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh>> SuSp
  (
    const tmp<volScalarField>&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );

}
}  // namespace mousse

#include "fvc_sup.ipp"

#endif
