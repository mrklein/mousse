// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "make_solid_thermo.hpp"
#include "solid_thermo.hpp"
#include "he_solid_thermo.hpp"
#include "specie.hpp"
#include "rho_const.hpp"
#include "h_const_thermo.hpp"
#include "h_power_thermo.hpp"
#include "h_polynomial_thermo.hpp"
#include "const_iso_solid_transport.hpp"
#include "const_an_iso_solid_transport.hpp"
#include "exponential_solid_transport.hpp"
#include "polynomial_solid_transport.hpp"
#include "pure_mixture.hpp"
#include "sensible_enthalpy.hpp"
#include "sensible_internal_energy.hpp"
#include "thermo.hpp"
#include "solid_thermo_physics_types.hpp"
namespace mousse
{
/* * * * * * * * * * * * * * * * * Enthalpy-based * * * * * * * * * * * * * */
makeSolidThermo
(
  solidThermo,
  heSolidThermo,
  pureMixture,
  constIsoSolidTransport,
  sensibleEnthalpy,
  hConstThermo,
  rhoConst,
  specie
);
makeSolidThermo
(
  solidThermo,
  heSolidThermo,
  pureMixture,
  constAnIsoSolidTransport,
  sensibleEnthalpy,
  hConstThermo,
  rhoConst,
  specie
);
makeSolidThermo
(
  solidThermo,
  heSolidThermo,
  pureMixture,
  exponentialSolidTransport,
  sensibleEnthalpy,
  hPowerThermo,
  rhoConst,
  specie
);
makeSolidThermoPhysicsType
(
  solidThermo,
  heSolidThermo,
  pureMixture,
  hTransportThermoPoly8SolidThermoPhysics
);
}  // namespace mousse
