// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_diameter.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace diameterModels {
DEFINE_TYPE_NAME_AND_DEBUG(constant, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  diameterModel,
  constant,
  dictionary
);
}
}
// Constructors 
mousse::diameterModels::constant::constant
(
  const dictionary& diameterProperties,
  const phaseModel& phase
)
:
  diameterModel{diameterProperties, phase},
  d_{"d", dimLength, diameterProperties_}
{}
// Destructor 
mousse::diameterModels::constant::~constant()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::diameterModels::constant::d() const
{
  return tmp<mousse::volScalarField>
  {
    new volScalarField
    {
      {
        "d",
        phase_.time().timeName(),
        phase_.mesh()
      },
      phase_.mesh(),
      d_
    }
  };
}
bool mousse::diameterModels::constant::read(const dictionary& phaseProperties)
{
  diameterModel::read(phaseProperties);
  diameterProperties_.lookup("d") >> d_;
  return true;
}
