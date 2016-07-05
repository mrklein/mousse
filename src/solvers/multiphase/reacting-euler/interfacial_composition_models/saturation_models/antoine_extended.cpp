// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "antoine_extended.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace saturationModels {
DEFINE_TYPE_NAME_AND_DEBUG(AntoineExtended, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  saturationModel,
  AntoineExtended,
  dictionary
);
}
}
// Constructors 
mousse::saturationModels::AntoineExtended::AntoineExtended
(
  const dictionary& dict
)
:
  Antoine{dict},
  D_{"D", dimless, dict},
  F_{"F", dimless, dict},
  E_{"E", dimless/pow(dimTemperature, F_), dict}
{}
// Destructor 
mousse::saturationModels::AntoineExtended::~AntoineExtended()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::AntoineExtended::pSat
(
  const volScalarField& T
) const
{
  return dimensionedScalar("one", dimPressure/pow(dimTemperature, D_), 1)
    *exp(A_ + B_/(C_ + T) + E_*pow(T, F_))*pow(T, D_);
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::AntoineExtended::pSatPrime
(
  const volScalarField& T
) const
{
  return pSat(T)*((D_ + E_*F_*pow(T, F_))/T - B_/sqr(C_ + T));
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::AntoineExtended::lnPSat
(
  const volScalarField& T
) const
{
  return
    A_ + B_/(C_ + T)
    + D_*log(T*dimensionedScalar("one", dimless/dimTemperature, 1))
    + E_*pow(T, F_);
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::AntoineExtended::Tsat
(
  const volScalarField& /*p*/
) const
{
  NOT_IMPLEMENTED
  (
    "saturationModels::AntoineExtended::Tsat(const volScalarField& p)"
  );
  return volScalarField::null();
}
