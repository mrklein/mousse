// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arden_buck.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace saturationModels {
DEFINE_TYPE_NAME_AND_DEBUG(ArdenBuck, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(saturationModel, ArdenBuck, dictionary);
}
}
static const mousse::dimensionedScalar zeroC{"", mousse::dimTemperature, 273.15};
static const mousse::dimensionedScalar A{"", mousse::dimPressure, 611.21};
static const mousse::dimensionedScalar B{"", mousse::dimless, 18.678};
static const mousse::dimensionedScalar C{"", mousse::dimTemperature, 234.5};
static const mousse::dimensionedScalar D{"", mousse::dimTemperature, 257.14};
// Private Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::ArdenBuck::xByTC
(
  const volScalarField& TC
) const
{
  return (B - TC/C)/(D + TC);
}
// Constructors 
mousse::saturationModels::ArdenBuck::ArdenBuck(const dictionary& /*dict*/)
:
  saturationModel()
{}
// Destructor 
mousse::saturationModels::ArdenBuck::~ArdenBuck()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::ArdenBuck::pSat
(
  const volScalarField& T
) const
{
  volScalarField TC{T - zeroC};
  return A*exp(TC*xByTC(TC));
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::ArdenBuck::pSatPrime
(
  const volScalarField& T
) const
{
  volScalarField TC{T - zeroC};
  volScalarField x{xByTC(TC)};
  return A*exp(TC*x)*(D*x - TC/C)/(D + TC);
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::ArdenBuck::lnPSat
(
  const volScalarField& T
) const
{
  volScalarField TC{T - zeroC};
  return log(A.value()) + TC*xByTC(TC);
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::ArdenBuck::Tsat
(
  const volScalarField& /*p*/
) const
{
  NOT_IMPLEMENTED
  (
    "saturationModels::ArdenBuck::Tsat(const volScalarField& p)"
  );
  return volScalarField::null();
}
