// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UList
// Description
//   A 1D vector of objects of type \<T\>, where the size of the vector is
//   known and can be used for subscript bounds checking, etc.
//   Storage is not allocated during construction or use but is supplied to
//   the constructor as an argument.  This type of list is particularly useful
//   for lists that refer to parts of existing lists such as SubList.
// SourceFiles
//   ulist.cpp
//   ulist_io.cpp

#ifndef ulist_hpp_
#define ulist_hpp_

#include "bool.hpp"
#include "label.hpp"
#include "ulabel.hpp"
#include "null_object.hpp"
#include "error.hpp"
#include "ptraits.hpp"
#include "swap.hpp"

namespace mousse
{

// Forward declaration of friend classes
template<class T> class List;
template<class T> class SubList;

// Forward declaration of friend functions and operators
template<class T> class UList;
template<class T> Ostream& operator<<(Ostream&, const UList<T>&);
template<class T> Istream& operator>>(Istream&, UList<T>&);
typedef UList<label> labelUList;

template<class T>
class UList
{
  // Private data

    //- Number of elements in UList.
    label size_;

    //- Vector of values of type T.
    T* __restrict__ v_;

public:

  // Related types

    //- Declare friendship with the List class
    friend class List<T>;

    //- Declare friendship with the SubList class
    friend class SubList<T>;

  // Static Member Functions

    //- Return a null UList
    inline static const UList<T>& null();

  // Public classes

    //- Less function class that can be used for sorting
    class less
    {
      const UList<T>& values_;
    public:
      less(const UList<T>& values)
      :
        values_{values}
      {}

      bool operator()(const label a, const label b)
      {
        return values_[a] < values_[b];
      }
    };

    //- Greater function class that can be used for sorting
    class greater
    {
      const UList<T>& values_;
    public:
      greater(const UList<T>& values)
      :
        values_{values}
      {}

      bool operator()(const label a, const label b)
      {
        return values_[a] > values_[b];
      }
    };

  // Constructors
    //- Null constructor.
    inline UList();

    //- Construct from components
    inline UList(T* __restrict__ v, label size);

  // Member Functions

    // Access

      //- Return the forward circular index, i.e. the next index
      //  which returns to the first at the end of the list
      inline label fcIndex(const label i) const;

      //- Return the reverse circular index, i.e. the previous index
      //  which returns to the last at the beginning of the list
      inline label rcIndex(const label i) const;

      //- Return the binary size in number of characters of the UList
      //  if the element is a primitive type
      //  i.e. contiguous<T>() == true.
      //  Note that is of type streamsize since used in stream ops
      std::streamsize byteSize() const;

      //- Return a const pointer to the first data element,
      //  similar to the STL front() method and the string::data() method
      //  This can be used (with caution) when interfacing with C code.
      inline const T* cdata() const;

      //- Return a pointer to the first data element,
      //  similar to the STL front() method and the string::data() method
      //  This can be used (with caution) when interfacing with C code.
      inline T* data();

      //- Return the first element of the list.
      inline T& first();

      //- Return first element of the list.
      inline const T& first() const;

      //- Return the last element of the list.
      inline T& last();

      //- Return the last element of the list.
      inline const T& last() const;

    // Check

      //- Check start is within valid range (0 ... size-1).
      inline void checkStart(const label start) const;

      //- Check size is within valid range (0 ... size).
      inline void checkSize(const label size) const;

      //- Check index i is within valid range (0 ... size-1).
      inline void checkIndex(const label i) const;

    //- Write the UList as a dictionary entry.
    void writeEntry(Ostream&) const;

    //- Write the UList as a dictionary entry with keyword.
    void writeEntry(const word& keyword, Ostream&) const;

    //- Assign elements to those from UList.
    void assign(const UList<T>&);

  // Member operators

    //- Return element of UList.
    inline T& operator[](const label);

    //- Return element of constant UList.
    //  Note that the bool specialization adds lazy evaluation so reading
    //  an out-of-range element returns false without any ill-effects
    inline const T& operator[](const label) const;

    //- Allow cast to a const List<T>&
    inline operator const mousse::List<T>&() const;

    //- Assignment of all entries to the given value
    void operator=(const T&);

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

  // STL iterator

    //- Random access iterator for traversing UList.
    typedef T* iterator;

    //- Return an iterator to begin traversing the UList.
    inline iterator begin();

    //- Return an iterator to end traversing the UList.
    inline iterator end();

  // STL const_iterator

    //- Random access iterator for traversing UList.
    typedef const T* const_iterator;

    //- Return const_iterator to begin traversing the constant UList.
    inline const_iterator cbegin() const;

    //- Return const_iterator to end traversing the constant UList.
    inline const_iterator cend() const;

    //- Return const_iterator to begin traversing the constant UList.
    inline const_iterator begin() const;

    //- Return const_iterator to end traversing the constant UList.
    inline const_iterator end() const;

  // STL reverse_iterator

    //- Reverse iterator for reverse traversal of UList.
    typedef T* reverse_iterator;

    //- Return reverse_iterator to begin reverse traversing the UList.
    inline reverse_iterator rbegin();

    //- Return reverse_iterator to end reverse traversing the UList.
    inline reverse_iterator rend();

  // STL const_reverse_iterator

    //- Reverse iterator for reverse traversal of constant UList.
    typedef const T* const_reverse_iterator;

    //- Return const_reverse_iterator to begin reverse traversing the UList.
    inline const_reverse_iterator crbegin() const;

    //- Return const_reverse_iterator to end reverse traversing the UList.
    inline const_reverse_iterator crend() const;

    //- Return const_reverse_iterator to begin reverse traversing the UList.
    inline const_reverse_iterator rbegin() const;

    //- Return const_reverse_iterator to end reverse traversing the UList.
    inline const_reverse_iterator rend() const;

  // STL member functions

    //- Return the number of elements in the UList.
    inline label size() const;

    //- Return size of the largest possible UList.
    inline label max_size() const;

    //- Return true if the UList is empty (ie, size() is zero).
    inline bool empty() const;

    //- Swap two ULists of the same type in constant time.
    void swap(UList<T>&);

  // STL member operators

    //- Equality operation on ULists of the same type.
    //  Returns true when the ULists are elementwise equal
    //  (using UList::value_type::operator==).  Takes linear time.
    bool operator==(const UList<T>&) const;

    //- The opposite of the equality operation. Takes linear time.
    bool operator!=(const UList<T>&) const;

    //- Compare two ULists lexicographically. Takes linear time.
    bool operator<(const UList<T>&) const;

    //- Compare two ULists lexicographically. Takes linear time.
    bool operator>(const UList<T>&) const;

    //- Return true if !(a > b). Takes linear time.
    bool operator<=(const UList<T>&) const;

    //- Return true if !(a < b). Takes linear time.
    bool operator>=(const UList<T>&) const;

  // Ostream operator

    // Write UList to Ostream.
    friend Ostream& operator<< <T>
    (
      Ostream&,
      const UList<T>&
    );

    //- Read UList contents from Istream. Requires size to have been set
    //  before.
    friend Istream& operator>> <T>
    (
      Istream&,
      UList<T>&
    );
};

template<class T>
void sort(UList<T>&);

template<class T, class Cmp>
void sort(UList<T>&, const Cmp&);

template<class T>
void stableSort(UList<T>&);

template<class T, class Cmp>
void stableSort(UList<T>&, const Cmp&);

template<class T>
void shuffle(UList<T>&);

// Reverse the first n elements of the list
template<class T>
inline void reverse(UList<T>&, const label n);

// Reverse all the elements of the list
template<class T>
inline void reverse(UList<T>&);

}  // namespace mousse

// Constructors 
template<class T>
inline mousse::UList<T>::UList()
:
  size_{0},
  v_{0}
{}


template<class T>
inline mousse::UList<T>::UList(T* __restrict__ v, label size)
:
  size_{size},
  v_{v}
{}


// Member Functions 
template<class T>
inline const mousse::UList<T>& mousse::UList<T>::null()
{
  return NullObjectRef<UList<T>>();
}


template<class T>
inline mousse::label mousse::UList<T>::fcIndex(const label i) const
{
  return (i == size()-1 ? 0 : i+1);
}


template<class T>
inline mousse::label mousse::UList<T>::rcIndex(const label i) const
{
  return (i ? i-1 : size()-1);
}


// Check start is within valid range (0 ... size-1).
template<class T>
inline void mousse::UList<T>::checkStart(const label start) const
{
  if (start<0 || (start && start>=size_))
  {
    FATAL_ERROR_IN("UList<T>::checkStart(const label)")
      << "start " << start << " out of range 0 ... " << max(size_-1, 0)
      << abort(FatalError);
  }
}


// Check size is within valid range (0 ... size).
template<class T>
inline void mousse::UList<T>::checkSize(const label size) const
{
  if (size<0 || size>size_)
  {
    FATAL_ERROR_IN("UList<T>::checkSize(const label)")
      << "size " << size << " out of range 0 ... " << size_
      << abort(FatalError);
  }
}


// Check index i is within valid range (0 ... size-1).
template<class T>
inline void mousse::UList<T>::checkIndex(const label i) const
{
  if (!size_)
  {
    FATAL_ERROR_IN("UList<T>::checkIndex(const label)")
      << "attempt to access element from zero sized list"
      << abort(FatalError);
  }
  else if (i<0 || i>=size_)
  {
    FATAL_ERROR_IN("UList<T>::checkIndex(const label)")
      << "index " << i << " out of range 0 ... " << size_-1
      << abort(FatalError);
  }
}


template<class T>
inline T& mousse::UList<T>::first()
{
  return this->operator[](0);
}


template<class T>
inline const T& mousse::UList<T>::first() const
{
  return this->operator[](0);
}


template<class T>
inline T& mousse::UList<T>::last()
{
  return this->operator[](this->size()-1);
}


template<class T>
inline const T& mousse::UList<T>::last() const
{
  return this->operator[](this->size()-1);
}


template<class T>
inline const T* mousse::UList<T>::cdata() const
{
  return v_;
}


template<class T>
inline T* mousse::UList<T>::data()
{
  return v_;
}


// Member Operators 
// element access
template<class T>
inline T& mousse::UList<T>::operator[](const label i)
{
#ifdef FULLDEBUG
  checkIndex(i);
#endif
  return v_[i];
}


namespace mousse
{

// Template specialization for bool
template<>
inline const bool& mousse::UList<bool>::operator[](const label i) const
{
  // lazy evaluation - return false for out-of-range
  if (i < size_)
  {
    return v_[i];
  }
  else
  {
    return mousse::pTraits<bool>::zero;
  }
}

} // end of namespace mousse

// const element access
template<class T>
inline const T& mousse::UList<T>::operator[](const label i) const
{
#ifdef FULLDEBUG
  checkIndex(i);
#endif
  return v_[i];
}


// Allow cast to a const List<T>&
template<class T>
inline mousse::UList<T>::operator const mousse::List<T>&() const
{
  return *reinterpret_cast<const List<T>*>(this);
}


// STL Member Functions 
template<class T>
inline typename mousse::UList<T>::iterator
mousse::UList<T>::begin()
{
  return v_;
}


template<class T>
inline typename mousse::UList<T>::const_iterator
mousse::UList<T>::begin() const
{
  return v_;
}


template<class T>
inline typename mousse::UList<T>::const_iterator
mousse::UList<T>::cbegin() const
{
  return v_;
}


template<class T>
inline typename mousse::UList<T>::iterator
mousse::UList<T>::end()
{
  return &v_[size_];
}


template<class T>
inline typename mousse::UList<T>::const_iterator
mousse::UList<T>::end() const
{
  return &v_[size_];
}


template<class T>
inline typename mousse::UList<T>::const_iterator
mousse::UList<T>::cend() const
{
  return &v_[size_];
}


template<class T>
inline typename mousse::UList<T>::iterator
mousse::UList<T>::rbegin()
{
  return &v_[size_-1];
}


template<class T>
inline typename mousse::UList<T>::const_iterator
mousse::UList<T>::rbegin() const
{
  return &v_[size_-1];
}


template<class T>
inline typename mousse::UList<T>::const_iterator
mousse::UList<T>::crbegin() const
{
  return &v_[size_-1];
}


template<class T>
inline typename mousse::UList<T>::iterator
mousse::UList<T>::rend()
{
  return &v_[-1];
}


template<class T>
inline typename mousse::UList<T>::const_iterator
mousse::UList<T>::rend() const
{
  return &v_[-1];
}


template<class T>
inline typename mousse::UList<T>::const_iterator
mousse::UList<T>::crend() const
{
  return &v_[-1];
}


template<class T>
inline mousse::label mousse::UList<T>::size() const
{
  return size_;
}


template<class T>
inline mousse::label mousse::UList<T>::max_size() const
{
  return labelMax;
}


template<class T>
inline bool mousse::UList<T>::empty() const
{
  return !size_;
}


// Global Functions 
template<class T>
inline void mousse::reverse(UList<T>& ul, const label n)
{
  for (int i=0; i<n/2; i++)
  {
    Swap(ul[i], ul[n-1-i]);
  }
}


template<class T>
inline void mousse::reverse(UList<T>& ul)
{
  reverse(ul, ul.size());
}

/**
* \def FOR_ALL(list, i)
* Loop across all elements in \a list
* \par Usage
* \code
* FOR_ALL(anyList, i)
* {
*      statements;
* }
* \endcode
* \sa FOR_ALL_REVERSE
*/
/**
* \def FOR_ALL_REVERSE(list, i)
* Reverse loop across all elements in \a list
* \par Usage
* \code
* FOR_ALL_REVERSE(anyList, i)
* {
*      statements;
* }
* \endcode
* \sa FOR_ALL
*/
#define FOR_ALL(list, i)                                                      \
  for (mousse::label i=0; i<(list).size(); i++)

#define FOR_ALL_REVERSE(list, i)                                              \
  for (mousse::label i=(list).size()-1; i>=0; i--)

/**
* \def FOR_ALL_ITER(Container, container, iter)
* Iterate across all elements in the \a container object of type
* \a Container.
* \par Usage
* \code
* FOR_ALL(ContainerType, container, iter)
* {
*     statements;
* }
* \endcode
* \sa FOR_ALL_CONST_ITER
*/
#define FOR_ALL_ITER(Container,container,iter)                                \
  for                                                                         \
  (                                                                           \
    Container::iterator iter = (container).begin();                           \
    iter != (container).end();                                                \
    ++iter                                                                    \
  )

/**
* \def FOR_ALL_CONST_ITER(Container, container, iter)
* Iterate across all elements in the \a container object of type
* \a Container with const access.
* \par Usage
* \code
* FOR_ALL_CONST_ITER(ContainerType, container, iter)
* {
*     statements;
* }
* \endcode
* \sa FOR_ALL_ITER
*/
#define FOR_ALL_CONST_ITER(Container,container,iter)                          \
  for                                                                         \
  (                                                                           \
    Container::const_iterator iter = (container).begin();                     \
    iter != (container).end();                                                \
    ++iter                                                                    \
  )

#ifdef NoRepository
#   include "ulist.cpp"
#endif
#endif
