// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rho_thermo.hpp"
#include "make_thermo.hpp"
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "incompressible_perfect_gas.hpp"
#include "rho_const.hpp"
#include "perfect_fluid.hpp"
#include "peng_robinson_gas.hpp"
#include "adiabatic_perfect_fluid.hpp"
#include "h_const_thermo.hpp"
#include "janaf_thermo.hpp"
#include "sensible_enthalpy.hpp"
#include "sensible_internal_energy.hpp"
#include "thermo.hpp"
#include "const_transport.hpp"
#include "sutherland_transport.hpp"
#include "ico_polynomial.hpp"
#include "h_polynomial_thermo.hpp"
#include "polynomial_transport.hpp"
#include "he_rho_thermo.hpp"
#include "pure_mixture.hpp"
namespace mousse
{
/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  perfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  rhoConst,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  perfectFluid,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  adiabaticPerfectFluid,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  polynomialTransport,
  sensibleEnthalpy,
  hPolynomialThermo,
  icoPolynomial,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleEnthalpy,
  hConstThermo,
  incompressiblePerfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  hConstThermo,
  incompressiblePerfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  janafThermo,
  incompressiblePerfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleEnthalpy,
  hConstThermo,
  PengRobinsonGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  polynomialTransport,
  sensibleEnthalpy,
  hPolynomialThermo,
  PengRobinsonGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  polynomialTransport,
  sensibleEnthalpy,
  janafThermo,
  PengRobinsonGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hConstThermo,
  perfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleInternalEnergy,
  hConstThermo,
  perfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleInternalEnergy,
  janafThermo,
  perfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hConstThermo,
  rhoConst,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hConstThermo,
  perfectFluid,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hConstThermo,
  adiabaticPerfectFluid,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  polynomialTransport,
  sensibleInternalEnergy,
  hPolynomialThermo,
  icoPolynomial,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  constTransport,
  sensibleInternalEnergy,
  hConstThermo,
  incompressiblePerfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleInternalEnergy,
  hConstThermo,
  incompressiblePerfectGas,
  specie
);
makeThermo
(
  rhoThermo,
  heRhoThermo,
  pureMixture,
  sutherlandTransport,
  sensibleInternalEnergy,
  janafThermo,
  incompressiblePerfectGas,
  specie
);
}  // namespace mousse
