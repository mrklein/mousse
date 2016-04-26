// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_reaction_thermo.hpp"
#include "psi_reaction_thermo.hpp"
#include "he_psi_thermo.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
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

// constTransport, hConstThermo
MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  homogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  inhomogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  veryInhomogeneousMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

// sutherlandTransport, hConstThermo
MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  homogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  inhomogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  veryInhomogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);

// sutherlandTransport, janafThermo
MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  homogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  inhomogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  perfectGas,
  specie
);

MAKE_REACTION_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  veryInhomogeneousMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  perfectGas,
  specie
);

// Multi-component thermo for sensible enthalpy
MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  multiComponentMixture,
  constGasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  multiComponentMixture,
  gasHThermoPhysics
);

// Multi-component thermo for internal energy
MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  multiComponentMixture,
  constGasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  multiComponentMixture,
  gasEThermoPhysics
);

// Multi-component reaction thermo for sensible enthalpy
MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  reactingMixture,
  constGasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  reactingMixture,
  gasHThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  singleStepReactingMixture,
  gasHThermoPhysics
);

// Multi-component reaction thermo for internal energy
MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  reactingMixture,
  constGasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  reactingMixture,
  gasEThermoPhysics
);

MAKE_REACTION_MIXTURE_THERMO
(
  psiThermo,
  psiReactionThermo,
  hePsiThermo,
  singleStepReactingMixture,
  gasEThermoPhysics
);

}  // namespace mousse

