// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_combustion_types.hpp"
#include "psi_chemistry_combustion.hpp"
#include "rho_chemistry_combustion.hpp"
#include "pasr.hpp"
makeCombustionTypes(PaSR, psiChemistryCombustion, psiCombustionModel);
makeCombustionTypes(PaSR, rhoChemistryCombustion, rhoCombustionModel);
