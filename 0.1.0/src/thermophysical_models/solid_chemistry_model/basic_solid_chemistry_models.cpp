// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_solid_chemistry_model.hpp"
#include "pyrolysis_chemistry_model.hpp"
#include "basic_solid_chemistry_model.hpp"
#include "solid_chemistry_model.hpp"
#include "solid_thermo_physics_types.hpp"
#include "thermo_physics_types.hpp"


// hConstSolidThermoPhysics
MAKE_SOLID_CHEMISTRY_MODEL
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hConstSolidThermoPhysics
);

MAKE_SOLID_GAS_CHEMISTRY_MODEL
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hConstSolidThermoPhysics,
  gasHThermoPhysics
);

// hPowerSolidThermoPhysics
MAKE_SOLID_CHEMISTRY_MODEL
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hPowerSolidThermoPhysics
);

MAKE_SOLID_GAS_CHEMISTRY_MODEL
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hPowerSolidThermoPhysics,
  gasHThermoPhysics
);

// hExpKappaConstSolidThermoPhysics
MAKE_SOLID_CHEMISTRY_MODEL
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hExpKappaConstSolidThermoPhysics
);

MAKE_SOLID_GAS_CHEMISTRY_MODEL
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hExpKappaConstSolidThermoPhysics,
  gasHThermoPhysics
);

