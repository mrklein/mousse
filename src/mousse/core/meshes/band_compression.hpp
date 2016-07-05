#ifndef CORE_MESHES_BAND_COMPRESSION_HPP_
#define CORE_MESHES_BAND_COMPRESSION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_list.hpp"


namespace mousse {

//- Renumbers the addressing to reduce the band of the matrix.
//  The algorithm uses a simple search through the neighbour list
//  Returns the order in which the cells need to be visited (i.e. ordered to
//  original)
labelList bandCompression(const labelListList& addressing);

//- Renumber with addressing in losort form (neighbour + start in neighbour)
labelList bandCompression(const labelList& cellCells, const labelList& offsets);

}  // namespace mousse

#endif
