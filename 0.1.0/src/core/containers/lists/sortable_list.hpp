#ifndef CORE_CONTAINERS_LISTS_SORTABLE_LIST_HPP_
#define CORE_CONTAINERS_LISTS_SORTABLE_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SortableList
// Description
//   A list that is sorted upon construction or when explicitly requested
//   with the sort() method.
//   Uses the mousse::stableSort() algorithm.
// SourceFiles
//   sortable_list.cpp
#include "label_list.hpp"
namespace mousse
{
template<class T>
class SortableList
:
  public List<T>
{
  // Private data
    //- Original indices
    labelList indices_;
public:
  // Constructors
    //- Null constructor, sort later (eg, after assignment or transfer)
    SortableList();
    //- Construct from UList, sorting immediately.
    explicit SortableList(const UList<T>&);
    //- Construct from transferred List, sorting immediately.
    explicit SortableList(const Xfer<List<T> >&);
    //- Construct given size. Sort later on.
    //  The indices remain empty until the list is sorted
    explicit SortableList(const label size);
    //- Construct given size and initial value. Sort later on.
    //  The indices remain empty until the list is sorted
    SortableList(const label size, const T&);
    //- Construct as copy.
    SortableList(const SortableList<T>&);
  // Member Functions
    //- Return the list of sorted indices. Updated every sort.
    const labelList& indices() const
    {
      return indices_;
    }
    //- Return non-const access to the sorted indices. Updated every sort.
    labelList& indices()
    {
      return indices_;
    }
    //- Clear the list and the indices
    void clear();
    //- Clear the indices and return a reference to the underlying List
    List<T>& shrink();
    //- (stable) sort the list (if changed after construction time)
    //  also resizes the indices as required
    void sort();
    //- Reverse (stable) sort the list
    void reverseSort();
    //- Transfer contents to the Xfer container as a plain List
    inline Xfer<List<T> > xfer();
  // Member Operators
    //- Assignment of all entries to the given value
    inline void operator=(const T&);
    //- Assignment from UList operator. Takes linear time.
    inline void operator=(const UList<T>&);
    //- Assignment operator. Takes linear time.
    inline void operator=(const SortableList<T>&);
};
}  // namespace mousse
#ifdef NoRepository
#   include "sortable_list.cpp"
#endif
#endif
