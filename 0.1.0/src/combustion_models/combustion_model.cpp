// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "combustion_model.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(combustionModel, 0);
}
// Constructors 
mousse::combustionModel::combustionModel
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  IOdictionary
  (
    IOobject
    (
      IOobject::groupName("combustionProperties", phaseName),
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    )
  ),
  turbulencePtr_(),
  mesh_(mesh),
  active_(lookupOrDefault<Switch>("active", true)),
  coeffs_(subDict(modelType + "Coeffs")),
  modelType_(modelType),
  phaseName_(phaseName)
{}
// Destructor 
mousse::combustionModel::~combustionModel()
{
  if (turbulencePtr_)
  {
    turbulencePtr_ = 0;
  }
}
// Member Functions 
bool mousse::combustionModel::read()
{
  if (regIOobject::read())
  {
    this->lookup("active") >> active_;
    coeffs_ = subDict(modelType_ + "Coeffs");
    return true;
  }
  else
  {
    return false;
  }
}
mousse::tmp<mousse::volScalarField> mousse::combustionModel::Sh() const
{
  return tmp<mousse::volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        IOobject::groupName("Sh", phaseName_),
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh_,
      dimensionedScalar("zero", dimEnergy/dimVolume/dimTime, 0.0)
    )
  );
}
