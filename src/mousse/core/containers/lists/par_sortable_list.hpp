#ifndef CORE_CONTAINERS_LISTS_PAR_SORTABLE_LIST_HPP_
#define CORE_CONTAINERS_LISTS_PAR_SORTABLE_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParSortableList
// Description
//   Implementation of PSRS parallel sorting routine.
//   From "On the Versatility of Parallel Sorting by Regular Sampling"
//   Xiaobo Li et. all.
//   Construct from list of things to sort (uses SortableList, 'thing' should
//   implement >, ==).
//   Will contain sorted data and in
//     - indices() the original indices
//     - procs() the original processor id.
//   Can also be constructed from size, filled at ease and then sort()'ed.

#include "label_list.hpp"
#include "class_name.hpp"


namespace mousse {

TEMPLATE_NAME(ParSortableList);


template<class Type>
class ParSortableList
:
  public ParSortableListName,
  public List<Type>
{
  // Private data

    //- Original indices
    labelList indices_;

    //- Processor numbers
    labelList procs_;

  // Private class

    //- Private class for sorting. Sorts on value_.
    class taggedValue
    {
      // Private data

        //- Value
        Type value_;

        //- Index
        label index_;

        //- Proc
        label procID_;

    public:
      // Constructors
        taggedValue()
        {}

      // Member Functions
        const Type& value() const
        {
          return value_;
        }

        Type& value()
        {
          return value_;
        }

        label index() const
        {
          return index_;
        }

        label& index()
        {
          return index_;
        }

        label procID() const
        {
          return procID_;
        }

        label& procID()
        {
          return procID_;
        }

    };

  // Private Member Functions

    //- Write for debugging
    void write(const List<Type>& elems, Ostream& os) const;

    //- Copy into dest, setting indices and processor no.
    void copyInto
    (
      const List<Type>& values,
      const labelList& indices,
      const label fromProcNo,
      label& destI,
      List<taggedValue>& dest
    ) const;

    //- Calculate pivots.
    void getPivots(const List<Type>& elems, List<Type>& pivots) const;

    //- If destProc != myProcNo send & clear values and indices
    void checkAndSend
    (
      List<Type>& values,
      labelList& indices,
      const label bufSize,
      const label destProcI
    ) const;

public:

  // Constructors

    //- Construct from List, sorting the elements.
    ParSortableList(const UList<Type>&);

    //- Construct given size. Sort later on.
    ParSortableList(const label size);

  // Member Functions

    //- (stable) sort the list (if changed after construction time)
    void sort();

    //- Return the list of sorted point indices
    const labelList& indices() const
    {
      return indices_;
    }

    //- Return the list of processor number
    const labelList& procs() const
    {
      return procs_;
    }
};

}  // namespace mousse

#include "par_sortable_list.ipp"

#endif
