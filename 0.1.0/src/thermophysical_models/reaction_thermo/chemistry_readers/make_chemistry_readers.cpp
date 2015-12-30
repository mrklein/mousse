// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_reaction_thermo.hpp"
#include "thermo_physics_types.hpp"
#include "solid_thermo_physics_types.hpp"
#include "chemistry_reader.hpp"
#include "foam_chemistry_reader.hpp"
namespace mousse
{
// Solid chemistry readers based on sensibleEnthalpy
makeChemistryReader(constGasHThermoPhysics);
makeChemistryReader(gasHThermoPhysics);
makeChemistryReader(constIncompressibleGasHThermoPhysics);
makeChemistryReader(incompressibleGasHThermoPhysics);
makeChemistryReader(icoPoly8HThermoPhysics);
makeChemistryReaderType(foamChemistryReader, constGasHThermoPhysics);
makeChemistryReaderType(foamChemistryReader, gasHThermoPhysics);
makeChemistryReaderType
(
  foamChemistryReader,
  constIncompressibleGasHThermoPhysics
);
makeChemistryReaderType(foamChemistryReader, incompressibleGasHThermoPhysics);
makeChemistryReaderType(foamChemistryReader, icoPoly8HThermoPhysics);
// Solid chemistry readers based on sensibleInternalEnergy
makeChemistryReader(constGasEThermoPhysics);
makeChemistryReader(gasEThermoPhysics);
makeChemistryReader(constIncompressibleGasEThermoPhysics);
makeChemistryReader(incompressibleGasEThermoPhysics);
makeChemistryReader(icoPoly8EThermoPhysics);
makeChemistryReaderType(foamChemistryReader, constGasEThermoPhysics);
makeChemistryReaderType(foamChemistryReader, gasEThermoPhysics);
makeChemistryReaderType
(
  foamChemistryReader,
  constIncompressibleGasEThermoPhysics
);
makeChemistryReaderType(foamChemistryReader, incompressibleGasEThermoPhysics);
makeChemistryReaderType(foamChemistryReader, icoPoly8EThermoPhysics);
// Solid chemistry readers for solids based on sensibleInternalEnergy
makeChemistryReader(hConstSolidThermoPhysics);
makeChemistryReader(hPowerSolidThermoPhysics);
makeChemistryReader(hExpKappaConstSolidThermoPhysics);
makeChemistryReaderType(foamChemistryReader, hConstSolidThermoPhysics);
makeChemistryReaderType(foamChemistryReader, hPowerSolidThermoPhysics);
makeChemistryReaderType(foamChemistryReader, hExpKappaConstSolidThermoPhysics);
}  // namespace mousse
