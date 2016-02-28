// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_break_up.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace diameterModels {
namespace IATEsources{
DEFINE_TYPE_NAME_AND_DEBUG(turbulentBreakUp, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(IATEsource, turbulentBreakUp, dictionary);
}
}
}
// Constructors 
mousse::diameterModels::IATEsources::turbulentBreakUp::
turbulentBreakUp
(
  const IATE& iate,
  const dictionary& dict
)
:
  IATEsource{iate},
  Cti_{"Cti", dimless, dict},
  WeCr_{"WeCr", dimless, dict}
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::diameterModels::IATEsources::turbulentBreakUp::R() const
{
  tmp<volScalarField> tR
  {
    new volScalarField
    {
      {
        "R",
        iate_.phase().time().timeName(),
        iate_.phase().mesh()
      },
      iate_.phase().mesh(),
      {"R", dimless/dimTime, 0}
    }
  };
  volScalarField R = tR();
  scalar Cti = Cti_.value();
  scalar WeCr = WeCr_.value();
  volScalarField Ut{this->Ut()};
  volScalarField We{this->We()};
  const volScalarField& d = iate_.d()();
  FOR_ALL(R, celli)
  {
    if (We[celli] > WeCr)
    {
      R[celli] = (1.0/3.0)*Cti/d[celli]*Ut[celli]*sqrt(1 - WeCr/We[celli])
        *exp(-WeCr/We[celli]);
    }
  }
  return tR;
}
