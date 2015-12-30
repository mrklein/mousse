// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_solid_chemistry_solver_type.hpp"
#include "solid_thermo_physics_types.hpp"
#include "thermo_physics_types.hpp"
#include "pyrolysis_chemistry_model.hpp"
#include "basic_solid_chemistry_model.hpp"
namespace mousse
{
  makeSolidChemistrySolverTypes
  (
    pyrolysisChemistryModel,
    basicSolidChemistryModel,
    hConstSolidThermoPhysics,
    gasHThermoPhysics
  )
  makeSolidChemistrySolverTypes
  (
    pyrolysisChemistryModel,
    basicSolidChemistryModel,
    hPowerSolidThermoPhysics,
    gasHThermoPhysics
  )
  makeSolidChemistrySolverTypes
  (
    pyrolysisChemistryModel,
    basicSolidChemistryModel,
    hExpKappaConstSolidThermoPhysics,
    gasHThermoPhysics
  )
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
