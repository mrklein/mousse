// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
namespace mousse
{
namespace limitFuncs
{
template<class Type>
inline tmp<volScalarField> magSqr<Type>::operator()
(
  const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
  return mousse::magSqr(phi);
}
template<>
inline tmp<volScalarField> magSqr<scalar>::operator()
(
  const volScalarField& phi
) const
{
  return phi;
}
template<>
inline tmp<volScalarField> magSqr<symmTensor>::operator()
(
  const volSymmTensorField& phi
) const
{
  return mousse::tr(phi);
}
template<>
inline tmp<volScalarField> magSqr<tensor>::operator()
(
  const volTensorField& phi
) const
{
  return mousse::tr(phi);
}
template<class Type>
inline tmp<volScalarField> rhoMagSqr<Type>::operator()
(
  const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
  const volScalarField& rho =
    phi.db().objectRegistry::template lookupObject<volScalarField>("rho");
  return mousse::magSqr(phi/rho);
}
template<>
inline tmp<volScalarField> rhoMagSqr<scalar>::operator()
(
  const volScalarField& phi
) const
{
  const volScalarField& rho =
    phi.db().objectRegistry::lookupObject<volScalarField>("rho");
  return phi/rho;
}
}  // namespace limitFuncs
}  // namespace mousse
