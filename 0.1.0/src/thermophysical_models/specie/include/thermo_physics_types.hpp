// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef thermo_physics_types_hpp_
#define thermo_physics_types_hpp_
#include "specie.hpp"
#include "perfect_gas.hpp"
#include "incompressible_perfect_gas.hpp"
#include "h_const_thermo.hpp"
#include "janaf_thermo.hpp"
#include "sensible_enthalpy.hpp"
#include "sensible_internal_energy.hpp"
#include "thermo.hpp"
#include "sutherland_transport.hpp"
#include "const_transport.hpp"
#include "ico_polynomial.hpp"
#include "h_polynomial_thermo.hpp"
#include "polynomial_transport.hpp"
namespace mousse
{
  // thermo physics types based on sensibleEnthalpy
  typedef
  constTransport
  <
    species::thermo
    <
      hConstThermo
      <
        perfectGas<specie>
      >,
      sensibleEnthalpy
    >
  > constGasHThermoPhysics;
  typedef
  sutherlandTransport
  <
    species::thermo
    <
      janafThermo
      <
        perfectGas<specie>
      >,
      sensibleEnthalpy
    >
  > gasHThermoPhysics;
  typedef
  constTransport
  <
    species::thermo
    <
      hConstThermo
      <
        incompressiblePerfectGas<specie>
      >,
      sensibleEnthalpy
    >
  > constIncompressibleGasHThermoPhysics;
  typedef
  sutherlandTransport
  <
    species::thermo
    <
      janafThermo
      <
        incompressiblePerfectGas<specie>
      >,
      sensibleEnthalpy
    >
  > incompressibleGasHThermoPhysics;
  typedef
  polynomialTransport
  <
    species::thermo
    <
      hPolynomialThermo
      <
        icoPolynomial<specie, 8>,
        8
      >,
      sensibleEnthalpy
    >,
    8
  > icoPoly8HThermoPhysics;
  // thermo physics types based on sensibleInternalEnergy
  typedef
  constTransport
  <
    species::thermo
    <
      hConstThermo
      <
        perfectGas<specie>
      >,
      sensibleInternalEnergy
    >
  > constGasEThermoPhysics;
  typedef
  sutherlandTransport
  <
    species::thermo
    <
      janafThermo
      <
        perfectGas<specie>
      >,
      sensibleInternalEnergy
    >
  > gasEThermoPhysics;
  typedef
  constTransport
  <
    species::thermo
    <
      hConstThermo
      <
        incompressiblePerfectGas<specie>
      >,
      sensibleInternalEnergy
    >
  > constIncompressibleGasEThermoPhysics;
  typedef
  sutherlandTransport
  <
    species::thermo
    <
      janafThermo
      <
        incompressiblePerfectGas<specie>
      >,
      sensibleInternalEnergy
    >
  > incompressibleGasEThermoPhysics;
  typedef
  polynomialTransport
  <
    species::thermo
    <
      hPolynomialThermo
      <
        icoPolynomial<specie, 8>,
        8
      >,
      sensibleInternalEnergy
    >,
    8
  > icoPoly8EThermoPhysics;
}
#endif
