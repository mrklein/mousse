// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "psi_thermo.hpp"
#include "make_thermo.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "peng_robinson_gas.hpp"
#include "h_const_thermo.hpp"
#include "e_const_thermo.hpp"
#include "janaf_thermo.hpp"
#include "sensible_enthalpy.hpp"
#include "sensible_internal_energy.hpp"
#include "thermo.hpp"
#include "const_transport.hpp"
#include "sutherland_transport.hpp"
#include "h_polynomial_thermo.hpp"
#include "polynomial_transport.hpp"
#include "he_psi_thermo.hpp"
#include "pure_mixture.hpp"
namespace mousse
{
// Enthalpy-based
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  hConstThermo,
  PengRobinsonGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  polynomialTransport,
  sensibleEnthalpy,
  hPolynomialThermo,
  PengRobinsonGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  polynomialTransport,
  sensibleEnthalpy,
  janafThermo,
  PengRobinsonGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  PengRobinsonGas,
  specie
);
// Internal-energy-based
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  eConstThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  sutherlandTransport,
  sensibleInternalEnergy,
  eConstThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hConstThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  sutherlandTransport,
  sensibleInternalEnergy,
  hConstThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  sutherlandTransport,
  sensibleInternalEnergy,
  janafThermo,
  perfectGas,
  specie
);
MAKE_THERMO
(
  psiThermo,
  hePsiThermo,
  pureMixture,
  sutherlandTransport,
  sensibleInternalEnergy,
  janafThermo,
  PengRobinsonGas,
  specie
);
}  // namespace mousse
