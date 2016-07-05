// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "isothermal_diameter.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace diameterModels
{
DEFINE_TYPE_NAME_AND_DEBUG(isothermal, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  diameterModel,
  isothermal,
  dictionary
);
}
}
// Constructors 
mousse::diameterModels::isothermal::isothermal
(
  const dictionary& diameterProperties,
  const phaseModel& phase
)
:
  diameterModel{diameterProperties, phase},
  d0_{"d0", dimLength, diameterProperties_},
  p0_{"p0", dimPressure, diameterProperties_}
{}
// Destructor 
mousse::diameterModels::isothermal::~isothermal()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::diameterModels::isothermal::d() const
{
  const volScalarField& p = phase_.U().db().lookupObject<volScalarField>
  (
    "p"
  );
  return d0_*pow(p0_/p, 1.0/3.0);
}
bool mousse::diameterModels::isothermal::read(const dictionary& phaseProperties)
{
  diameterModel::read(phaseProperties);
  diameterProperties_.lookup("d0") >> d0_;
  diameterProperties_.lookup("p0") >> p0_;
  return true;
}
