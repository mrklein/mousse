// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sn_grad_scheme.hpp"
#include "hash_table.hpp"
#include "surface_fields.hpp"


namespace mousse {
namespace fv {

// Define the constructor function hash tables
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(snGradScheme<scalar>, Mesh);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(snGradScheme<vector>, Mesh);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(snGradScheme<sphericalTensor>, Mesh);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(snGradScheme<symmTensor>, Mesh);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(snGradScheme<tensor>, Mesh);

}  // namespace fv
}  // namespace mousse

