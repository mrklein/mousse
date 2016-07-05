// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "antoine.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace saturationModels {
DEFINE_TYPE_NAME_AND_DEBUG(Antoine, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(saturationModel, Antoine, dictionary);
}
}
// Constructors 
mousse::saturationModels::Antoine::Antoine(const dictionary& dict)
:
  saturationModel{},
  A_{"A", dimless, dict},
  B_{"B", dimTemperature, dict},
  C_{"C", dimTemperature, dict}
{}
// Destructor 
mousse::saturationModels::Antoine::~Antoine()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::Antoine::pSat
(
  const volScalarField& T
) const
{
  return dimensionedScalar("one", dimPressure, 1)*exp(A_ + B_/(C_ + T));
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::Antoine::pSatPrime
(
  const volScalarField& T
) const
{
  return - pSat(T)*B_/sqr(C_ + T);
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::Antoine::lnPSat
(
  const volScalarField& T
) const
{
  return A_ + B_/(C_ + T);
}
mousse::tmp<mousse::volScalarField>
mousse::saturationModels::Antoine::Tsat
(
  const volScalarField& p
) const
{
  return
    B_/(log(p*dimensionedScalar("one", dimless/dimPressure, 1)) - A_) - C_;
}
