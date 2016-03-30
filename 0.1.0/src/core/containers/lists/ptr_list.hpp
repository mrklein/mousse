#ifndef CORE_CONTAINERS_LISTS_PTR_LIST_HPP_
#define CORE_CONTAINERS_LISTS_PTR_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PtrList
// Description
//   A templated 1D list of pointers to objects of type \<T\>, where the
//   size of the array is known and used for subscript bounds checking, etc.
//   The element operator [] returns a reference to the object rather than a
//   pointer.

#include "list.hpp"
#include "auto_ptr.hpp"
#include "tmp.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
template<class T> class PtrList;
template<class T> class SLPtrList;

template<class T>
inline typename PtrList<T>::iterator operator+
(
  const typename PtrList<T>::iterator&,
  label
);

template<class T>
inline typename PtrList<T>::iterator operator+
(
  label,
  const typename PtrList<T>::iterator&
);

template<class T>
inline typename PtrList<T>::iterator operator-
(
  const typename PtrList<T>::iterator&,
  label
);

template<class T>
inline label operator-
(
  const typename PtrList<T>::iterator&,
  const typename PtrList<T>::iterator&
);
template<class T>
inline typename PtrList<T>::const_iterator operator+
(
  const typename PtrList<T>::const_iterator&,
  label
);
template<class T>
inline typename PtrList<T>::const_iterator operator+
(
  label,
  const typename PtrList<T>::const_iterator&
);
template<class T>
inline typename PtrList<T>::const_iterator operator-
(
  const typename PtrList<T>::const_iterator&,
  label
);

template<class T>
inline label operator-
(
  const typename PtrList<T>::const_iterator&,
  const typename PtrList<T>::const_iterator&
);

template<class T>
Istream& operator>>(Istream&, PtrList<T>&);

template<class T>
Ostream& operator<<(Ostream&, const PtrList<T>&);

template<class T>
class PtrList
{
  // Private data
    List<T*> ptrs_;

protected:

  // Protected Member Functions

    //- Read from Istream using given Istream constructor class
    template<class INew>
    void read(Istream&, const INew& inewt);

public:

  // Constructors

    //- Null Constructor.
    PtrList();

    //- Construct with size specified.
    explicit PtrList(const label);

    //- Copy constructor.
    PtrList(const PtrList<T>&);

    //- Copy constructor with additional argument for clone
    template<class CloneArg>
    PtrList(const PtrList<T>&, const CloneArg&);

    //- Construct by transferring the parameter contents
    PtrList(const Xfer<PtrList<T> >&);

    //- Construct as copy or re-use as specified.
    PtrList(PtrList<T>&, bool reUse);

    //- Construct as copy of SLPtrList<T>
    explicit PtrList(const SLPtrList<T>&);

    //- Construct from Istream using given Istream constructor class
    template<class INew>
    PtrList(Istream&, const INew&);

    //- Construct from Istream using default Istream constructor class
    PtrList(Istream&);

  //- Destructor
  ~PtrList();

  // Member functions

    // Access

      //- Return the number of elements in the PtrList
      inline label size() const;

      //- Return true if the PtrList is empty (ie, size() is zero).
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

      //- Reset size of PtrList. If extending the PtrList, new entries are
      //  set to NULL. If truncating the PtrList, removed entries are
      //  deleted.
      void setSize(const label);

      //- Alias for setSize(const label)
      inline void resize(const label);

      //- Clear the PtrList, i.e. set size to zero deleting all the
      //  allocated entries.
      void clear();

      //- Append an element at the end of the list
      inline void append(T*);
      inline void append(const autoPtr<T>&);
      inline void append(const tmp<T>&);

      //- Transfer the contents of the argument PtrList into this PtrList
      //  and annul the argument list.
      void transfer(PtrList<T>&);

      //- Transfer contents to the Xfer container
      inline Xfer<PtrList<T> > xfer();

      //- Is element set
      inline bool set(const label) const;

      //- Set element. Return old element (can be NULL).
      //  No checks on new element.
      inline autoPtr<T> set(const label, T*);
      inline autoPtr<T> set(const label, const autoPtr<T>&);
      inline autoPtr<T> set(const label, const tmp<T>&);

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

    //- Assignment.
    PtrList<T>& operator=(const PtrList<T>&);

  // STL type definitions

    //- Type of values the PtrList contains.
    typedef T value_type;

    //- Type that can be used for storing into PtrList::value_type objects.
    typedef T& reference;

    //- Type that can be used for storing into constant PtrList::value_type
    //  objects.
    typedef const T& const_reference;

  // STL iterator

  // Random access iterator for traversing PtrList.
    class iterator;
    class const_iterator;
    friend class iterator;
    friend class const_iterator;

    //- An STL-conforming iterator
    class iterator
    {
      T** ptr_;
    public:

      friend class const_iterator;

      //- Construct for a given PtrList entry
      inline iterator(T**);

      // Member operators
        inline bool operator==(const iterator&) const;
        inline bool operator!=(const iterator&) const;
        typedef T& Tref;
        inline Tref operator*();
        inline Tref operator()();
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

    //- Return an iterator to begin traversing the PtrList.
    inline iterator begin();

    //- Return an iterator to end traversing the PtrList.
    inline iterator end();

  // STL const_iterator

  // Random access iterator for traversing PtrList.

    //- An STL-conforming const_iterator
    class const_iterator
    {
      const T* const* ptr_;

    public:

      //- Construct for a given PtrList entry
      inline const_iterator(const T* const*);

      //- Construct from an iterator
      inline const_iterator(const iterator&);

      // Member operators
        inline bool operator==(const const_iterator&) const;
        inline bool operator!=(const const_iterator&) const;
        typedef const T& Tref;
        inline Tref operator*();
        inline Tref operator()();
        inline const_iterator operator++();
        inline const_iterator operator++(int);
        inline const_iterator operator--();
        inline const_iterator operator--(int);
        inline const_iterator operator+=(label);
        friend const_iterator operator+ <T>
        (
          const const_iterator&,
          label
        );
        friend const_iterator operator+ <T>
        (
          label,
          const const_iterator&
        );
        inline const_iterator operator-=(label);
        friend const_iterator operator- <T>
        (
          const const_iterator&,
          label
        );
        friend label operator- <T>
        (
          const const_iterator&,
          const const_iterator&
        );
        inline const T& operator[](label);
        inline bool operator<(const const_iterator&) const;
        inline bool operator>(const const_iterator&) const;
        inline bool operator<=(const const_iterator&) const;
        inline bool operator>=(const const_iterator&) const;
    };

    //- Return an const_iterator to begin traversing the PtrList.
    inline const_iterator cbegin() const;

    //- Return an const_iterator to end traversing the PtrList.
    inline const_iterator cend() const;

    //- Return an const_iterator to begin traversing the PtrList.
    inline const_iterator begin() const;

    //- Return an const_iterator to end traversing the PtrList.
    inline const_iterator end() const;

  // IOstream operator

    //- Read List from Istream, discarding contents of existing List.
    friend Istream& operator>> <T>(Istream&, PtrList<T>&);

    // Write List to Ostream.
    friend Ostream& operator<< <T>(Ostream&, const PtrList<T>&);
};
}  // namespace mousse

// Member Functions 
template<class T>
inline mousse::label mousse::PtrList<T>::size() const
{
  return ptrs_.size();
}


template<class T>
inline bool mousse::PtrList<T>::empty() const
{
  return ptrs_.empty();
}


template<class T>
inline T& mousse::PtrList<T>::first()
{
  return this->operator[](0);
}


template<class T>
inline const T& mousse::PtrList<T>::first() const
{
  return this->operator[](0);
}


template<class T>
inline T& mousse::PtrList<T>::last()
{
  return this->operator[](this->size()-1);
}


template<class T>
inline const T& mousse::PtrList<T>::last() const
{
  return this->operator[](this->size()-1);
}


template<class T>
inline void mousse::PtrList<T>::resize(const label newSize)
{
  this->setSize(newSize);
}


template<class T>
inline void mousse::PtrList<T>::append(T* ptr)
{
  label sz = size();
  this->setSize(sz+1);
  ptrs_[sz] = ptr;
}


template<class T>
inline void mousse::PtrList<T>::append(const autoPtr<T>& aptr)
{
  return append(const_cast<autoPtr<T>&>(aptr).ptr());
}


template<class T>
inline void mousse::PtrList<T>::append
(
  const tmp<T>& t
)
{
  return append(const_cast<tmp<T>&>(t).ptr());
}


template<class T>
inline bool mousse::PtrList<T>::set(const label i) const
{
  return ptrs_[i] != NULL;
}


template<class T>
inline mousse::autoPtr<T> mousse::PtrList<T>::set(const label i, T* ptr)
{
  autoPtr<T> old{ptrs_[i]};
  ptrs_[i] = ptr;
  return old;
}


template<class T>
inline mousse::autoPtr<T> mousse::PtrList<T>::set
(
  const label i,
  const autoPtr<T>& aptr
)
{
  return set(i, const_cast<autoPtr<T>&>(aptr).ptr());
}


template<class T>
inline mousse::autoPtr<T> mousse::PtrList<T>::set
(
  const label i,
  const tmp<T>& t
)
{
  return set(i, const_cast<tmp<T>&>(t).ptr());
}


template<class T>
inline mousse::Xfer<mousse::PtrList<T> > mousse::PtrList<T>::xfer()
{
  return xferMove(*this);
}


// Member Operators 
template<class T>
inline const T& mousse::PtrList<T>::operator[](const label i) const
{
  if (!ptrs_[i]) {
    FATAL_ERROR_IN("PtrList::operator[] const")
      << "hanging pointer of type " << typeid(T).name()
      << " at index " << i
      << " (size " << size()
      << "), cannot dereference"
      << abort(FatalError);
  }
  return *(ptrs_[i]);
}


template<class T>
inline T& mousse::PtrList<T>::operator[](const label i)
{
  if (!ptrs_[i]) {
    FATAL_ERROR_IN("PtrList::operator[]")
      << "hanging pointer of type " << typeid(T).name()
      << " at index " << i
      << " (size " << size()
      << "), cannot dereference"
      << abort(FatalError);
  }
  return *(ptrs_[i]);
}


template<class T>
inline const T* mousse::PtrList<T>::operator()(const label i) const
{
  return ptrs_[i];
}


// STL iterator 
template<class T>
inline mousse::PtrList<T>::iterator::iterator(T** ptr)
:
  ptr_{ptr}
{}


template<class T>
inline bool mousse::PtrList<T>::iterator::operator==(const iterator& iter) const
{
  return ptr_ == iter.ptr_;
}


template<class T>
inline bool mousse::PtrList<T>::iterator::operator!=(const iterator& iter) const
{
  return ptr_ != iter.ptr_;
}


template<class T>
inline T& mousse::PtrList<T>::iterator::operator*()
{
  return **ptr_;
}


template<class T>
inline T& mousse::PtrList<T>::iterator::operator()()
{
  return operator*();
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::PtrList<T>::iterator::operator++()
{
  ++ptr_;
  return *this;
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::PtrList<T>::iterator::operator++(int)
{
  iterator tmp = *this;
  ++ptr_;
  return tmp;
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::PtrList<T>::iterator::operator--()
{
  --ptr_;
  return *this;
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::PtrList<T>::iterator::operator--(int)
{
  iterator tmp = *this;
  --ptr_;
  return tmp;
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::PtrList<T>::iterator::operator+=(label n)
{
  ptr_ += n;
  return *this;
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::operator+(const typename PtrList<T>::iterator& iter, label n)
{
  typename PtrList<T>::iterator tmp = iter;
  return tmp += n;
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::operator+(label n, const typename PtrList<T>::iterator& iter)
{
  typename PtrList<T>::iterator tmp = iter;
  return tmp += n;
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::PtrList<T>::iterator::operator-=(label n)
{
  ptr_ -= n;
  return *this;
}


template<class T>
inline typename mousse::PtrList<T>::iterator
mousse::operator-(const typename PtrList<T>::iterator& iter, label n)
{
  typename PtrList<T>::iterator tmp = iter;
  return tmp -= n;
}


template<class T>
inline mousse::label mousse::operator-
(
  const typename PtrList<T>::iterator& iter1,
  const typename PtrList<T>::iterator& iter2
)
{
  return (iter1.ptr_ - iter2.ptr_)/sizeof(T*);
}


template<class T>
inline T& mousse::PtrList<T>::iterator::operator[](label n)
{
  return *(*this + n);
}


template<class T>
inline bool mousse::PtrList<T>::iterator::operator<(const iterator& iter) const
{
  return ptr_ < iter.ptr_;
}


template<class T>
inline bool mousse::PtrList<T>::iterator::operator>(const iterator& iter) const
{
  return ptr_ > iter.ptr_;
}


template<class T>
inline bool mousse::PtrList<T>::iterator::operator<=(const iterator& iter) const
{
  return ptr_ <= iter.ptr_;
}


template<class T>
inline bool mousse::PtrList<T>::iterator::operator>=(const iterator& iter) const
{
  return ptr_ >= iter.ptr_;
}


template<class T>
inline typename mousse::PtrList<T>::iterator mousse::PtrList<T>::begin()
{
  return ptrs_.begin();
}


template<class T>
inline typename mousse::PtrList<T>::iterator mousse::PtrList<T>::end()
{
  return ptrs_.end();
}


// STL const_iterator 
template<class T>
inline mousse::PtrList<T>::const_iterator::const_iterator(const T* const* ptr)
:
  ptr_{ptr}
{}


template<class T>
inline mousse::PtrList<T>::const_iterator::const_iterator(const iterator& iter)
:
  ptr_{iter.ptr_}
{}


template<class T>
inline bool mousse::PtrList<T>::const_iterator::operator==
(
  const const_iterator& iter
) const
{
  return ptr_ == iter.ptr_;
}


template<class T>
inline bool mousse::PtrList<T>::const_iterator::operator!=
(
  const const_iterator& iter
) const
{
  return ptr_ != iter.ptr_;
}


template<class T>
inline const T& mousse::PtrList<T>::const_iterator::operator*()
{
  return **ptr_;
}


template<class T>
inline const T& mousse::PtrList<T>::const_iterator::operator()()
{
  return operator*();
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::const_iterator::operator++()
{
  ++ptr_;
  return *this;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::const_iterator::operator++(int)
{
  const_iterator tmp = *this;
  ++ptr_;
  return tmp;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::const_iterator::operator--()
{
  --ptr_;
  return *this;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::const_iterator::operator--(int)
{
  const_iterator tmp = *this;
  --ptr_;
  return tmp;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::const_iterator::operator+=(label n)
{
  ptr_ += n;
  return *this;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::operator+(const typename PtrList<T>::const_iterator& iter, label n)
{
  typename PtrList<T>::const_iterator tmp = iter;
  return tmp += n;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::operator+(label n, const typename PtrList<T>::const_iterator& iter)
{
  typename PtrList<T>::const_iterator tmp = iter;
  return tmp += n;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::const_iterator::operator-=(label n)
{
  ptr_ -= n;
  return *this;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::operator-(const typename PtrList<T>::const_iterator& iter, label n)
{
  typename PtrList<T>::const_iterator tmp = iter;
  return tmp -= n;
}


template<class T>
inline mousse::label mousse::operator-
(
  const typename PtrList<T>::const_iterator& iter1,
  const typename PtrList<T>::const_iterator& iter2
)
{
  return (iter1.ptr_ - iter2.ptr_)/sizeof(T*);
}


template<class T>
inline const T& mousse::PtrList<T>::const_iterator::operator[](label n)
{
  return *(*this + n);
}


template<class T>
inline bool mousse::PtrList<T>::const_iterator::operator<
(
  const const_iterator& iter
) const
{
  return ptr_ < iter.ptr_;
}


template<class T>
inline bool mousse::PtrList<T>::const_iterator::operator>
(
  const const_iterator& iter
) const
{
  return ptr_ > iter.ptr_;
}


template<class T>
inline bool mousse::PtrList<T>::const_iterator::operator<=
(
  const const_iterator& iter
) const
{
  return ptr_ <= iter.ptr_;
}


template<class T>
inline bool mousse::PtrList<T>::const_iterator::operator>=
(
  const const_iterator& iter
) const
{
  return ptr_ >= iter.ptr_;
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::begin() const
{
  return ptrs_.begin();
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::end() const
{
  return ptrs_.end();
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::cbegin() const
{
  return ptrs_.begin();
}


template<class T>
inline typename mousse::PtrList<T>::const_iterator
mousse::PtrList<T>::cend() const
{
  return ptrs_.end();
}


#include "ptr_list.ipp"

#endif
