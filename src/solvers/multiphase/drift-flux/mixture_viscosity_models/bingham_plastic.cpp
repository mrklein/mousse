// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bingham_plastic.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvc_grad.hpp"


// Static Data Members
namespace mousse {
namespace mixtureViscosityModels {

DEFINE_TYPE_NAME_AND_DEBUG(BinghamPlastic, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  mixtureViscosityModel,
  BinghamPlastic,
  dictionary
);

}
}


// Constructors 
mousse::mixtureViscosityModels::BinghamPlastic::BinghamPlastic
(
  const word& name,
  const dictionary& viscosityProperties,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  plastic{name, viscosityProperties, U, phi, typeName},
  yieldStressCoeff_
  {
    "BinghamCoeff",
    {1, -1, -2, 0, 0},
    plasticCoeffs_
  },
  yieldStressExponent_
  {
    "BinghamExponent",
    dimless,
    plasticCoeffs_
  },
  yieldStressOffset_
  {
    "BinghamOffset",
    dimless,
    plasticCoeffs_
  },
  U_{U}
{}


// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::mixtureViscosityModels::BinghamPlastic::mu
(
  const volScalarField& muc
) const
{
  volScalarField tauy
  {
    yieldStressCoeff_
    *(pow(scalar(10), yieldStressExponent_*(max(alpha_, scalar(0))
                                            + yieldStressOffset_))
      - pow(scalar(10), yieldStressExponent_*yieldStressOffset_))
  };
  volScalarField mup{plastic::mu(muc)};
  dimensionedScalar tauySmall{"tauySmall", tauy.dimensions(), SMALL};
  return min
  (
    tauy/(sqrt(2.0)*mag(symm(fvc::grad(U_))) + 1.0e-4*(tauy + tauySmall)/mup)
    + mup,
    muMax_
  );
}
bool mousse::mixtureViscosityModels::BinghamPlastic::read
(
  const dictionary& viscosityProperties
)
{
  plastic::read(viscosityProperties);
  plasticCoeffs_.lookup("yieldStressCoeff") >> yieldStressCoeff_;
  plasticCoeffs_.lookup("yieldStressExponent") >> yieldStressExponent_;
  plasticCoeffs_.lookup("yieldStressOffset") >> yieldStressOffset_;
  return true;
}
