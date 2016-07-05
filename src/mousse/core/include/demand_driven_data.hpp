#ifndef CORE_INCLUDE_DEMAND_DRIVEN_DATA_HPP_
#define CORE_INCLUDE_DEMAND_DRIVEN_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


namespace mousse
{
template<class DataPtr>
inline void deleteDemandDrivenData(DataPtr& dataPtr)
{
  if (dataPtr)
  {
    delete dataPtr;
    dataPtr = 0;
  }
}
}  // namespace mousse

#endif
