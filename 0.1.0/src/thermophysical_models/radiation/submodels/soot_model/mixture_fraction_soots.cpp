// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mixture_fraction_soot.hpp"
#include "make_soot_types.hpp"
#include "thermo_physics_types.hpp"
// Static Data Members
makeSootTypesThermo(mixtureFractionSoot, gasHThermoPhysics);
makeSootTypesThermo(mixtureFractionSoot, gasEThermoPhysics);
