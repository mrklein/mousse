// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace XiModels
{
DEFINE_TYPE_NAME_AND_DEBUG(fixed, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiModel, fixed, dictionary);
}
}
// Constructors 
mousse::XiModels::fixed::fixed
(
  const dictionary& XiProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su,
  const volScalarField& rho,
  const volScalarField& b,
  const surfaceScalarField& phi
)
:
  XiModel{XiProperties, thermo, turbulence, Su, rho, b, phi}
{}
// Destructor 
mousse::XiModels::fixed::~fixed()
{}
// Member Functions 
bool mousse::XiModels::fixed::read(const dictionary& XiProperties)
{
  return XiModel::read(XiProperties);
}
