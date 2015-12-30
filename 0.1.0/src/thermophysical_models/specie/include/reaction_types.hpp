// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef reaction_types_hpp_
#define reaction_types_hpp_
#include "thermo_physics_types.hpp"
#include "reaction.hpp"
#include "ico_polynomial.hpp"
#include "h_polynomial_thermo.hpp"
#include "polynomial_transport.hpp"
namespace mousse
{
  // sensible enthalpy based reactions
  typedef Reaction<constGasHThermoPhysics> constGasHReaction;
  typedef Reaction<gasHThermoPhysics> gasHReaction;
  typedef Reaction<constIncompressibleGasHThermoPhysics>
    constIncompressibleGasHReaction;
  typedef Reaction<incompressibleGasHThermoPhysics>
    incompressibleGasHReaction;
  typedef Reaction<icoPoly8HThermoPhysics> icoPoly8HReaction;
  // internal ennergy based reactions
  typedef Reaction<constGasEThermoPhysics> constGasEReaction;
  typedef Reaction<gasEThermoPhysics> gasEReaction;
  typedef Reaction<constIncompressibleGasEThermoPhysics>
    constIncompressibleGasEReaction;
  typedef Reaction<incompressibleGasEThermoPhysics>
    incompressibleGasEReaction;
  typedef Reaction<icoPoly8EThermoPhysics> icoPoly8EReaction;
}
#endif
