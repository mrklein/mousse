// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tomiyama_wall_lubrication.hpp"
#include "phase_pair.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace wallLubricationModels {
DEFINE_TYPE_NAME_AND_DEBUG(TomiyamaWallLubrication, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  wallLubricationModel,
  TomiyamaWallLubrication,
  dictionary
);
}
}
// Constructors 
mousse::wallLubricationModels::TomiyamaWallLubrication::TomiyamaWallLubrication
(
  const dictionary& dict,
  const phasePair& pair
)
:
  wallLubricationModel{dict, pair},
  D_{"Cwd", dimLength, dict}
{}
// Destructor 
mousse::wallLubricationModels::TomiyamaWallLubrication::~TomiyamaWallLubrication()
{}
// Member Functions 
mousse::tmp<mousse::volVectorField>
mousse::wallLubricationModels::TomiyamaWallLubrication::Fi() const
{
  volVectorField Ur{pair_.Ur()};
  const volVectorField& n = nWall();
  const volScalarField& y = yWall();
  volScalarField Eo{pair_.Eo()};
  return
    (
      pos(Eo - 1.0)*neg(Eo - 5.0)*exp(-0.933*Eo + 0.179)
      + pos(Eo - 5.0)*neg(Eo - 33.0)*(0.00599*Eo - 0.0187)
      + pos(Eo - 33.0)*0.179
    )
    *0.5
    *pair_.dispersed().d()
    *(1/sqr(y) - 1/sqr(D_ - y))
    *pair_.continuous().rho()
    *magSqr(Ur - (Ur & n)*n)
    *n;
}
