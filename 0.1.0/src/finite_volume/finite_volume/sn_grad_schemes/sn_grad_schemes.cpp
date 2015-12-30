// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sn_grad_scheme.hpp"
#include "hash_table.hpp"
namespace mousse
{
namespace fv
{
// Define the constructor function hash tables
defineTemplateRunTimeSelectionTable(snGradScheme<scalar>, Mesh);
defineTemplateRunTimeSelectionTable(snGradScheme<vector>, Mesh);
defineTemplateRunTimeSelectionTable(snGradScheme<sphericalTensor>, Mesh);
defineTemplateRunTimeSelectionTable(snGradScheme<symmTensor>, Mesh);
defineTemplateRunTimeSelectionTable(snGradScheme<tensor>, Mesh);
}  // namespace fv
}  // namespace mousse
