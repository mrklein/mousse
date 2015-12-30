// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ddt_scheme.hpp"
namespace mousse
{
namespace fv
{
// Define the constructor function hash tables
defineTemplateRunTimeSelectionTable(ddtScheme<scalar>, Istream);
defineTemplateRunTimeSelectionTable(ddtScheme<vector>, Istream);
defineTemplateRunTimeSelectionTable(ddtScheme<sphericalTensor>, Istream);
defineTemplateRunTimeSelectionTable(ddtScheme<symmTensor>, Istream);
defineTemplateRunTimeSelectionTable(ddtScheme<tensor>, Istream);
}  // namespace fv
}  // namespace mousse
