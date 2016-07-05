// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "grad_scheme.hpp"
#include "hash_table.hpp"


namespace mousse {
namespace fv {

// Define the constructor function hash tables
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(gradScheme<scalar>, Istream);
DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE(gradScheme<vector>, Istream);

}  // namespace fv
}  // namespace mousse
