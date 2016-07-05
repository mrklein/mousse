// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interface.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace dragModels {
DEFINE_TYPE_NAME_AND_DEBUG(interface, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  dragModel,
  interface,
  dictionary
);
}
}
// Constructors 
mousse::dragModels::interface::interface
(
  const dictionary& interfaceDict,
  const phaseModel& phase1,
  const phaseModel& phase2
)
:
  dragModel{interfaceDict, phase1, phase2}
{}
// Destructor 
mousse::dragModels::interface::~interface()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::dragModels::interface::K
(
  const volScalarField& Ur
) const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "K",
        Ur.mesh().time().timeName(),
        Ur.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      Ur.mesh(),
      {"K", dimDensity/dimTime, 0}
    }
  };
}
