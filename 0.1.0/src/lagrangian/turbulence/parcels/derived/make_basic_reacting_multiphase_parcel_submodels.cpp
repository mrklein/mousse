// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_reacting_multiphase_cloud.hpp"
#include "make_parcel_turbulence_dispersion_models.hpp"
#include "make_thermo_parcel_turbulence_forces.hpp"

namespace mousse
{
  makeThermoParcelTurbulenceForces(basicReactingMultiphaseCloud);
  makeParcelTurbulenceDispersionModels(basicReactingMultiphaseCloud);
}
