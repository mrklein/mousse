// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "plastic.hpp"
#include "surface_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace mixtureViscosityModels
{
DEFINE_TYPE_NAME_AND_DEBUG(plastic, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  mixtureViscosityModel,
  plastic,
  dictionary
);
}
}
// Constructors 
mousse::mixtureViscosityModels::plastic::plastic
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi,
  const word modelName
)
:
  mixtureViscosityModel{name, viscosityProperties, U, phi},
  plasticCoeffs_{viscosityProperties.subDict(modelName + "Coeffs")},
  plasticViscosityCoeff_
  {
    "coeff",
    dimensionSet{1, -1, -1, 0, 0},
    plasticCoeffs_.lookup("coeff")
  },
  plasticViscosityExponent_
  {
    "exponent",
    dimless,
    plasticCoeffs_.lookup("exponent")
  },
  muMax_
  {
    "muMax",
    dimensionSet{1, -1, -1, 0, 0},
    plasticCoeffs_.lookup("muMax")
  },
  alpha_
  {
    U.mesh().lookupObject<volScalarField>
    (
      IOobject::groupName
      (
        viscosityProperties.lookupOrDefault<word>("alpha", "alpha"),
        viscosityProperties.dictName()
      )
    )
  }
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::mixtureViscosityModels::plastic::mu(const volScalarField& muc) const
{
  return min
  (
    muc
    + plasticViscosityCoeff_
    *(pow(scalar(10), plasticViscosityExponent_*alpha_) - scalar(1)),
    muMax_
  );
}
bool mousse::mixtureViscosityModels::plastic::read
(
  const dictionary& viscosityProperties
)
{
  mixtureViscosityModel::read(viscosityProperties);
  plasticCoeffs_ = viscosityProperties.subDict(typeName + "Coeffs");
  plasticCoeffs_.lookup("k") >> plasticViscosityCoeff_;
  plasticCoeffs_.lookup("n") >> plasticViscosityExponent_;
  plasticCoeffs_.lookup("muMax") >> muMax_;
  return true;
}
