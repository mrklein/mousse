#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_DDT_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_DDT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_ddt.cpp
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "dimensioned_types.hpp"
#include "one.hpp"
#include "geometric_zero_field.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
  (
    const dimensioned<Type>,
    const fvMesh&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
  (
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
  (
    const dimensionedScalar&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
  (
    const volScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
  (
    const volScalarField&,
    const volScalarField&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
  (
    const one&,
    const GeometricField<Type, fvPatchField, volMesh>&
  );
  template<class Type>
  tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
  (
    const GeometricField<Type, fvPatchField, volMesh>&,
    const one&
  );
  inline geometricZeroField ddt
  (
    const one&,
    const one&
  )
  {
    return geometricZeroField();
  }
  template<class Type>
  tmp
  <
    GeometricField
    <
      typename mousse::flux<Type>::type,
      fvsPatchField,
      surfaceMesh
    >
  >
  ddtCorr
  (
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
  );
  template<class Type>
  tmp
  <
    GeometricField
    <
      typename mousse::flux<Type>::type,
      fvsPatchField,
      surfaceMesh
    >
  >
  ddtCorr
  (
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField
    <
      typename mousse::flux<Type>::type,
      fvsPatchField,
      surfaceMesh
    >& phi
  );
  template<class Type>
  tmp
  <
    GeometricField
    <
      typename mousse::flux<Type>::type,
      fvsPatchField,
      surfaceMesh
    >
  >
  ddtCorr
  (
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
  );
  template<class Type>
  tmp
  <
    GeometricField
    <
      typename mousse::flux<Type>::type,
      fvsPatchField,
      surfaceMesh
    >
  >
  ddtCorr
  (
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField
    <
      typename mousse::flux<Type>::type,
      fvsPatchField,
      surfaceMesh
    >& phi
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_ddt.cpp"
#endif
#endif
