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
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::AveragingMethod<mousse::scalar>, 0);


namespace mousse {

DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  AveragingMethod<mousse::scalar>,
  dictionary
);

}


// Vector interpolation
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::AveragingMethod<mousse::vector>, 0);
namespace mousse {
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  mousse::AveragingMethod<mousse::vector>,
  dictionary
);
}


// Basic interpolation
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  mousse::AveragingMethods::Basic<mousse::scalar>,
  0
);
mousse::AveragingMethod<mousse::scalar>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Basic<mousse::scalar>>
  addBasicscalarConstructorToTable_;


DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  mousse::AveragingMethods::Basic<mousse::vector>,
  0
);
mousse::AveragingMethod<mousse::vector>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Basic<mousse::vector>>
  addBasicvectorConstructorToTable_;


// Dual interpolation
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  mousse::AveragingMethods::Dual<mousse::scalar>,
  0
);
mousse::AveragingMethod<mousse::scalar>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Dual<mousse::scalar>>
  addDualscalarConstructorToTable_;


DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  mousse::AveragingMethods::Dual<mousse::vector>,
  0
);
mousse::AveragingMethod<mousse::vector>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Dual<mousse::vector>>
  addDualvectorConstructorToTable_;


// Moment interpolation
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  mousse::AveragingMethods::Moment<mousse::scalar>,
  0
);
mousse::AveragingMethod<mousse::scalar>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Moment<mousse::scalar>>
  addMomentscalarConstructorToTable_;


DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  mousse::AveragingMethods::Moment<mousse::vector>,
  0
);
mousse::AveragingMethod<mousse::vector>::
adddictionaryConstructorToTable<mousse::AveragingMethods::Moment<mousse::vector>>
  addMomentvectorConstructorToTable_;

