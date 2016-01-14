// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_reacting_multiphase_cloud.hpp"
#include "make_parcel_turbulence_dispersion_models.hpp"
#include "make_thermo_parcel_turbulence_forces.hpp"

namespace mousse
{
MAKE_THERMO_PARCEL_TURBULENCE_FORCES(basicReactingMultiphaseCloud);
MAKE_PARCEL_TURBULENCE_DISPERSION_MODELS(basicReactingMultiphaseCloud);
}
