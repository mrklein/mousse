// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UPtrList
// Description
//   A templated 1D list of pointers to objects of type \<T\>, where the
//   size of the array is known and used for subscript bounds checking, etc.
//   The element operator [] returns a reference to the object rather than a
//   pointer.  Storage is not allocated during construction or use but is
//   supplied to the constructor as an argument.
// SourceFiles
//   uptr_list.cpp
//   uptr_list_io.cpp

#ifndef uptr_list_hpp_
#define uptr_list_hpp_

#include "list.hpp"
#include "error.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
template<class T> class UPtrList;
template<class T>
inline typename UPtrList<T>::iterator operator+
(
  const typename UPtrList<T>::iterator&,
  label
);
template<class T>
inline typename UPtrList<T>::iterator operator+
(
  label,
  const typename UPtrList<T>::iterator&
);
template<class T>
inline typename UPtrList<T>::iterator operator-
(
  const typename UPtrList<T>::iterator&,
  label
);
template<class T>
inline label operator-
(
  const typename UPtrList<T>::iterator&,
  const typename UPtrList<T>::iterator&
);
template<class T>
Istream& operator>>(Istream&, UPtrList<T>&);
template<class T>
Ostream& operator<<(Ostream&, const UPtrList<T>&);

template<class T>
class UPtrList
{
  // Private data
    List<T*> ptrs_;

public:

  // Constructors

    //- Null Constructor.
    UPtrList();

    //- Construct with size specified.
    explicit UPtrList(const label);

    //- Construct by transferring the parameter contents
    UPtrList(const Xfer<UPtrList<T> >&);

    //- Construct as copy or re-use as specified.
    UPtrList(UPtrList<T>&, bool reUse);

  // Member functions

    // Access

      //- Return the number of elements in the UPtrList
      inline label size() const;

      //- Return true if the UPtrList is empty (ie, size() is zero).
      inline bool empty() const;

      //- Return reference to the first element of the list.
      inline T& first();

      //- Return reference to first element of the list.
      inline const T& first() const;

      //- Return reference to the last element of the list.
      inline T& last();

      //- Return reference to the last element of the list.
      inline const T& last() const;

    // Edit

      //- Reset size of UPtrList.  This can only be used to set the size
      //  of an empty UPtrList, extend a UPtrList, remove entries from
      //  the end of a UPtrList.
      void setSize(const label);

      //- Reset size of UPtrList.  This can only be used to set the size
      //  of an empty UPtrList, extend a UPtrList, remove entries from
      //  the end of a UPtrList.
      inline void resize(const label);

      //- Clear the UPtrList, i.e. set size to zero
      void clear();

      //- Transfer the contents of the argument UPtrList into this
      //  UPtrList and annul the argument list.
      void transfer(UPtrList<T>&);

      //- Transfer contents to the Xfer container
      inline Xfer<UPtrList<T> > xfer();

      //- Is element set
      inline bool set(const label) const;

      //- Set element. Return old element (can be NULL).
      //  No checks on new element.
      inline T* set(const label, T*);

      //- Reorders elements. Ordering does not have to be done in
      //  ascending or descending order. Reordering has to be unique.
      //  (is shuffle)
      void reorder(const labelUList&);

  // Member operators

    //- Return element const reference.
    inline const T& operator[](const label) const;

    //- Return element reference.
    inline T& operator[](const label);

    //- Return element const pointer.
    inline const T* operator()(const label) const;

  // STL type definitions

    //- Type of values the UPtrList contains.
    typedef T value_type;

    //- Type that can be used for storing into UPtrList::value_type objects.
    typedef T& reference;

    //- Type that can be used for storing into constant UPtrList::value_type
    //  objects.
    typedef const T& const_reference;

  // STL iterator
  // Random access iterator for traversing UPtrList.
    class iterator;
    friend class iterator;
    //- An STL iterator
    class iterator
    {
      T** ptr_;
    public:
      //- Construct for a given UPtrList entry
      inline iterator(T**);
      // Member operators
        inline bool operator==(const iterator&) const;
        inline bool operator!=(const iterator&) const;
        inline T& operator*();
        inline T& operator()();
        inline iterator operator++();
        inline iterator operator++(int);
        inline iterator operator--();
        inline iterator operator--(int);
        inline iterator operator+=(label);
        friend iterator operator+ <T>(const iterator&, label);
        friend iterator operator+ <T>(label, const iterator&);
        inline iterator operator-=(label);
        friend iterator operator- <T>(const iterator&, label);
        friend label operator- <T>
        (
          const iterator&,
          const iterator&
        );
        inline T& operator[](label);
        inline bool operator<(const iterator&) const;
        inline bool operator>(const iterator&) const;
        inline bool operator<=(const iterator&) const;
        inline bool operator>=(const iterator&) const;
    };
    //- Return an iterator to begin traversing the UPtrList.
    inline iterator begin();
    //- Return an iterator to end traversing the UPtrList.
    inline iterator end();

  // IOstream operator

    // Write List to Ostream.
    friend Ostream& operator<< <T>(Ostream&, const UPtrList<T>&);
};
}  // namespace mousse

// Member Functions 
template<class T>
inline mousse::label mousse::UPtrList<T>::size() const
{
  return ptrs_.size();
}


template<class T>
inline bool mousse::UPtrList<T>::empty() const
{
  return ptrs_.empty();
}


template<class T>
inline T& mousse::UPtrList<T>::first()
{
  return this->operator[](0);
}


template<class T>
inline const T& mousse::UPtrList<T>::first() const
{
  return this->operator[](0);
}


template<class T>
inline T& mousse::UPtrList<T>::last()
{
  return this->operator[](this->size()-1);
}


template<class T>
inline const T& mousse::UPtrList<T>::last() const
{
  return this->operator[](this->size()-1);
}


template<class T>
inline void mousse::UPtrList<T>::resize(const label newSize)
{
  this->setSize(newSize);
}


template<class T>
inline bool mousse::UPtrList<T>::set(const label i) const
{
  return ptrs_[i] != NULL;
}


template<class T>
inline T* mousse::UPtrList<T>::set(const label i, T* ptr)
{
  T* old = ptrs_[i];
  ptrs_[i] = ptr;
  return old;
}


template<class T>
inline mousse::Xfer<mousse::UPtrList<T> > mousse::UPtrList<T>::xfer()
{
  return xferMove(*this);
}


// Member Operators 
template<class T>
inline const T& mousse::UPtrList<T>::operator[](const label i) const
{
  if (!ptrs_[i])
  {
    FATAL_ERROR_IN("UPtrList::operator[] const")
      << "hanging pointer at index " << i
      << " (size " << size()
      << "), cannot dereference"
      << abort(FatalError);
  }
  return *(ptrs_[i]);
}


template<class T>
inline T& mousse::UPtrList<T>::operator[](const label i)
{
  if (!ptrs_[i])
  {
    FATAL_ERROR_IN("UPtrList::operator[]")
      << "hanging pointer at index " << i
      << " (size " << size()
      << "), cannot dereference"
      << abort(FatalError);
  }
  return *(ptrs_[i]);
}


template<class T>
inline const T* mousse::UPtrList<T>::operator()(const label i) const
{
  return ptrs_[i];
}


// STL iterator 
template<class T>
inline mousse::UPtrList<T>::iterator::iterator(T** ptr)
:
  ptr_{ptr}
{}


template<class T>
inline bool mousse::UPtrList<T>::iterator::operator==(const iterator& iter) const
{
  return ptr_ == iter.ptr_;
}


template<class T>
inline bool mousse::UPtrList<T>::iterator::operator!=(const iterator& iter) const
{
  return ptr_ != iter.ptr_;
}


template<class T>
inline T& mousse::UPtrList<T>::iterator::operator*()
{
  return **ptr_;
}


template<class T>
inline T& mousse::UPtrList<T>::iterator::operator()()
{
  return operator*();
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::UPtrList<T>::iterator::operator++()
{
  ++ptr_;
  return *this;
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::UPtrList<T>::iterator::operator++(int)
{
  iterator tmp = *this;
  ++ptr_;
  return tmp;
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::UPtrList<T>::iterator::operator--()
{
  --ptr_;
  return *this;
}
template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::UPtrList<T>::iterator::operator--(int)
{
  iterator tmp = *this;
  --ptr_;
  return tmp;
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::UPtrList<T>::iterator::operator+=(label n)
{
  ptr_ += n;
  return *this;
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::operator+(const typename UPtrList<T>::iterator& iter, label n)
{
  typename UPtrList<T>::iterator tmp = iter;
  return tmp += n;
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::operator+(label n, const typename UPtrList<T>::iterator& iter)
{
  typename UPtrList<T>::iterator tmp = iter;
  return tmp += n;
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::UPtrList<T>::iterator::operator-=(label n)
{
  ptr_ -= n;
  return *this;
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::operator-(const typename UPtrList<T>::iterator& iter, label n)
{
  typename UPtrList<T>::iterator tmp = iter;
  return tmp -= n;
}


template<class T>
inline mousse::label mousse::operator-
(
  const typename UPtrList<T>::iterator& iter1,
  const typename UPtrList<T>::iterator& iter2
)
{
  return (iter1.ptr_ - iter2.ptr_)/sizeof(T*);
}


template<class T>
inline T& mousse::UPtrList<T>::iterator::operator[](label n)
{
  return *(*this + n);
}


template<class T>
inline bool mousse::UPtrList<T>::iterator::operator<(const iterator& iter) const
{
  return ptr_ < iter.ptr_;
}


template<class T>
inline bool mousse::UPtrList<T>::iterator::operator>(const iterator& iter) const
{
  return ptr_ > iter.ptr_;
}


template<class T>
inline bool mousse::UPtrList<T>::iterator::operator<=(const iterator& iter) const
{
  return ptr_ <= iter.ptr_;
}


template<class T>
inline bool mousse::UPtrList<T>::iterator::operator>=(const iterator& iter) const
{
  return ptr_ >= iter.ptr_;
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::UPtrList<T>::begin()
{
  return ptrs_.begin();
}


template<class T>
inline typename mousse::UPtrList<T>::iterator
mousse::UPtrList<T>::end()
{
  return ptrs_.end();
}

#ifdef NoRepository
#   include "uptr_list.cpp"
#endif
#endif
