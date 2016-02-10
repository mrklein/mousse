// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "channel_index.hpp"
// Member Functions 
template<class T>
mousse::Field<T> mousse::channelIndex::regionSum(const Field<T>& cellField) const
{
  Field<T> regionField{cellRegion_().nRegions(), pTraits<T>::zero};
  FOR_ALL(cellRegion_(), cellI)
  {
    regionField[cellRegion_()[cellI]] += cellField[cellI];
  }
  // Global sum
  Pstream::listCombineGather(regionField, plusEqOp<T>());
  Pstream::listCombineScatter(regionField);
  return regionField;
}
template<class T>
mousse::Field<T> mousse::channelIndex::collapse
(
  const Field<T>& cellField,
  const bool asymmetric
) const
{
  // Average and order
  const Field<T> summedField{regionSum(cellField)};
  Field<T> regionField
  {
    summedField/regionCount_,
    sortMap_
  };
  // Symmetry?
  if (symmetric_)
  {
    label nlb2 = cellRegion_().nRegions()/2;
    if (asymmetric)
    {
      for (label j=0; j<nlb2; j++)
      {
        regionField[j] = 0.5*(regionField[j]
                              - regionField[cellRegion_().nRegions() - j - 1]);
      }
    }
    else
    {
      for (label j=0; j<nlb2; j++)
      {
        regionField[j] = 0.5*(regionField[j]
                              + regionField[cellRegion_().nRegions() - j - 1]);
      }
    }
    regionField.setSize(nlb2);
  }
  return regionField;
}
