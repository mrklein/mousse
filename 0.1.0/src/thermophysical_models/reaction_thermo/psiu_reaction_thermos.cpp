// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psiu_reaction_thermo.hpp"
#include "heheu_psi_thermo.hpp"
#include "make_reaction_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "h_const_thermo.hpp"
#include "janaf_thermo.hpp"
#include "thermo.hpp"
#include "const_transport.hpp"
#include "sutherland_transport.hpp"
#include "absolute_enthalpy.hpp"
#include "absolute_internal_energy.hpp"
#include "homogeneous_mixture.hpp"
#include "inhomogeneous_mixture.hpp"
#include "very_inhomogeneous_mixture.hpp"
#include "multi_component_mixture.hpp"
#include "egr_mixture.hpp"
namespace mousse
{
// * * * * * * * * * * * * * * * * h-hu-Thermos * * * * * * * * * * * * * * * //
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  homogeneousMixture,
  constTransport,
  absoluteEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  inhomogeneousMixture,
  constTransport,
  absoluteEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  veryInhomogeneousMixture,
  constTransport,
  absoluteEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  egrMixture,
  constTransport,
  absoluteEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  homogeneousMixture,
  sutherlandTransport,
  absoluteEnthalpy,
  janafThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  inhomogeneousMixture,
  sutherlandTransport,
  absoluteEnthalpy,
  janafThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  veryInhomogeneousMixture,
  sutherlandTransport,
  absoluteEnthalpy,
  janafThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  egrMixture,
  sutherlandTransport,
  absoluteEnthalpy,
  janafThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  homogeneousMixture,
  sutherlandTransport,
  absoluteInternalEnergy,
  janafThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  inhomogeneousMixture,
  sutherlandTransport,
  absoluteInternalEnergy,
  janafThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  veryInhomogeneousMixture,
  sutherlandTransport,
  absoluteInternalEnergy,
  janafThermo,
  perfectGas,
  specie
);
makeReactionThermo
(
  psiThermo,
  psiuReactionThermo,
  heheuPsiThermo,
  egrMixture,
  sutherlandTransport,
  absoluteInternalEnergy,
  janafThermo,
  perfectGas,
  specie
);
}  // namespace mousse
