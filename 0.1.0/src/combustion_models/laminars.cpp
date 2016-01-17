// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_combustion_types.hpp"
#include "psi_chemistry_combustion.hpp"
#include "rho_chemistry_combustion.hpp"
#include "laminar.hpp"

MAKE_COMBUSTION_TYPES(laminar, psiChemistryCombustion, psiCombustionModel);
MAKE_COMBUSTION_TYPES(laminar, rhoChemistryCombustion, rhoCombustionModel);
