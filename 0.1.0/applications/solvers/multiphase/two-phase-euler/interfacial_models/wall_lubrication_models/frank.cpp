// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "frank.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace wallLubricationModels {
DEFINE_TYPE_NAME_AND_DEBUG(Frank, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  wallLubricationModel,
  Frank,
  dictionary
);
}
}
// Constructors 
mousse::wallLubricationModels::Frank::Frank
(
  const dictionary& dict,
  const phasePair& pair
)
:
  wallLubricationModel{dict, pair},
  Cwd_{"Cwd", dimless, dict},
  Cwc_{"Cwc", dimless, dict},
  p_{readScalar(dict.lookup("p"))}
{}
// Destructor 
mousse::wallLubricationModels::Frank::~Frank()
{}
// Member Functions 
mousse::tmp<mousse::volVectorField> mousse::wallLubricationModels::Frank::Fi() const
{
  volVectorField Ur{pair_.Ur()};
  const volVectorField& n = nWall();
  const volScalarField& y = yWall();
  volScalarField Eo{pair_.Eo()};
  volScalarField yTilde{y/(Cwc_*pair_.dispersed().d())};
  return
    (
      pos(Eo - 1.0)*neg(Eo - 5.0)*exp(-0.933*Eo + 0.179)
      + pos(Eo - 5.0)*neg(Eo - 33.0)*(0.00599*Eo - 0.0187)
      + pos(Eo - 33.0)*0.179
    )
    *max
    (
      {"zero", dimless/dimLength, 0.0},
      (1.0 - yTilde)/(Cwd_*y*pow(yTilde, p_ - 1.0))
    )
    *pair_.continuous().rho()
    *magSqr(Ur - (Ur & n)*n)
    *n;
}
