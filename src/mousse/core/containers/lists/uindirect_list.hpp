#ifndef CORE_CONTAINERS_LISTS_UINDIRECT_LIST_HPP_
#define CORE_CONTAINERS_LISTS_UINDIRECT_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UIndirectList
// Description
//   A List with indirect addressing.
//   Like IndirectList but does not store addressing.
//   Note the const_cast of the completeList. This is so we can use it both
//   on const and non-const lists. Alternative would be to have a const_
//   variant etc.

#include "list.hpp"

namespace mousse {

// Forward declaration of friend functions and operators
template<class T> class UIndirectList;
template<class T> Ostream& operator<<(Ostream&, const UIndirectList<T>&);

template<class T>
class UIndirectList
{
  // Private data
    UList<T>& completeList_;
    const labelUList& addressing_;

public:

  // Constructors

    //- Construct given the complete list and the addressing array
    inline UIndirectList(const UList<T>&, const labelUList&);

  // Member Functions

    // Access

      //- Return the number of elements in the list
      inline label size() const;

      //- Return true if the list is empty (ie, size() is zero).
      inline bool empty() const;

      //- Return the first element of the list.
      inline T& first();

      //- Return first element of the list.
      inline const T& first() const;

      //- Return the last element of the list.
      inline T& last();

      //- Return the last element of the list.
      inline const T& last() const;

      //- Return the complete list
      inline const UList<T>& completeList() const;

      //- Return the list addressing
      inline const List<label>& addressing() const;

    // Member Operators

      //- Return the addressed elements as a List
      inline List<T> operator()() const;

      //- Return non-const access to an element
      inline T& operator[](const label);

      //- Return const access to an element
      inline const T& operator[](const label) const;

      //- Assignment from UList of addressed elements
      inline void operator=(const UList<T>&);

      //- Assignment from UIndirectList of addressed elements
      inline void operator=(const UIndirectList<T>&);

      //- Assignment of all entries to the given value
      inline void operator=(const T&);

  // STL type definitions

    //- Type of values the UList contains.
    typedef T value_type;

    //- Type that can be used for storing into
    //  UList::value_type objects.
    typedef T& reference;

    //- Type that can be used for storing into
    //  constant UList::value_type objects
    typedef const T& const_reference;

    //- The type that can represent the difference between any two
    //  UList iterator objects.
    typedef label difference_type;

    //- The type that can represent the size of a UList.
    typedef label size_type;

  // Ostream operator

    //- Write UIndirectList to Ostream
    //  Binary output is currently still a bit of a problem
    friend Ostream& operator<< <T>
    (
      Ostream&,
      const UIndirectList<T>&
    );

};

}  // namespace mousse


// Constructors
template<class T>
inline mousse::UIndirectList<T>::UIndirectList
(
  const UList<T>& completeList,
  const labelUList& addr
)
:
  completeList_{const_cast<UList<T>&>(completeList)},
  addressing_{addr}
{}


// Member Functions
template<class T>
inline mousse::label mousse::UIndirectList<T>::size() const
{
  return addressing_.size();
}


template<class T>
inline bool mousse::UIndirectList<T>::empty() const
{
  return addressing_.empty();
}


template<class T>
inline T& mousse::UIndirectList<T>::first()
{
  return completeList_[addressing_.first()];
}


template<class T>
inline const T& mousse::UIndirectList<T>::first() const
{
  return completeList_[addressing_.first()];
}


template<class T>
inline T& mousse::UIndirectList<T>::last()
{
  return completeList_[addressing_.last()];
}


template<class T>
inline const T& mousse::UIndirectList<T>::last() const
{
  return completeList_[addressing_.last()];
}


template<class T>
inline const mousse::UList<T>& mousse::UIndirectList<T>::completeList() const
{
  return completeList_;
}


template<class T>
inline const mousse::List<mousse::label>& mousse::UIndirectList<T>::addressing() const
{
  return addressing_;
}


// Member Operators
template<class T>
inline mousse::List<T> mousse::UIndirectList<T>::operator()() const
{
  List<T> result(size());
  FOR_ALL(*this, i) {
    result[i] = operator[](i);
  }
  return result;
}


template<class T>
inline T& mousse::UIndirectList<T>::operator[](const label i)
{
  return completeList_[addressing_[i]];
}


template<class T>
inline const T& mousse::UIndirectList<T>::operator[](const label i) const
{
  return completeList_[addressing_[i]];
}


template<class T>
inline void mousse::UIndirectList<T>::operator=(const UList<T>& ae)
{
  if (addressing_.size() != ae.size()) {
    FATAL_ERROR_IN("UIndirectList<T>::operator=(const UList<T>&)")
      << "Addressing and list of addressed elements "
       "have different sizes: "
      << addressing_.size() << " " << ae.size()
      << abort(FatalError);
  }
  FOR_ALL(addressing_, i) {
    completeList_[addressing_[i]] = ae[i];
  }
}


template<class T>
inline void mousse::UIndirectList<T>::operator=(const UIndirectList<T>& ae)
{
  if (addressing_.size() != ae.size()) {
    FATAL_ERROR_IN("UIndirectList<T>::operator=(const UIndirectList<T>&)")
      << "Addressing and list of addressed elements "
         "have different sizes: "
      << addressing_.size() << " " << ae.size()
      << abort(FatalError);
  }
  FOR_ALL(addressing_, i) {
    completeList_[addressing_[i]] = ae[i];
  }
}


template<class T>
inline void mousse::UIndirectList<T>::operator=(const T& t)
{
  FOR_ALL(addressing_, i) {
    completeList_[addressing_[i]] = t;
  }
}

#include "uindirect_list_io.ipp"

#endif
