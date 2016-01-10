// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "viscosity_model.hpp"
#include "vol_fields.hpp"
#include "fvc_grad.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(viscosityModel, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(viscosityModel, dictionary);
}
// Constructors 
mousse::viscosityModel::viscosityModel
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  name_(name),
  viscosityProperties_(viscosityProperties),
  U_(U),
  phi_(phi)
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::viscosityModel::strainRate() const
{
  return sqrt(2.0)*mag(symm(fvc::grad(U_)));
}
bool mousse::viscosityModel::read(const dictionary& viscosityProperties)
{
  viscosityProperties_ = viscosityProperties;
  return true;
}
