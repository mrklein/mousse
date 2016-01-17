// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation.hpp"

namespace mousse
{

// Define the constructor function hash tables
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(interpolation<scalar>, dictionary);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(interpolation<vector>, dictionary);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(interpolation<sphericalTensor>, dictionary);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(interpolation<symmTensor>, dictionary);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(interpolation<tensor>, dictionary);

}  // namespace mousse
