// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mathematical_constants.hpp"
#include "universal_constants.hpp"
#include "electromagnetic_constants.hpp"
#include "atomic_constants.hpp"
#include "dimensioned_constants.hpp"


namespace mousse {
namespace constant {

const char* const atomic::group = "atomic";

// Note: cannot use dimless etc. since not guaranteed to be constructed
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  atomic::group,
  atomic::alpha,
  dimensionedScalar
  (
    "alpha",
    sqr(electromagnetic::e)
   /(
      dimensionedScalar("C", dimensionSet(0, 0, 0, 0, 0), 2.0)
     *electromagnetic::epsilon0
     *universal::h
     *universal::c
    )
  ),
  constantatomicalpha,
  "alpha"
);


DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  atomic::group,
  atomic::Rinf,
  dimensionedScalar
  (
    "Rinf",
    sqr(atomic::alpha)
   *atomic::me
   *universal::c
   /(
      mousse::dimensionedScalar
      (
        "C",
        dimensionSet(0, 0, 0, 0, 0),
        2.0
      )
     *universal::h
    )
  ),
  constantatomicRinf,
  "Rinf"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  atomic::group,
  atomic::a0,
  dimensionedScalar
  (
    "a0",
    atomic::alpha
   /(
      mousse::dimensionedScalar
      (
        "C",
        dimensionSet(0, 0, 0, 0, 0),
        4.0*mathematical::pi
      )
     *atomic::Rinf
    )
  ),
  constantatomica0,
  "a0"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  atomic::group,
  atomic::re,
  dimensionedScalar
  (
    "re",
    mousse::sqr(electromagnetic::e)
   /(
      mousse::dimensionedScalar
      (
        "C",
        dimensionSet(0, 0, 0, 0, 0),
        4.0*mathematical::pi
      )
     *electromagnetic::epsilon0
     *atomic::me
     *mousse::sqr(universal::c)
    )
  ),
  constantatomicre,
  "re"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  atomic::group,
  atomic::Eh,
  dimensionedScalar
  (
    "Eh",
    mousse::dimensionedScalar("C", dimensionSet(0, 0, 0, 0, 0), 2.0)
   *atomic::Rinf*universal::h*universal::c
  ),
  constantatomicEh,
  "Eh"
);
}  // namespace constant
}  // namespace mousse
