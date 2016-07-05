// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wake_entrainment_coalescence.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace diameterModels {
namespace IATEsources {
DEFINE_TYPE_NAME_AND_DEBUG(wakeEntrainmentCoalescence, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  IATEsource,
  wakeEntrainmentCoalescence,
  dictionary
);
}
}
}
// Constructors 
mousse::diameterModels::IATEsources::wakeEntrainmentCoalescence::
wakeEntrainmentCoalescence
(
  const IATE& iate,
  const dictionary& dict
)
:
  IATEsource{iate},
  Cwe_{"Cwe", dimless, dict}
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::diameterModels::IATEsources::wakeEntrainmentCoalescence::R() const
{
  return (-12)*phi()*Cwe_*cbrt(CD())*iate_.a()*Ur();
}
