// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "div_scheme.hpp"

namespace mousse {
namespace fv {

// Define the constructor function hash tables
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  divScheme<vector>,
  Istream
);

DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  divScheme<sphericalTensor>,
  Istream
);

DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  divScheme<symmTensor>,
  Istream
);

DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE
(
  divScheme<tensor>,
  Istream
);

}  // namespace fv
}  // namespace mousse
