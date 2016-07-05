// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mathematical_constants.hpp"
#include "universal_constants.hpp"
#include "electromagnetic_constants.hpp"
#include "physico_chemical_constants.hpp"
#include "dimensioned_constants.hpp"
namespace mousse
{
namespace constant
{
const char* const physicoChemical::group = "physicoChemical";
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  physicoChemical::group,
  physicoChemical::R,
  dimensionedScalar
  (
    "R",
    physicoChemical::NA*physicoChemical::k
  ),
  constantphysicoChemicalR,
  "R"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  physicoChemical::group,
  physicoChemical::F,
  dimensionedScalar
  (
    "F",
    physicoChemical::NA*electromagnetic::e
  ),
  constantphysicoChemicalF,
  "F"
);
// Note: cannot use dimless etc. since not guaranteed to be constructed
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  physicoChemical::group,
  physicoChemical::sigma,
  dimensionedScalar
  (
    "sigma",
    mousse::dimensionedScalar
    (
      "C",
      dimensionSet(0, 0, 0, 0, 0),    //mousse::dimless,
      mousse::sqr(mathematical::pi)/60.0
    )
   *mousse::pow4(physicoChemical::k)
   /(pow3(universal::hr)*sqr(universal::c))
  ),
  constantphysicoChemicalsigma,
  "sigma"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  physicoChemical::group,
  physicoChemical::b,
  dimensionedScalar
  (
    "b",
    (universal::h*universal::c/physicoChemical::k)
   /mousse::dimensionedScalar
    (
      "C",
      dimensionSet(0, 0, 0, 0, 0),    //mousse::dimless
      4.965114231
    )
  ),
  constantphysicoChemicalb,
  "b"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  physicoChemical::group,
  physicoChemical::c1,
  dimensionedScalar
  (
    "c1",
    mousse::dimensionedScalar
    (
      "C",
      dimensionSet(0, 0, 0, 0, 0),    //mousse::dimless,
      mathematical::twoPi
    )
   *universal::h*mousse::sqr(universal::c)
  ),
  constantphysicoChemicalc1,
  "c1"
);
DEFINE_DIMENSIONED_CONSTANT_WITH_DEFAULT
(
  physicoChemical::group,
  physicoChemical::c2,
  dimensionedScalar
  (
    "c2",
    universal::h*universal::c/physicoChemical::k
  ),
  constantphysicoChemicalc2,
  "c2"
);
}  // namespace constant
}  // namespace mousse
