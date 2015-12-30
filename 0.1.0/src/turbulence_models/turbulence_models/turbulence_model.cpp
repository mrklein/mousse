// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulence_model.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "wall_fv_patch.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(turbulenceModel, 0);
}
const mousse::word mousse::turbulenceModel::propertiesName("turbulenceProperties");
// Constructors 
mousse::turbulenceModel::turbulenceModel
(
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const word& propertiesName
)
:
  IOdictionary
  (
    IOobject
    (
      IOobject::groupName(propertiesName, U.group()),
      U.time().constant(),
      U.db(),
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    )
  ),
  runTime_(U.time()),
  mesh_(U.mesh()),
  U_(U),
  alphaRhoPhi_(alphaRhoPhi),
  phi_(phi),
  y_(mesh_)
{}
// Member Functions 
mousse::tmp<mousse::surfaceScalarField> mousse::turbulenceModel::phi() const
{
  return phi_;
}
bool mousse::turbulenceModel::read()
{
  return regIOobject::read();
}
void mousse::turbulenceModel::correct()
{
  if (mesh_.changing())
  {
    y_.correct();
  }
}
