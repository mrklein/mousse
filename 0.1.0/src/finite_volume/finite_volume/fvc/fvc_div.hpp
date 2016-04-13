#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_DIV_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_DIV_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"


namespace mousse {
namespace fvc {

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>&
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
);
template<class Type>
tmp
<
  GeometricField
  <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
> div
(
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word& name
);
template<class Type>
tmp
<
  GeometricField
  <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
> div
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
  const word& name
);
template<class Type>
tmp
<
  GeometricField
  <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
> div
(
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type>
tmp
<
  GeometricField
  <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
> div
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const surfaceScalarField&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word& name
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const tmp<surfaceScalarField>&,
  const GeometricField<Type, fvPatchField, volMesh>&,
  const word& name
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const surfaceScalarField&,
  const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
  const word& name
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const tmp<surfaceScalarField>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
  const word& name
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const surfaceScalarField&,
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const tmp<surfaceScalarField>&,
  const GeometricField<Type, fvPatchField, volMesh>&
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const surfaceScalarField&,
  const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > div
(
  const tmp<surfaceScalarField>&,
  const tmp<GeometricField<Type, fvPatchField, volMesh> >&
);

}
}  // namespace mousse

#include "fvc_div.ipp"

#endif
