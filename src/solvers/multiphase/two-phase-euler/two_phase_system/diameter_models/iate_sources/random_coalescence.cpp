// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "random_coalescence.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace diameterModels {
namespace IATEsources {
DEFINE_TYPE_NAME_AND_DEBUG(randomCoalescence, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(IATEsource, randomCoalescence, dictionary);
}
}
}
// Constructors 
mousse::diameterModels::IATEsources::randomCoalescence::
randomCoalescence
(
  const IATE& iate,
  const dictionary& dict
)
:
  IATEsource{iate},
  Crc_{"Crc", dimless, dict},
  C_{"C", dimless, dict},
  alphaMax_{"alphaMax", dimless, dict}
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::diameterModels::IATEsources::randomCoalescence::R() const
{
  tmp<volScalarField> tR
  {
    new volScalarField
    {
      {
        "R",
        iate_.phase().U().time().timeName(),
        iate_.phase().mesh()
      },
      iate_.phase().U().mesh(),
      {"R", dimless/dimTime, 0}
    }
  };
  volScalarField R = tR();
  scalar Crc = Crc_.value();
  scalar C = C_.value();
  scalar alphaMax = alphaMax_.value();
  volScalarField Ut{this->Ut()};
  const volScalarField& alpha = phase();
  const volScalarField& kappai = iate_.kappai();
  scalar cbrtAlphaMax = cbrt(alphaMax);
  FOR_ALL(R, celli)
  {
    if (alpha[celli] < alphaMax - SMALL)
    {
      scalar cbrtAlphaMaxMAlpha = cbrtAlphaMax - cbrt(alpha[celli]);
      R[celli] = (-12)*phi()*kappai[celli]*alpha[celli]
        *Crc*Ut[celli]
        *(1 - exp(-C*cbrt(alpha[celli]*alphaMax)/cbrtAlphaMaxMAlpha))
        /(cbrtAlphaMax*cbrtAlphaMaxMAlpha);
    }
  }
  return tR;
}
