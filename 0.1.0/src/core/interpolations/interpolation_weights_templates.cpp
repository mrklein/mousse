// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation_weights.hpp"
#include "list_ops.hpp"
#include "ioobject.hpp"
#include "hash_set.hpp"
#include "object_registry.hpp"
namespace mousse
{
// Member Functions 
template<class ListType1, class ListType2>
typename mousse::outerProduct
<
  typename ListType1::value_type,
  typename ListType2::value_type
>::type
mousse::interpolationWeights::weightedSum
(
  const ListType1& f1,
  const ListType2& f2
)
{
  typedef typename outerProduct
  <
    typename ListType1::value_type,
    typename ListType2::value_type
  >::type returnType;
  if (f1.size())
  {
    returnType SumProd = f1[0]*f2[0];
    for (label i = 1; i < f1.size(); ++i)
    {
      SumProd += f1[i]*f2[i];
    }
    return SumProd;
  }
  else
  {
    return pTraits<returnType>::zero;
  }
}
}  // namespace mousse
