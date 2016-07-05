// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_turbulent_dispersion_coefficient.hpp"
#include "phase_pair.hpp"
#include "phase_compressible_turbulence_model.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace turbulentDispersionModels {
DEFINE_TYPE_NAME_AND_DEBUG(constantTurbulentDispersionCoefficient, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  turbulentDispersionModel,
  constantTurbulentDispersionCoefficient,
  dictionary
);
}
}
// Constructors 
mousse::turbulentDispersionModels::constantTurbulentDispersionCoefficient::
constantTurbulentDispersionCoefficient
(
  const dictionary& dict,
  const phasePair& pair
)
:
  turbulentDispersionModel{dict, pair},
  Ctd_{"Ctd", dimless, dict}
{}
// Destructor 
mousse::turbulentDispersionModels::constantTurbulentDispersionCoefficient::
~constantTurbulentDispersionCoefficient()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::turbulentDispersionModels::constantTurbulentDispersionCoefficient::
D() const
{
  return
    Ctd_
    *pair_.dispersed()
    *pair_.continuous().rho()
    *pair_.continuous().turbulence().k();
}
