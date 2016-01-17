// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mathematical_constants.hpp"
#include "universal_constants.hpp"
#include "electromagnetic_constants.hpp"
#include "atomic_constants.hpp"
#include "dimensioned_constants.hpp"
namespace mousse
{
namespace constant
{
const char* const electromagnetic::group = "electromagnetic";
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  electromagnetic::group,
  electromagnetic::mu0,
  dimensionedScalar
  (
    "mu0",
    dimensionSet(1, 1, -2, 0, 0, -2, 0),
    4.0*mathematical::pi*1e-07
  ),
  constantelectromagneticmu0,
  "mu0"
);
// Note: cannot use dimless etc. since not guaranteed to be constructed
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  electromagnetic::group,
  electromagnetic::epsilon0,
  dimensionedScalar
  (
    "epsilon0",
    dimensionedScalar
    (
      "C",
      dimensionSet(0, 0, 0, 0, 0),
      1.0
    )
   /(electromagnetic::mu0*sqr(universal::c))
  ),
  constantelectromagneticepsilon0,
  "epsilon0"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  electromagnetic::group,
  electromagnetic::Z0,
  dimensionedScalar
  (
    "Z0",
    electromagnetic::mu0*universal::c
  ),
  constantelectromagneticZ0,
  "Z0"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  electromagnetic::group,
  electromagnetic::kappa,
  dimensionedScalar
  (
    "kappa",
    dimensionedScalar
    (
      "C",
      dimensionSet(0, 0, 0, 0, 0),
      1.0/(4.0*mathematical::pi)
    )
   /electromagnetic::epsilon0
  ),
  constantelectromagnetickappa,
  "kappa"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  electromagnetic::group,
  electromagnetic::G0,
  dimensionedScalar
  (
    "G0",
    dimensionedScalar("C", dimensionSet(0, 0, 0, 0, 0), 2)
   *sqr(electromagnetic::e)
   /universal::h
  ),
  constantelectromagneticG0,
  "G0"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  electromagnetic::group,
  electromagnetic::KJ,
  dimensionedScalar
  (
    "KJ",
    dimensionedScalar("C", dimensionSet(0, 0, 0, 0, 0), 2)
   *electromagnetic::e
   /universal::h
  ),
  constantelectromagneticKJ,
  "KJ"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  electromagnetic::group,
  electromagnetic::phi0,
  dimensionedScalar
  (
    "phi0",
    universal::h
   /(
      dimensionedScalar("C", dimensionSet(0, 0, 0, 0, 0), 2)
     *electromagnetic::e
    )
  ),
  constantelectromagneticphi0,
  "phi0"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  electromagnetic::group,
  electromagnetic::RK,
  dimensionedScalar
  (
    "RK",
    universal::h/mousse::sqr(electromagnetic::e)
  ),
  constantelectromagneticRK,
  "RK"
);
}  // namespace constant
}  // namespace mousse
