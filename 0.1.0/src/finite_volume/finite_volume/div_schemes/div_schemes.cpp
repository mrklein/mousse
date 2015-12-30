// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "div_scheme.hpp"
namespace mousse
{
namespace fv
{
// Define the constructor function hash tables
defineTemplateRunTimeSelectionTable
(
  divScheme<vector>,
  Istream
);
defineTemplateRunTimeSelectionTable
(
  divScheme<sphericalTensor>,
  Istream
);
defineTemplateRunTimeSelectionTable
(
  divScheme<symmTensor>,
  Istream
);
defineTemplateRunTimeSelectionTable
(
  divScheme<tensor>,
  Istream
);
}  // namespace fv
}  // namespace mousse
