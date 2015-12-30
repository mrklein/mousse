// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cross_power_law.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
// Static Data Members
namespace mousse
{
namespace viscosityModels
{
  defineTypeNameAndDebug(CrossPowerLaw, 0);
  addToRunTimeSelectionTable
  (
    viscosityModel,
    CrossPowerLaw,
    dictionary
  );
}
}
// Private Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::viscosityModels::CrossPowerLaw::calcNu() const
{
  return (nu0_ - nuInf_)/(scalar(1) + pow(m_*strainRate(), n_)) + nuInf_;
}
// Constructors 
mousse::viscosityModels::CrossPowerLaw::CrossPowerLaw
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  viscosityModel(name, viscosityProperties, U, phi),
  CrossPowerLawCoeffs_(viscosityProperties.subDict(typeName + "Coeffs")),
  nu0_("nu0", dimViscosity, CrossPowerLawCoeffs_),
  nuInf_("nuInf", dimViscosity, CrossPowerLawCoeffs_),
  m_("m", dimTime, CrossPowerLawCoeffs_),
  n_("n", dimless, CrossPowerLawCoeffs_),
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
bool mousse::viscosityModels::CrossPowerLaw::read
(
  const dictionary& viscosityProperties
)
{
  viscosityModel::read(viscosityProperties);
  CrossPowerLawCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");
  CrossPowerLawCoeffs_.lookup("nu0") >> nu0_;
  CrossPowerLawCoeffs_.lookup("nuInf") >> nuInf_;
  CrossPowerLawCoeffs_.lookup("m") >> m_;
  CrossPowerLawCoeffs_.lookup("n") >> n_;
  return true;
}
