// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_mesh_phi.hpp"
#include "fv_mesh.hpp"
#include "ddt_scheme.hpp"
mousse::tmp<mousse::surfaceScalarField> mousse::fvc::meshPhi
(
  const volVectorField& vf
)
{
  return fv::ddtScheme<vector>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("ddt(" + vf.name() + ')')
  )().meshPhi(vf);
}
mousse::tmp<mousse::surfaceScalarField> mousse::fvc::meshPhi
(
  const dimensionedScalar& rho,
  const volVectorField& vf
)
{
  return fv::ddtScheme<vector>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
  )().meshPhi(vf);
}
mousse::tmp<mousse::surfaceScalarField> mousse::fvc::meshPhi
(
  const volScalarField& rho,
  const volVectorField& vf
)
{
  return fv::ddtScheme<vector>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
  )().meshPhi(vf);
}
void mousse::fvc::makeRelative
(
  surfaceScalarField& phi,
  const volVectorField& U
)
{
  if (phi.mesh().moving())
  {
    phi -= fvc::meshPhi(U);
  }
}
void mousse::fvc::makeRelative
(
  surfaceScalarField& phi,
  const dimensionedScalar& rho,
  const volVectorField& U
)
{
  if (phi.mesh().moving())
  {
    phi -= rho*fvc::meshPhi(rho, U);
  }
}
void mousse::fvc::makeRelative
(
  surfaceScalarField& phi,
  const volScalarField& rho,
  const volVectorField& U
)
{
  if (phi.mesh().moving())
  {
    phi -= fvc::interpolate(rho)*fvc::meshPhi(rho, U);
  }
}
void mousse::fvc::makeAbsolute
(
  surfaceScalarField& phi,
  const volVectorField& U
)
{
  if (phi.mesh().moving())
  {
    phi += fvc::meshPhi(U);
  }
}
void mousse::fvc::makeAbsolute
(
  surfaceScalarField& phi,
  const dimensionedScalar& rho,
  const volVectorField& U
)
{
  if (phi.mesh().moving())
  {
    phi += rho*fvc::meshPhi(rho, U);
  }
}
void mousse::fvc::makeAbsolute
(
  surfaceScalarField& phi,
  const volScalarField& rho,
  const volVectorField& U
)
{
  if (phi.mesh().moving())
  {
    phi += fvc::interpolate(rho)*fvc::meshPhi(rho, U);
  }
}
mousse::tmp<mousse::surfaceScalarField> mousse::fvc::relative
(
  const tmp<surfaceScalarField>& tphi,
  const volVectorField& U
)
{
  if (tphi().mesh().moving())
  {
    return tphi - fvc::meshPhi(U);
  }
  else
  {
    return tmp<surfaceScalarField>(tphi, true);
  }
}
mousse::tmp<mousse::surfaceScalarField> mousse::fvc::relative
(
  const tmp<surfaceScalarField>& tphi,
  const volScalarField& rho,
  const volVectorField& U
)
{
  if (tphi().mesh().moving())
  {
    return tphi - fvc::interpolate(rho)*fvc::meshPhi(rho, U);
  }
  else
  {
    return tmp<surfaceScalarField>(tphi, true);
  }
}
mousse::tmp<mousse::surfaceScalarField> mousse::fvc::absolute
(
  const tmp<surfaceScalarField>& tphi,
  const volVectorField& U
)
{
  if (tphi().mesh().moving())
  {
    return tphi + fvc::meshPhi(U);
  }
  else
  {
    return tmp<surfaceScalarField>(tphi, true);
  }
}
mousse::tmp<mousse::surfaceScalarField> mousse::fvc::absolute
(
  const tmp<surfaceScalarField>& tphi,
  const volScalarField& rho,
  const volVectorField& U
)
{
  if (tphi().mesh().moving())
  {
    return tphi + fvc::interpolate(rho)*fvc::meshPhi(rho, U);
  }
  else
  {
    return tmp<surfaceScalarField>(tphi, true);
  }
}
