// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bin_sum.hpp"


// Constructors 
template<class IndexType, class List, class CombineOp>
mousse::BinSum<IndexType, List, CombineOp>::BinSum
(
  const IndexType min,
  const IndexType max,
  const IndexType delta
)
:
  List{ceil((max-min)/delta), pTraits<typename List::value_type>::zero},
  min_{min},
  max_{max},
  delta_{delta},
  lowSum_{pTraits<typename List::value_type>::zero},
  highSum_{pTraits<typename List::value_type>::zero}
{}


template<class IndexType, class List, class CombineOp>
mousse::BinSum<IndexType, List, CombineOp>::BinSum
(
  const IndexType min,
  const IndexType max,
  const IndexType delta,
  const UList<IndexType>& indexVals,
  const List& vals,
  const CombineOp& cop
)
:
  List{ceil((max-min)/delta), pTraits<typename List::value_type>::zero},
  min_{min},
  max_{max},
  delta_{delta},
  lowSum_{pTraits<typename List::value_type>::zero},
  highSum_{pTraits<typename List::value_type>::zero}
{
  FOR_ALL(indexVals, i)
  {
    add(indexVals[i], vals[i], cop);
  }
}


// Member Functions 
template<class IndexType, class List, class CombineOp>
void mousse::BinSum<IndexType, List, CombineOp>::add
(
  const IndexType& indexVal,
  const typename List::const_reference val,
  const CombineOp& cop
)
{
  if (indexVal < min_) {
    cop(lowSum_, val);
  } else if (indexVal >= max_) {
    cop(highSum_, val);
  } else {
    label index = (indexVal-min_)/delta_;
    cop(this->operator[](index), val);
  }
}


template<class IndexType, class List, class CombineOp>
void mousse::BinSum<IndexType, List, CombineOp>::add
(
  const UList<IndexType>& indexVals,
  const List& vals,
  const CombineOp& cop
)
{
  FOR_ALL(indexVals, i)
  {
    add(indexVals[i], vals[i], cop);
  }
}
