// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::indexedCellEnum
// Description
// SourceFiles
//   indexed_cell_enum.cpp
#ifndef indexed_cell_enum_hpp_
#define indexed_cell_enum_hpp_
#include "named_enum.hpp"
namespace mousse
{
class indexedCellEnum
{
public:
  enum cellTypes
  {
    ctUnassigned    = INT_MIN,
    ctFar           = INT_MIN + 1,
    ctInternal      = INT_MIN + 2,
    ctSurface       = INT_MIN + 3,
    ctFeatureEdge   = INT_MIN + 4,
    ctFeaturePoint  = INT_MIN + 5
  };
  static const mousse::NamedEnum<cellTypes, 6> cellTypesNames_;
};
template<>
inline bool contiguous<indexedCellEnum>()
{
  return true;
}
}  // namespace mousse
#endif