// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "indexed_cell_ops.hpp"
#include "pstream.hpp"
// Global Functions 
template<typename CellType>
mousse::label CGAL::indexedCellOps::dualVertexMasterProc(const CellType& c)
{
  if (!c->parallelDualVertex())
  {
    return -1;
  }
  // The master processor is the lowest numbered of the four on this tet.
  int masterProc = mousse::Pstream::nProcs() + 1;
  for (mousse::label vI = 0; vI < 4; ++vI)
  {
    if (c->vertex(vI)->referred())
    {
      masterProc = min(masterProc, c->vertex(vI)->procIndex());
    }
    else
    {
      masterProc = min(masterProc, mousse::Pstream::myProcNo());
    }
  }
  return masterProc;
}
template<typename CellType>
mousse::FixedList<mousse::label, 4>
CGAL::indexedCellOps::processorsAttached(const CellType& c)
{
  mousse::FixedList<mousse::label, 4> procsAttached(mousse::Pstream::myProcNo());
  if (!c->parallelDualVertex())
  {
    return procsAttached;
  }
  for (mousse::label vI = 0; vI < 4; ++vI)
  {
    if (c->vertex(vI)->referred())
    {
      procsAttached[vI] = c->vertex(vI)->procIndex();
    }
  }
  return procsAttached;
}
