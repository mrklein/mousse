// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wallis.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace compressibilityModels
  {
    DEFINE_TYPE_NAME_AND_DEBUG(Wallis, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE
    (
      barotropicCompressibilityModel,
      Wallis,
      dictionary
    );
  }
}
// Constructors 
mousse::compressibilityModels::Wallis::Wallis
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
  ),
  rhovSat_
  (
    "rhovSat",
    dimDensity,
    compressibilityProperties_.lookup("rhovSat")
  ),
  rholSat_
  (
    "rholSat",
    dimDensity,
    compressibilityProperties_.lookup("rholSat")
  )
{
  correct();
}
// Member Functions 
void mousse::compressibilityModels::Wallis::correct()
{
  psi_ =
    (gamma_*rhovSat_ + (scalar(1) - gamma_)*rholSat_)
   *(gamma_*psiv_/rhovSat_ + (scalar(1) - gamma_)*psil_/rholSat_);
}
bool mousse::compressibilityModels::Wallis::read
(
  const dictionary& compressibilityProperties
)
{
  barotropicCompressibilityModel::read(compressibilityProperties);
  compressibilityProperties_.lookup("psiv") >> psiv_;
  compressibilityProperties_.lookup("psil") >> psil_;
  compressibilityProperties_.lookup("rhovSat") >> rhovSat_;
  compressibilityProperties_.lookup("rholSat") >> rholSat_;
  return true;
}
