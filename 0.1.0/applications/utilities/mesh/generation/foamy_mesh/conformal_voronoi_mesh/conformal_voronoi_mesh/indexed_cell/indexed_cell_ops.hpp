// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   indexed_cell_ops_templates.cpp
#ifndef indexed_cell_ops_hpp_
#define indexed_cell_ops_hpp_
#include "label.hpp"
#include "fixed_list.hpp"
namespace CGAL
{
namespace indexedCellOps
{
//- Does the Dual vertex form part of a processor patch
template<typename CellType>
mousse::label dualVertexMasterProc(const CellType& c);
template<typename CellType>
mousse::FixedList<mousse::label, 4> processorsAttached(const CellType& c);
}  // namespace indexedCellOps
}  // namespace CGAL
#ifdef NoRepository
  #include "indexed_cell_ops_templates.cpp"
#endif
#endif
