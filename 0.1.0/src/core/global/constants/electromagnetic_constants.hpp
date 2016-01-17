// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef electromagnetic_constants_hpp_
#define electromagnetic_constants_hpp_
#include "dimensioned_scalar.hpp"
namespace mousse
{
namespace constant
{
namespace electromagnetic
{
  //- Group name for electromagnetic constants
  extern const char* const group;
  //- Magnetic constant/permeability of free space: default SI units: [H/m]
  extern const dimensionedScalar mu0;
  //- Electric constant: default SI units: [F/m]
  extern const dimensionedScalar epsilon0;
  //- Characteristic impedance of a vacuum: default SI units: [ohm]
  extern const dimensionedScalar Z0;
  //- Coulomb constant: default SI units: [N.m2/C2]
  extern const dimensionedScalar kappa;
  //- Conductance quantum: default SI units: [S]
  extern const dimensionedScalar G0;
  //- Josephson constant: default SI units: [Hz/V]
  extern const dimensionedScalar KJ;
  //- Magnetic flux quantum: default SI units: [Wb]
  extern const dimensionedScalar phi0;
  //- Von Klitzing constant: default SI units: [ohm]
  extern const dimensionedScalar RK;
}  // namespace electromagnetic
}  // namespace constant
}  // namespace mousse
#endif
