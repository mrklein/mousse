// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bird_carreau.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
// Static Data Members
namespace mousse
{
namespace viscosityModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(BirdCarreau, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    viscosityModel,
    BirdCarreau,
    dictionary
  );
}
}
// Private Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::viscosityModels::BirdCarreau::calcNu() const
{
  return
    nuInf_
   + (nu0_ - nuInf_)
   *pow(scalar(1) + pow(k_*strainRate(), a_), (n_ - 1.0)/a_);
}
// Constructors 
mousse::viscosityModels::BirdCarreau::BirdCarreau
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  viscosityModel(name, viscosityProperties, U, phi),
  BirdCarreauCoeffs_(viscosityProperties.subDict(typeName + "Coeffs")),
  nu0_("nu0", dimViscosity, BirdCarreauCoeffs_),
  nuInf_("nuInf", dimViscosity, BirdCarreauCoeffs_),
  k_("k", dimTime, BirdCarreauCoeffs_),
  n_("n", dimless, BirdCarreauCoeffs_),
  a_
  (
    BirdCarreauCoeffs_.lookupOrDefault
    (
      "a",
      dimensionedScalar("a", dimless, 2)
    )
  ),
  nu_
  (
    IOobject
    (
      "nu",
      U_.time().timeName(),
      U_.db(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    ),
    calcNu()
  )
{}
// Member Functions 
bool mousse::viscosityModels::BirdCarreau::read
(
  const dictionary& viscosityProperties
)
{
  viscosityModel::read(viscosityProperties);
  BirdCarreauCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");
  BirdCarreauCoeffs_.lookup("nu0") >> nu0_;
  BirdCarreauCoeffs_.lookup("nuInf") >> nuInf_;
  BirdCarreauCoeffs_.lookup("k") >> k_;
  BirdCarreauCoeffs_.lookup("n") >> n_;
  a_ = BirdCarreauCoeffs_.lookupOrDefault
  (
    "a",
    dimensionedScalar("a", dimless, 2)
  );
  return true;
}
