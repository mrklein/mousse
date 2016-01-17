// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
namespace fvc
{
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Su
(
  const GeometricField<Type, fvPatchField, volMesh>& su,
  const GeometricField<Type, fvPatchField, volMesh>& /*vf*/
)
{
  return su;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Su
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu,
  const GeometricField<Type, fvPatchField, volMesh>& /*vf*/
)
{
  return tsu;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Sp
(
  const volScalarField& sp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return sp*vf;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Sp
(
  const tmp<volScalarField>& tsp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return tsp*vf;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Sp
(
  const dimensionedScalar& sp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return sp*vf;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
SuSp
(
  const volScalarField& sp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return sp*vf;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
SuSp
(
  const tmp<volScalarField>& tsp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return tsp*vf;
}
}  // namespace fvc
}  // namespace mousse
