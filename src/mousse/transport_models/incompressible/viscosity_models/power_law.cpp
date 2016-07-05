// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "power_law.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"


// Static Data Members
namespace mousse {
namespace viscosityModels {

DEFINE_TYPE_NAME_AND_DEBUG(powerLaw, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
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
  const dimensionedScalar ONE("one", dimTime, 1.0);
  const dimensionedScalar SMALL("VSMALL", dimless, VSMALL);

  return
    max
    (
      nuMin_,
      min
      (
        nuMax_,
        k_*pow(max(ONE*strainRate(), SMALL), n_.value() - scalar(1.0))
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
  viscosityModel{name, viscosityProperties, U, phi},
  coeffs_{viscosityProperties.subDict(typeName + "Coeffs")},
  k_{"k", dimViscosity, coeffs_},
  n_{"n", dimless, coeffs_},
  nuMin_{"nuMin", dimViscosity, coeffs_},
  nuMax_{"nuMax", dimViscosity, coeffs_},
  nu_
  {
    {
      name,
      U_.time().timeName(),
      U_.db(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    calcNu()
  }
{}


// Member Functions 
bool mousse::viscosityModels::powerLaw::read
(
  const dictionary& viscosityProperties
)
{
  viscosityModel::read(viscosityProperties);
  coeffs_ = viscosityProperties.subDict(typeName + "Coeffs");
  coeffs_.lookup("k") >> k_;
  coeffs_.lookup("n") >> n_;
  coeffs_.lookup("nuMin") >> nuMin_;
  coeffs_.lookup("nuMax") >> nuMax_;
  return true;
}

