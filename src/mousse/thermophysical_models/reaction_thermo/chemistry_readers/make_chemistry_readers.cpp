// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_reaction_thermo.hpp"
#include "thermo_physics_types.hpp"
#include "solid_thermo_physics_types.hpp"
#include "chemistry_reader.hpp"
#include "foam_chemistry_reader.hpp"


namespace mousse {

// Solid chemistry readers based on sensibleEnthalpy
MAKE_CHEMISTRY_READER(constGasHThermoPhysics);
MAKE_CHEMISTRY_READER(gasHThermoPhysics);
MAKE_CHEMISTRY_READER(constIncompressibleGasHThermoPhysics);
MAKE_CHEMISTRY_READER(incompressibleGasHThermoPhysics);
MAKE_CHEMISTRY_READER(icoPoly8HThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, constGasHThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, gasHThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE
(
  foamChemistryReader,
  constIncompressibleGasHThermoPhysics
);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, incompressibleGasHThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, icoPoly8HThermoPhysics);
// Solid chemistry readers based on sensibleInternalEnergy
MAKE_CHEMISTRY_READER(constGasEThermoPhysics);
MAKE_CHEMISTRY_READER(gasEThermoPhysics);
MAKE_CHEMISTRY_READER(constIncompressibleGasEThermoPhysics);
MAKE_CHEMISTRY_READER(incompressibleGasEThermoPhysics);
MAKE_CHEMISTRY_READER(icoPoly8EThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, constGasEThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, gasEThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE
(
  foamChemistryReader,
  constIncompressibleGasEThermoPhysics
);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, incompressibleGasEThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, icoPoly8EThermoPhysics);
// Solid chemistry readers for solids based on sensibleInternalEnergy
MAKE_CHEMISTRY_READER(hConstSolidThermoPhysics);
MAKE_CHEMISTRY_READER(hPowerSolidThermoPhysics);
MAKE_CHEMISTRY_READER(hExpKappaConstSolidThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, hConstSolidThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, hPowerSolidThermoPhysics);
MAKE_CHEMISTRY_READER_TYPE(foamChemistryReader, hExpKappaConstSolidThermoPhysics);

}  // namespace mousse

