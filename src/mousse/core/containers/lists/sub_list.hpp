#ifndef CORE_CONTAINERS_LISTS_SUB_LIST_HPP_
#define CORE_CONTAINERS_LISTS_SUB_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SubList
// Description
//   A List obtained as a section of another List.
//   Since the SubList is itself unallocated, no storage is allocated or
//   de-allocated during its use.  To achieve this behaviour, SubList is
//   derived from UList rather than List.

#include "list.hpp"


namespace mousse {

template<class T>
class SubList
:
  public UList<T>
{
public:
  // Static Member Functions
    //- Return a null SubList
    inline static const SubList<T>& null();
  // Constructors
    //- Construct from UList and sub-list size
    inline SubList
    (
      const UList<T>& list,
      const label subSize
    );
    //- Construct from UList, sub-list size and start index
    inline SubList
    (
      const UList<T>& list,
      const label subSize,
      const label startIndex
    );
  // Member operators
    //- Allow cast to a const List<T>&
    inline operator const mousse::List<T>&() const;
    //- Assignment of all entries to the given value
    inline void operator=(const T&);
};

}  // namespace mousse


// Constructors 
template<class T>
inline mousse::SubList<T>::SubList
(
  const UList<T>& list,
  const label subSize
)
:
  UList<T>{list.v_, subSize}
{
#ifdef FULLDEBUG
  list.checkSize(subSize);
#endif
}


template<class T>
inline mousse::SubList<T>::SubList
(
  const UList<T>& list,
  const label subSize,
  const label startIndex
)
:
  UList<T>{&(list.v_[startIndex]), subSize}
{
#ifdef FULLDEBUG
  // Artificially allow the start of a zero-sized subList to be
  // one past the end of the original list.
  if (subSize) {
    list.checkStart(startIndex);
    list.checkSize(startIndex + subSize);
  } else {
    // Start index needs to fall between 0 and size.  One position
    // behind the last element is allowed
    list.checkSize(startIndex);
  }
#endif
}


// Member Functions 
template<class T>
inline const mousse::SubList<T>& mousse::SubList<T>::null()
{
  return NullObjectRef<SubList<T> >();
}


// Member Operators 
template<class T>
inline mousse::SubList<T>::operator const mousse::List<T>&() const
{
  return *reinterpret_cast< const List<T>* >(this);
}


template<class T>
inline void mousse::SubList<T>::operator=(const T& t)
{
  UList<T>::operator=(t);
}

#endif
