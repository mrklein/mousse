// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_reaction_thermo.hpp"
#include "make_thermo.hpp"
#include "rho_reaction_thermo.hpp"
#include "he_rho_thermo.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "perfect_fluid.hpp"
#include "rho_const.hpp"
#include "sensible_enthalpy.hpp"
#include "h_ref_const_thermo.hpp"
#include "const_transport.hpp"
#include "pure_mixture.hpp"
#include "multi_component_mixture.hpp"
#include "thermo_physics_types.hpp"
namespace mousse
{
// Thermo type typedefs:
typedef
constTransport
<
  species::thermo
  <
    hRefConstThermo
    <
      perfectGas<specie>
    >,
    sensibleEnthalpy
  >
> constRefGasHThermoPhysics;
typedef
constTransport
<
  species::thermo
  <
    hRefConstThermo
    <
      perfectFluid<specie>
    >,
    sensibleEnthalpy
  >
> constRefFluidHThermoPhysics;
typedef
constTransport
<
  species::thermo
  <
    hRefConstThermo
    <
      perfectGas<specie>
    >,
    sensibleInternalEnergy
  >
> constRefGasEThermoPhysics;
typedef
constTransport
<
  species::thermo
  <
    hRefConstThermo
    <
      perfectFluid<specie>
    >,
    sensibleInternalEnergy
  >
> constRefFluidEThermoPhysics;
typedef
constTransport
<
  species::thermo
  <
    hRefConstThermo
    <
      rhoConst<specie>
    >,
    sensibleInternalEnergy
  >
> constRefRhoConstEThermoPhysics;
typedef
constTransport
<
  species::thermo
  <
    hRefConstThermo
    <
      rhoConst<specie>
    >,
    sensibleEnthalpy
  >
> constRefRhoConstHThermoPhysics;
// pureMixture, sensibleEnthalpy:
MAKE_THERMO
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hRefConstThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hRefConstThermo,
  perfectFluid,
  specie
);
MAKE_THERMO
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hRefConstThermo,
  rhoConst,
  specie
);
// pureMixture, sensibleInternalEnergy:
MAKE_THERMO
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hRefConstThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hRefConstThermo,
  perfectFluid,
  specie
);
MAKE_THERMO
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hRefConstThermo,
  rhoConst,
  specie
);
// multiComponentMixture, sensibleInternalEnergy:
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constRefGasEThermoPhysics
);
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constRefFluidEThermoPhysics
);
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constRefRhoConstEThermoPhysics
);
// multiComponentMixture, sensibleEnthalpy:
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constRefRhoConstHThermoPhysics
);
MAKE_REACTION_MIXTURE_THERMO
(
  rhoThermo,
  rhoReactionThermo,
  heRhoThermo,
  multiComponentMixture,
  constRefGasHThermoPhysics
);
}  // namespace mousse
