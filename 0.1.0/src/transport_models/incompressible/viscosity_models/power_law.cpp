// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "power_law.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
// Static Data Members
namespace mousse
{
namespace viscosityModels
{
  defineTypeNameAndDebug(powerLaw, 0);
  addToRunTimeSelectionTable
  (
    viscosityModel,
    powerLaw,
    dictionary
  );
}
}
// Private Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::viscosityModels::powerLaw::calcNu() const
{
  return max
  (
    nuMin_,
    min
    (
      nuMax_,
      k_*pow
      (
        max
        (
          dimensionedScalar("one", dimTime, 1.0)*strainRate(),
          dimensionedScalar("VSMALL", dimless, VSMALL)
        ),
        n_.value() - scalar(1.0)
      )
    )
  );
}
// Constructors 
mousse::viscosityModels::powerLaw::powerLaw
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  viscosityModel(name, viscosityProperties, U, phi),
  powerLawCoeffs_(viscosityProperties.subDict(typeName + "Coeffs")),
  k_("k", dimViscosity, powerLawCoeffs_),
  n_("n", dimless, powerLawCoeffs_),
  nuMin_("nuMin", dimViscosity, powerLawCoeffs_),
  nuMax_("nuMax", dimViscosity, powerLawCoeffs_),
  nu_
  (
    IOobject
    (
      name,
      U_.time().timeName(),
      U_.db(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    ),
    calcNu()
  )
{}
// Member Functions 
bool mousse::viscosityModels::powerLaw::read
(
  const dictionary& viscosityProperties
)
{
  viscosityModel::read(viscosityProperties);
  powerLawCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");
  powerLawCoeffs_.lookup("k") >> k_;
  powerLawCoeffs_.lookup("n") >> n_;
  powerLawCoeffs_.lookup("nuMin") >> nuMin_;
  powerLawCoeffs_.lookup("nuMax") >> nuMax_;
  return true;
}
