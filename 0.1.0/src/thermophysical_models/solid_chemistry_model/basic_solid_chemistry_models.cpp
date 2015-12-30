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
makeSolidChemistryModel
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hConstSolidThermoPhysics
);
makeSolidGasChemistryModel
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hConstSolidThermoPhysics,
  gasHThermoPhysics
);
// hPowerSolidThermoPhysics
makeSolidChemistryModel
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hPowerSolidThermoPhysics
);
makeSolidGasChemistryModel
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hPowerSolidThermoPhysics,
  gasHThermoPhysics
);
// hExpKappaConstSolidThermoPhysics
makeSolidChemistryModel
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hExpKappaConstSolidThermoPhysics
);
makeSolidGasChemistryModel
(
  solidChemistryModel,
  pyrolysisChemistryModel,
  basicSolidChemistryModel,
  hExpKappaConstSolidThermoPhysics,
  gasHThermoPhysics
);
