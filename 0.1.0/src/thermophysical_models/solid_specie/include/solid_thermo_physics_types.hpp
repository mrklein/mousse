#ifndef THERMOPHYSICAL_MODELS_SOLID_SPECIE_INCLUDE_SOLID_THERMO_PHYSICS_TYPES_HPP_
#define THERMOPHYSICAL_MODELS_SOLID_SPECIE_INCLUDE_SOLID_THERMO_PHYSICS_TYPES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "specie.hpp"
#include "rho_const.hpp"
#include "h_const_thermo.hpp"
#include "h_polynomial_thermo.hpp"
#include "h_power_thermo.hpp"
#include "const_iso_solid_transport.hpp"
#include "const_an_iso_solid_transport.hpp"
#include "exponential_solid_transport.hpp"
#include "polynomial_solid_transport.hpp"
#include "sensible_enthalpy.hpp"
#include "thermo.hpp"
namespace mousse
{
  typedef
    constIsoSolidTransport
    <
      species::thermo
      <
        hConstThermo
        <
          rhoConst<specie>
        >,
        sensibleEnthalpy
      >
    > hConstSolidThermoPhysics;
  typedef
    exponentialSolidTransport
    <
      species::thermo
      <
        hPowerThermo
        <
          rhoConst<specie>
        >,
        sensibleEnthalpy
      >
    > hPowerSolidThermoPhysics;
  typedef
    polynomialSolidTransport
    <
      species::thermo
      <
        hPolynomialThermo
        <
          rhoConst<specie>,
          8
        >,
        sensibleEnthalpy
      >,
      8
    > hTransportThermoPoly8SolidThermoPhysics;
  typedef
    constIsoSolidTransport
    <
      species::thermo
      <
        hPowerThermo
        <
          rhoConst<specie>
        >,
        sensibleEnthalpy
      >
    > hExpKappaConstSolidThermoPhysics;
}
#endif
