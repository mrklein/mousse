// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_primitive_mesh.hpp"

// Private Member Functions 
template<class ProcPatch>
mousse::lduSchedule mousse::lduPrimitiveMesh::nonBlockingSchedule
(
  const lduInterfacePtrsList& interfaces
)
{
  lduSchedule schedule(2*interfaces.size());
  label slotI = 0;
  FOR_ALL(interfaces, i)
  {
    if (interfaces.set(i) && !isA<ProcPatch>(interfaces[i]))
    {
      schedule[slotI].patch = i;
      schedule[slotI].init = true;
      slotI++;
      schedule[slotI].patch = i;
      schedule[slotI].init = false;
      slotI++;
    }
  }
  FOR_ALL(interfaces, i)
  {
    if (interfaces.set(i) && isA<ProcPatch>(interfaces[i]))
    {
      schedule[slotI].patch = i;
      schedule[slotI].init = true;
      slotI++;
    }
  }
  FOR_ALL(interfaces, i)
  {
    if (interfaces.set(i) && isA<ProcPatch>(interfaces[i]))
    {
      schedule[slotI].patch = i;
      schedule[slotI].init = false;
      slotI++;
    }
  }
  return schedule;
}
