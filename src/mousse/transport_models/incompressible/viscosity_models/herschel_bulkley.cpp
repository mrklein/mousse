// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "herschel_bulkley.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"


// Static Data Members
namespace mousse {
namespace viscosityModels {

DEFINE_TYPE_NAME_AND_DEBUG(HerschelBulkley, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  viscosityModel,
  HerschelBulkley,
  dictionary
);

}
}


// Private Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::viscosityModels::HerschelBulkley::calcNu() const
{
  const dimensionedScalar S("VSMALL", dimless/dimTime, VSMALL);

  dimensionedScalar tone{"tone", dimTime, 1.0};
  dimensionedScalar rtone{"rtone", dimless/dimTime, 1.0};
  tmp<volScalarField> sr{strainRate()};

  return min(nu0_, (tau0_ + k_*rtone*pow(tone*sr(), n_))/(max(sr(), S)));
}


// Constructors 
mousse::viscosityModels::HerschelBulkley::HerschelBulkley
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
  tau0_{"tau0", dimViscosity/dimTime, coeffs_},
  nu0_{"nu0", dimViscosity, coeffs_},
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
bool mousse::viscosityModels::HerschelBulkley::read
(
  const dictionary& viscosityProperties
)
{
  viscosityModel::read(viscosityProperties);
  coeffs_ = viscosityProperties.subDict(typeName + "Coeffs");
  coeffs_.lookup("k") >> k_;
  coeffs_.lookup("n") >> n_;
  coeffs_.lookup("tau0") >> tau0_;
  coeffs_.lookup("nu0") >> nu0_;
  return true;
}

