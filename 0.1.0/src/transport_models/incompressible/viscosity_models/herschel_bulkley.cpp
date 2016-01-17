// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "herschel_bulkley.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
// Static Data Members
namespace mousse
{
namespace viscosityModels
{
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
  dimensionedScalar tone("tone", dimTime, 1.0);
  dimensionedScalar rtone("rtone", dimless/dimTime, 1.0);
  tmp<volScalarField> sr(strainRate());
  return
  (
    min
    (
      nu0_,
      (tau0_ + k_*rtone*pow(tone*sr(), n_))
     /(max(sr(), dimensionedScalar ("VSMALL", dimless/dimTime, VSMALL)))
    )
  );
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
  viscosityModel(name, viscosityProperties, U, phi),
  HerschelBulkleyCoeffs_(viscosityProperties.subDict(typeName + "Coeffs")),
  k_("k", dimViscosity, HerschelBulkleyCoeffs_),
  n_("n", dimless, HerschelBulkleyCoeffs_),
  tau0_("tau0", dimViscosity/dimTime, HerschelBulkleyCoeffs_),
  nu0_("nu0", dimViscosity, HerschelBulkleyCoeffs_),
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
bool mousse::viscosityModels::HerschelBulkley::read
(
  const dictionary& viscosityProperties
)
{
  viscosityModel::read(viscosityProperties);
  HerschelBulkleyCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");
  HerschelBulkleyCoeffs_.lookup("k") >> k_;
  HerschelBulkleyCoeffs_.lookup("n") >> n_;
  HerschelBulkleyCoeffs_.lookup("tau0") >> tau0_;
  HerschelBulkleyCoeffs_.lookup("nu0") >> nu0_;
  return true;
}
