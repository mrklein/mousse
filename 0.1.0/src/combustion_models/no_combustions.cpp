// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_combustion_types.hpp"
#include "psi_combustion_model.hpp"
#include "rho_combustion_model.hpp"
#include "psi_thermo_combustion.hpp"
#include "rho_thermo_combustion.hpp"
#include "no_combustion.hpp"
makeCombustionTypes
(
  noCombustion,
  psiThermoCombustion,
  psiCombustionModel
);
makeCombustionTypes
(
  noCombustion,
  rhoThermoCombustion,
  rhoCombustionModel
);
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
