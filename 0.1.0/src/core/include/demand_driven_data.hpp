// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef demand_driven_data_hpp_
#define demand_driven_data_hpp_
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
