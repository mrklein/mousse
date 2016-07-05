#ifndef CORE_PRIMITIVES_VECTOR_VECTOR_LIST_IO_LIST_HPP_
#define CORE_PRIMITIVES_VECTOR_VECTOR_LIST_IO_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vector_list.hpp"
#include "compact_io_list.hpp"


namespace mousse {

typedef IOList<vectorList> vectorListIOList;
typedef CompactIOList<vectorList, vector> vectorListCompactIOList;

}

#endif
