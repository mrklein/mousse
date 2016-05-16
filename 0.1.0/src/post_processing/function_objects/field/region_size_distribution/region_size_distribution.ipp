// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_size_distribution.hpp"
#include "region_split.hpp"
#include "vol_fields.hpp"


// Member Functions 
template<class Type>
mousse::Map<Type> mousse::regionSizeDistribution::regionSum
(
  const regionSplit& regions,
  const Field<Type>& fld
) const
{
  // Per region the sum of fld
  Map<Type> regionToSum{regions.nRegions()/Pstream::nProcs()};
  FOR_ALL(fld, cellI) {
    label regionI = regions[cellI];
    typename Map<Type>::iterator fnd = regionToSum.find(regionI);
    if (fnd == regionToSum.end()) {
      regionToSum.insert(regionI, fld[cellI]);
    } else {
      fnd() += fld[cellI];
    }
  }
  Pstream::mapCombineGather(regionToSum, plusEqOp<Type>());
  Pstream::mapCombineScatter(regionToSum);
  return regionToSum;
}


// Get data in sortedToc order
template<class Type>
mousse::List<Type> mousse::regionSizeDistribution::extractData
(
  const UList<label>& keys,
  const Map<Type>& regionData
) const
{
  List<Type> sortedData{keys.size()};
  FOR_ALL(keys, i) {
    sortedData[i] = regionData[keys[i]];
  }
  return sortedData;
}

