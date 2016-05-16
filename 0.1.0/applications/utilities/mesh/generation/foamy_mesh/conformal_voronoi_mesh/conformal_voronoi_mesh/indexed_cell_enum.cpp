// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "indexed_cell_enum.hpp"


// Static Data Members
template<>
const char*
mousse::NamedEnum<mousse::indexedCellEnum::cellTypes, 6>::names[] =
{
  "Unassigned",
  "Internal",
  "Surface",
  "FeatureEdge",
  "FeaturePoint",
  "Far"
};

const mousse::NamedEnum<mousse::indexedCellEnum::cellTypes, 6>
cellTypesNames_;

