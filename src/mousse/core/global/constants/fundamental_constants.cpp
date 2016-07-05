// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fundamental_constants.hpp"
#include "universal_constants.hpp"
#include "electromagnetic_constants.hpp"
#include "atomic_constants.hpp"
#include "physico_chemical_constants.hpp"
#include "dimensioned_constants.hpp"
// Universal constants
namespace mousse
{
namespace constant
{
DEFINE_DIMENSIONED_CONSTANT
(
  universal::group,
  universal::c,
  constantuniversalc,
  "c"
);

DEFINE_DIMENSIONED_CONSTANT
(
  universal::group,
  universal::G,
  constantuniversalG,
  "G"
);

DEFINE_DIMENSIONED_CONSTANT
(
  universal::group,
  universal::h,
  constantuniversalh,
  "h"
);

// Electromagnetic
DEFINE_DIMENSIONED_CONSTANT
(
  electromagnetic::group,
  electromagnetic::e,
  constantelectromagnetice,
  "e"
);

// Atomic
DEFINE_DIMENSIONED_CONSTANT
(
  atomic::group,
  atomic::me,
  constantatomicme,
  "me"
);

DEFINE_DIMENSIONED_CONSTANT
(
  atomic::group,
  atomic::mp,
  constantatomicmp,
  "mp"
);

// Physico-chemical
DEFINE_DIMENSIONED_CONSTANT
(
  physicoChemical::group,
  physicoChemical::mu,
  constantphysicoChemicalmu,
  "mu"
);

// Note: cannot use dimless etc since not guaranteed to be constructed
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  physicoChemical::group,
  physicoChemical::NA,
  mousse::dimensionedScalar
  (
    "NA",
    dimensionSet(0, 0, 0, 0, -1), //mousse::dimless/mousse::dimMoles,
    6.0221417930e+23
  ),
  constantphysicoChemicalNA,
  "NA"
);

DEFINE_DIMENSIONED_CONSTANT
(
  physicoChemical::group,
  physicoChemical::k,
  constantphysicoChemicalk,
  "k"
);

// Standard
DEFINE_DIMENSIONED_CONSTANT
(
  "standard",
  standard::Pstd,
  constantstandardPstd,
  "Pstd"
);

DEFINE_DIMENSIONED_CONSTANT
(
  "standard",
  standard::Tstd,
  constantstandardTstd,
  "Tstd"
);

}  // namespace constant
}  // namespace mousse
