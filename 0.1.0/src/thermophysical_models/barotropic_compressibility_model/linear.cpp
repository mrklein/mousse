// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace compressibilityModels
  {
    DEFINE_TYPE_NAME_AND_DEBUG(linear, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE
    (
      barotropicCompressibilityModel,
      linear,
      dictionary
    );
  }
}
// Constructors 
mousse::compressibilityModels::linear::linear
(
  const dictionary& compressibilityProperties,
  const volScalarField& gamma,
  const word& psiName
)
:
  barotropicCompressibilityModel(compressibilityProperties, gamma, psiName),
  psiv_
  (
    "psiv",
    dimCompressibility,
    compressibilityProperties_.lookup("psiv")
  ),
  psil_
  (
    "psil",
    dimCompressibility,
    compressibilityProperties_.lookup("psil")
  )
{
  correct();
  psi_.oldTime();
}
// Member Functions 
void mousse::compressibilityModels::linear::correct()
{
  psi_ = gamma_*psiv_ + (scalar(1) - gamma_)*psil_;
}
bool mousse::compressibilityModels::linear::read
(
  const dictionary& compressibilityProperties
)
{
  barotropicCompressibilityModel::read(compressibilityProperties);
  compressibilityProperties_.lookup("psiv") >> psiv_;
  compressibilityProperties_.lookup("psil") >> psil_;
  return true;
}
