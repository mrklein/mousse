// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"


namespace mousse {
namespace regionModels {
namespace thermalBaffleModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(noThermo, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(thermalBaffleModel, noThermo, mesh);
ADD_TO_RUN_TIME_SELECTION_TABLE(thermalBaffleModel, noThermo, dictionary);


// Protected Member Functions 
bool noThermo::read()
{
  return regionModel1D::read();
}


// Constructors 
noThermo::noThermo(const word& /*modelType*/, const fvMesh& mesh)
:
  thermalBaffleModel{mesh}
{}


noThermo::noThermo
(
  const word& modelType,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  thermalBaffleModel{modelType, mesh, dict}
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
  FATAL_ERROR_IN("const tmp<volScalarField>& noThermo::Cp() const")
    << "Cp field not available for " << type()
    << abort(FatalError);
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "noThermo::Cp",
            time().timeName(),
            primaryMesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        },
        primaryMesh(),
        {"zero", dimEnergy/dimVolume/dimTime, 0.0}
      }
    };
}


const volScalarField& noThermo::kappaRad() const
{
  FATAL_ERROR_IN("const volScalarField& noThermo::kappaRad() const")
    << "kappa field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noThermo::rho() const
{
  FATAL_ERROR_IN("const volScalarField& noThermo::rho() const")
    << "rho field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noThermo::kappa() const
{
 FATAL_ERROR_IN("const volScalarField& noThermo::kappa() const")
    << "K field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}


const volScalarField& noThermo::T() const
{
  FATAL_ERROR_IN("const volScalarField& noThermo::T() const")
    << "T field not available for " << type()
    << abort(FatalError);
  return volScalarField::null();
}


const solidThermo& noThermo::thermo() const
{
  FATAL_ERROR_IN("const volScalarField& noThermo::T() const")
    << "T field not available for " << type()
    << abort(FatalError);
  return NullObjectRef<solidThermo>();
}

}  // namespace thermalBaffleModels
}  // namespace regionModels
}  // namespace mousse
