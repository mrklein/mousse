// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vector_list.hpp"
#include "token.hpp"


namespace mousse {

DEFINE_COMPOUND_TYPE_NAME(List<vector>, vectorList);
ADD_COMPOUND_TO_RUN_TIME_SELECTION_TABLE(List<vector>, vectorList);

}  // namespace mousse
