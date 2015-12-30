// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_size_distribution_function_object.hpp"
// Static Data Members
namespace mousse
{
  defineNamedTemplateTypeNameAndDebug
  (
    regionSizeDistributionFunctionObject,
    0
  );
  addToRunTimeSelectionTable
  (
    functionObject,
    regionSizeDistributionFunctionObject,
    dictionary
  );
}
