// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ddt_scheme.hpp"

namespace mousse {
namespace fv {

// Define the constructor function hash tables
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(ddtScheme<scalar>, Istream);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(ddtScheme<vector>, Istream);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(ddtScheme<sphericalTensor>, Istream);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(ddtScheme<symmTensor>, Istream);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(ddtScheme<tensor>, Istream);

}  // namespace fv
}  // namespace mousse

