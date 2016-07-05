// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "add_to_run_time_selection_table.hpp"
#include "interface_composition_model.hpp"
#include "_interface_composition_model.hpp"
#include "henry.hpp"
#include "non_random_two_liquid.hpp"
#include "raoult.hpp"
#include "saturated.hpp"
#include "make_reaction_thermo.hpp"
#include "thermo_physics_types.hpp"
#include "rho_const.hpp"
#include "perfect_fluid.hpp"
#include "pure_mixture.hpp"
#include "multi_component_mixture.hpp"
#include "reacting_mixture.hpp"
#include "specie_mixture.hpp"
#include "rho_thermo.hpp"
#include "rho_reaction_thermo.hpp"
#include "he_rho_thermo.hpp"
namespace mousse
{
  typedef
    constTransport
    <
      species::thermo
      <
        hConstThermo
        <
          perfectFluid<specie>
        >,
        sensibleInternalEnergy
      >
    > constFluidEThermoPhysics;
};
namespace mousse
{
  // multi-component liquid
  MAKE_REACTION_THERMO
  (
    rhoThermo,
    rhoReactionThermo,
    heRhoThermo,
    multiComponentMixture,
    constTransport,
    sensibleInternalEnergy,
    hConstThermo,
    perfectFluid,
    specie
  );
}
namespace mousse
{
  using namespace interfaceCompositionModels;
  // multi-component gas in the presence of a pure liquid
  MAKE_INTERFACE_COMPOSITION_TYPE
  (
    Saturated,
    heRhoThermo,
    rhoReactionThermo,
    multiComponentMixture,
    gasEThermoPhysics,
    heRhoThermo,
    rhoThermo,
    pureMixture,
    constFluidEThermoPhysics
  );
  // reacting gas in the presence of a pure liquid
  MAKE_INTERFACE_COMPOSITION_TYPE
  (
    Saturated,
    heRhoThermo,
    rhoReactionThermo,
    reactingMixture,
    gasEThermoPhysics,
    heRhoThermo,
    rhoThermo,
    pureMixture,
    constFluidEThermoPhysics
  );
  // multi-component gas in the presence of a multi-component liquid
  MAKE_SPECIE_INTERFACE_COMPOSITION_TYPE
  (
    Saturated,
    heRhoThermo,
    rhoReactionThermo,
    multiComponentMixture,
    constGasEThermoPhysics,
    heRhoThermo,
    rhoReactionThermo,
    multiComponentMixture,
    constFluidEThermoPhysics
  );
  // multi-component liquid in the presence of a multi-component gas
  MAKE_SPECIE_INTERFACE_COMPOSITION_TYPE
  (
    Henry,
    heRhoThermo,
    rhoReactionThermo,
    multiComponentMixture,
    constFluidEThermoPhysics,
    heRhoThermo,
    rhoReactionThermo,
    multiComponentMixture,
    constGasEThermoPhysics
  );
}
