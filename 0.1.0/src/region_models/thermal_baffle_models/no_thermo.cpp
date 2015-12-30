// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
namespace mousse
{
namespace regionModels
{
namespace thermalBaffleModels
{
// Static Data Members
defineTypeNameAndDebug(noThermo, 0);
addToRunTimeSelectionTable(thermalBaffleModel, noThermo, mesh);
addToRunTimeSelectionTable(thermalBaffleModel, noThermo, dictionary);
// Protected Member Functions 
bool noThermo::read()
{
  return regionModel1D::read();
}
// Constructors 
noThermo::noThermo(const word& modelType, const fvMesh& mesh)
:
  thermalBaffleModel(mesh)
{}
noThermo::noThermo
(
  const word& modelType,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  thermalBaffleModel(modelType, mesh, dict)
{}
// Destructor 
noThermo::~noThermo()
{}
// Member Functions 
void noThermo::preEvolveRegion()
{}
void noThermo::evolveRegion()
{}
const tmp<volScalarField> noThermo::Cp() const
{
  FatalErrorIn("const tmp<volScalarField>& noThermo::Cp() const")
    << "Cp field not available for " << type()
    << abort(FatalError);
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        "noThermo::Cp",
        time().timeName(),
        primaryMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      primaryMesh(),
      dimensionedScalar("zero", dimEnergy/dimVolume/dimTime, 0.0)
    )
  );
}
const volScalarField& noThermo::kappaRad() const
{
  FatalErrorIn("const volScalarField& noThermo::kappaRad() const")
    << "kappa field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noThermo::rho() const
{
  FatalErrorIn("const volScalarField& noThermo::rho() const")
    << "rho field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noThermo::kappa() const
{
 FatalErrorIn("const volScalarField& noThermo::kappa() const")
    << "K field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}
const volScalarField& noThermo::T() const
{
  FatalErrorIn("const volScalarField& noThermo::T() const")
    << "T field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}
const solidThermo& noThermo::thermo() const
{
  FatalErrorIn("const volScalarField& noThermo::T() const")
    << "T field not available for " << type()
    << abort(FatalError);
  return NullObjectRef<solidThermo>();
}
}  // namespace thermalBaffleModels
}  // namespace regionModels
}  // namespace mousse
