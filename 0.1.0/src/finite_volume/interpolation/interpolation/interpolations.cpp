// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation.hpp"
namespace mousse
{
// Define the constructor function hash tables
defineTemplateRunTimeSelectionTable(interpolation<scalar>, dictionary);
defineTemplateRunTimeSelectionTable(interpolation<vector>, dictionary);
defineTemplateRunTimeSelectionTable(interpolation<sphericalTensor>, dictionary);
defineTemplateRunTimeSelectionTable(interpolation<symmTensor>, dictionary);
defineTemplateRunTimeSelectionTable(interpolation<tensor>, dictionary);
}  // namespace mousse
