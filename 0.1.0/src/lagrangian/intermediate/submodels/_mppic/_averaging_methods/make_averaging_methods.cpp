// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field.hpp"
#include "fvc_grad.hpp"
#include "poly_mesh_tet_decomposition.hpp"
#include "_basic.hpp"
#include "_dual.hpp"
#include "_moment.hpp"
// Scalar interpolation
defineNamedTemplateTypeNameAndDebug(mousse::AveragingMethod<mousse::scalar>, 0);
namespace mousse
{
  defineTemplateRunTimeSelectionTable
  (
    AveragingMethod<mousse::scalar>,
    dictionary
  );
}
// Vector interpolation
defineNamedTemplateTypeNameAndDebug(mousse::AveragingMethod<mousse::vector>, 0);
namespace mousse
{
  defineTemplateRunTimeSelectionTable
  (
    mousse::AveragingMethod<mousse::vector>,
    dictionary
  );
}
// Basic interpolation
defineNamedTemplateTypeNameAndDebug
(
  mousse::AveragingMethods::Basic<mousse::scalar>,
  0
);
mousse::AveragingMethod<mousse::scalar>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Basic<mousse::scalar> >
  addBasicscalarConstructorToTable_;
defineNamedTemplateTypeNameAndDebug
(
  mousse::AveragingMethods::Basic<mousse::vector>,
  0
);
mousse::AveragingMethod<mousse::vector>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Basic<mousse::vector> >
  addBasicvectorConstructorToTable_;
// Dual interpolation
defineNamedTemplateTypeNameAndDebug
(
  mousse::AveragingMethods::Dual<mousse::scalar>,
  0
);
mousse::AveragingMethod<mousse::scalar>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Dual<mousse::scalar> >
  addDualscalarConstructorToTable_;
defineNamedTemplateTypeNameAndDebug
(
  mousse::AveragingMethods::Dual<mousse::vector>,
  0
);
mousse::AveragingMethod<mousse::vector>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Dual<mousse::vector> >
  addDualvectorConstructorToTable_;
// Moment interpolation
defineNamedTemplateTypeNameAndDebug
(
  mousse::AveragingMethods::Moment<mousse::scalar>,
  0
);
mousse::AveragingMethod<mousse::scalar>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Moment<mousse::scalar> >
  addMomentscalarConstructorToTable_;
defineNamedTemplateTypeNameAndDebug
(
  mousse::AveragingMethods::Moment<mousse::vector>,
  0
);
mousse::AveragingMethod<mousse::vector>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Moment<mousse::vector> >
  addMomentvectorConstructorToTable_;
