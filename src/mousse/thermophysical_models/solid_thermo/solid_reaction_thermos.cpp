// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_reacting_solid_thermo.hpp"
#include "solid_reaction_thermo.hpp"
#include "he_solid_thermo.hpp"
#include "specie.hpp"
#include "rho_const.hpp"
#include "h_const_thermo.hpp"
#include "h_power_thermo.hpp"
#include "const_iso_solid_transport.hpp"
#include "const_an_iso_solid_transport.hpp"
#include "exponential_solid_transport.hpp"
#include "reacting_mixture.hpp"
#include "multi_component_mixture.hpp"
#include "sensible_enthalpy.hpp"
#include "thermo.hpp"


namespace mousse {

// Enthalpy-based
MAKE_REACTING_SOLID_THERMO
(
  solidReactionThermo,
  heSolidThermo,
  reactingMixture,
  constIsoSolidTransport,
  sensibleEnthalpy,
  hConstThermo,
  rhoConst,
  specie
);

MAKE_REACTING_SOLID_THERMO
(
  solidReactionThermo,
  heSolidThermo,
  reactingMixture,
  constIsoSolidTransport,
  sensibleEnthalpy,
  hPowerThermo,
  rhoConst,
  specie
);

MAKE_REACTING_SOLID_THERMO
(
  solidThermo,
  heSolidThermo,
  multiComponentMixture,
  constIsoSolidTransport,
  sensibleEnthalpy,
  hConstThermo,
  rhoConst,
  specie
);

}  // namespace mousse

