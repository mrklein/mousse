// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_reaction_thermo.hpp"
#include "rho_reaction_thermo.hpp"
#include "he_rho_thermo.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "incompressible_perfect_gas.hpp"
#include "h_const_thermo.hpp"
#include "janaf_thermo.hpp"
#include "sensible_enthalpy.hpp"
#include "thermo.hpp"
#include "const_transport.hpp"
#include "sutherland_transport.hpp"
#include "homogeneous_mixture.hpp"
#include "inhomogeneous_mixture.hpp"
#include "very_inhomogeneous_mixture.hpp"
#include "multi_component_mixture.hpp"
#include "reacting_mixture.hpp"
#include "single_step_reacting_mixture.hpp"
#include "thermo_physics_types.hpp"


namespace mousse {

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  homogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  inhomogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  veryInhomogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  homogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  inhomogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  veryInhomogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  homogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  incompressiblePerfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  inhomogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  incompressiblePerfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  veryInhomogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  incompressiblePerfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  homogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  incompressiblePerfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  inhomogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  incompressiblePerfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  veryInhomogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  incompressiblePerfectGas,
  specie
);

// Multi-component thermo for internal energy
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constGasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  gasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constIncompressibleGasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  incompressibleGasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  icoPoly8EThermoPhysics
);

// Multi-component reaction thermo
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  constGasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  gasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  constIncompressibleGasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  incompressibleGasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  icoPoly8EThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  singleStepReactingMixture,
  gasEThermoPhysics
);

// Multi-component thermo for sensible enthalpy
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constGasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  gasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constIncompressibleGasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  incompressibleGasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  icoPoly8HThermoPhysics
);
// Multi-component reaction thermo
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  constGasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  gasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  constIncompressibleGasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  incompressibleGasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  reactingMixture,
  icoPoly8HThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  singleStepReactingMixture,
  gasHThermoPhysics
);

}  // namespace mousse

