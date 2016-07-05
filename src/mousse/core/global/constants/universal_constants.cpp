// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "universal_constants.hpp"
#include "mathematical_constants.hpp"
#include "dimensioned_constants.hpp"
namespace mousse
{
namespace constant
{
const char* const universal::group = "universal";
// Note: cannot use dimless etc. since not guaranteed to be constructed
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  "universal",
  universal::hr,
  dimensionedScalar
  (
    "hr",
    universal::h
   /(
      dimensionedScalar
      (
        "C",
        dimensionSet(0, 0, 0, 0, 0),//dimless
        mathematical::twoPi
      )
    )
  ),
  constantuniversalhr,
  "hr"
);
}  // namespace constant
}  // namespace mousse
