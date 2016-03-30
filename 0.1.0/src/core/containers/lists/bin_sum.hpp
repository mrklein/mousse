#ifndef CORE_CONTAINERS_LISTS_BIN_SUM_HPP_
#define CORE_CONTAINERS_LISTS_BIN_SUM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BinSum
// Description
//   Sums into bins

#include "ops.hpp"


namespace mousse {

template
<
  class IndexType,
  class List,
  class CombineOp = plusEqOp<typename List::value_type>
>
class BinSum
:
  public List
{
  // Private data
    const IndexType min_;

    const IndexType max_;

    const IndexType delta_;

    //- Sum < lowest bin
    typename List::value_type lowSum_;

    //- Sum of >= highest bin
    typename List::value_type highSum_;

public:
  // Constructors

    //- Construct given min, max, delta
    BinSum
    (
      const IndexType min,
      const IndexType max,
      const IndexType delta
    );

    //- Construct given min, max, delta and data
    BinSum
    (
      const IndexType min,
      const IndexType max,
      const IndexType delta,
      const UList<IndexType>& indexVals,
      const List& vals,
      const CombineOp& cop = plusEqOp<typename List::value_type>()
    );

    // Access

      //- Return the delta
      inline IndexType delta() const
      {
        return delta_;
      }

      //- Return the sum of all added elements < min
      inline const IndexType& lowSum() const
      {
        return lowSum_;
      }

      //- Return the sum of all added elements >= max
      inline const IndexType& highSum() const
      {
        return highSum_;
      }

      void add
      (
        const IndexType& indexVal,
        const typename List::const_reference val,
        const CombineOp& cop = plusEqOp<typename List::value_type>()
      );

      void add
      (
        const UList<IndexType>& indexVals,
        const List& vals,
        const CombineOp& cop = plusEqOp<typename List::value_type>()
      );

};
}  // namespace mousse

#include "bin_sum.ipp"

#endif
