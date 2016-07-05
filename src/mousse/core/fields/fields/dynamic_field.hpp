#ifndef CORE_FIELDS_FIELDS_DYNAMIC_FIELD_HPP_
#define CORE_FIELDS_FIELDS_DYNAMIC_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DynamicField
// Description
//   Dynamically sized Field.

#include "field.hpp"
#include "static_assert.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
class DynamicField;

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Ostream& operator<<
(
  Ostream&,
  const DynamicField<T, SizeInc, SizeMult, SizeDiv>&
);

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Istream& operator>>
(
  Istream&,
  DynamicField<T, SizeInc, SizeMult, SizeDiv>&
);


template<class T, unsigned SizeInc=0, unsigned SizeMult=2, unsigned SizeDiv=1>
class DynamicField
:
  public Field<T>
{
  //- Avoid invalid sizing parameters
  STATIC_ASSERT((SizeInc || SizeMult) && SizeDiv);
  // Private data
    //- The capacity (allocated size) of the underlying field.
    label capacity_;
public:
  // Static Member Functions
    //- Return a null field
    inline static const DynamicField<T, SizeInc, SizeMult, SizeDiv>& null()
    {
      return *reinterpret_cast
      <
        DynamicField<T, SizeInc, SizeMult, SizeDiv>*
      >(0);
    }
  // Constructors
    //- Construct null
    inline DynamicField();
    //- Construct given size.
    explicit inline DynamicField(const label);
    //- Construct from UList. Size set to UList size.
    //  Also constructs from DynamicField with different sizing parameters.
    explicit inline DynamicField(const UList<T>&);
    //- Construct by transferring the parameter contents
    explicit inline DynamicField(const Xfer<List<T> >&);
    //- Construct by 1 to 1 mapping from the given field
    inline DynamicField
    (
      const UList<T>& mapF,
      const labelList& mapAddressing
    );
    //- Construct by interpolative mapping from the given field
    inline DynamicField
    (
      const UList<T>& mapF,
      const labelListList& mapAddressing,
      const scalarListList& weights
    );
    //- Construct by mapping from the given field
    inline DynamicField
    (
      const UList<T>& mapF,
      const FieldMapper& map
    );
    //- Construct copy
    inline DynamicField(const DynamicField<T, SizeInc, SizeMult, SizeDiv>&);
    //- Construct by transferring the Field contents
    inline DynamicField
    (
      const Xfer<DynamicField<T, SizeInc, SizeMult, SizeDiv> >&
    );
    //- Construct from Istream. Size set to size of list read.
    explicit DynamicField(Istream&);
    //- Clone
    tmp<DynamicField<T, SizeInc, SizeMult, SizeDiv> > clone() const;
  // Member Functions
    // Access
      //- Size of the underlying storage.
      inline label capacity() const;
    // Edit
      //- Alter the size of the underlying storage.
      //  The addressed size will be truncated if needed to fit, but will
      //  remain otherwise untouched.
      //  Use this or reserve() in combination with append().
      inline void setCapacity(const label);
      //- Alter the addressed list size.
      //  New space will be allocated if required.
      //  Use this to resize the list prior to using the operator[] for
      //  setting values (as per List usage).
      inline void setSize(const label);
      //- Alter the addressed list size and fill new space with a
      //  constant.
      inline void setSize(const label, const T&);
      //- Alter the addressed list size.
      //  New space will be allocated if required.
      //  Use this to resize the list prior to using the operator[] for
      //  setting values (as per List usage).
      inline void resize(const label);
      //- Alter the addressed list size and fill new space with a
      //  constant.
      inline void resize(const label, const T&);
      //- Reserve allocation space for at least this size.
      //  Never shrinks the allocated size, use setCapacity() for that.
      inline void reserve(const label);
      //- Clear the addressed list, i.e. set the size to zero.
      //  Allocated size does not change
      inline void clear();
      //- Clear the list and delete storage.
      inline void clearStorage();
      //- Shrink the allocated space to the number of elements used.
      //  Returns a reference to the DynamicField.
      inline DynamicField<T, SizeInc, SizeMult, SizeDiv>& shrink();
      //- Transfer contents to the Xfer container as a plain List
      inline Xfer<List<T> > xfer();
    // Member Operators
      //- Append an element at the end of the list
      inline DynamicField<T, SizeInc, SizeMult, SizeDiv>& append
      (
        const T&
      );
      //- Append a List at the end of this list
      inline DynamicField<T, SizeInc, SizeMult, SizeDiv>& append
      (
        const UList<T>&
      );
      //- Remove and return the top element
      inline T remove();
      //- Return non-const access to an element, resizing list if
      //  necessary
      inline T& operator()(const label);
      //- Assignment of all addressed entries to the given value
      inline void operator=(const T&);
      //- Assignment from DynamicField
      inline void operator=
      (
        const DynamicField<T, SizeInc, SizeMult, SizeDiv>&
      );
      //- Assignment from UList
      inline void operator=(const UList<T>&);
};
}  // namespace mousse


// Constructors 
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField()
:
  Field<T>{0},
  capacity_{Field<T>::size()}
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField
(
  const label nElem
)
:
  Field<T>{nElem},
  capacity_{Field<T>::size()}
{
  // we could also enforce SizeInc granularity when (!SizeMult || !SizeDiv)
  Field<T>::size(0);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField
(
  const UList<T>& lst
)
:
  Field<T>{lst},
  capacity_{Field<T>::size()}
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField
(
  const Xfer<List<T> >& lst
)
:
  Field<T>{lst},
  capacity_{Field<T>::size()}
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField
(
  const UList<T>& mapF,
  const labelList& mapAddressing
)
:
  Field<T>{mapF, mapAddressing},
  capacity_{Field<T>::size()}
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField
(
  const UList<T>& mapF,
  const labelListList& mapAddressing,
  const scalarListList& weights
)
:
  Field<T>{mapF, mapAddressing, weights},
  capacity_{Field<T>::size()}
{}


//- Construct by mapping from the given field
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField
(
  const UList<T>& mapF,
  const FieldMapper& map
)
:
  Field<T>{mapF, map},
  capacity_{Field<T>::size()}
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField
(
  const DynamicField<T, SizeInc, SizeMult, SizeDiv>& lst
)
:
  Field<T>{lst},
  capacity_{lst.capacity()}
{}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::DynamicField
(
  const Xfer<DynamicField<T, SizeInc, SizeMult, SizeDiv> >& lst
)
:
  Field<T>{lst},
  capacity_{Field<T>::size()}
{}


// Member Functions 
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::label mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::capacity()
const
{
  return capacity_;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::setCapacity
(
  const label nElem
)
{
  label nextFree = Field<T>::size();
  capacity_ = nElem;
  if (nextFree > capacity_) {
    // truncate addressed sizes too
    nextFree = capacity_;
  }
  // we could also enforce SizeInc granularity when (!SizeMult || !SizeDiv)
  Field<T>::setSize(capacity_);
  Field<T>::size(nextFree);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::reserve
(
  const label nElem
)
{
  // allocate more capacity?
  if (nElem > capacity_) {
// TODO: convince the compiler that division by zero does not occur
//        if (SizeInc && (!SizeMult || !SizeDiv))
//        {
//            // resize with SizeInc as the granularity
//            capacity_ = nElem;
//            unsigned pad = SizeInc - (capacity_ % SizeInc);
//            if (pad != SizeInc)
//            {
//                capacity_ += pad;
//            }
//        }
//        else
    {
      capacity_ = max
      (
        nElem,
        label(SizeInc + capacity_ * SizeMult / SizeDiv)
      );
    }
    // adjust allocated size, leave addressed size untouched
    label nextFree = Field<T>::size();
    Field<T>::setSize(capacity_);
    Field<T>::size(nextFree);
  }
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::setSize
(
  const label nElem
)
{
  // allocate more capacity?
  if (nElem > capacity_) {
// TODO: convince the compiler that division by zero does not occur
//        if (SizeInc && (!SizeMult || !SizeDiv))
//        {
//            // resize with SizeInc as the granularity
//            capacity_ = nElem;
//            unsigned pad = SizeInc - (capacity_ % SizeInc);
//            if (pad != SizeInc)
//            {
//                capacity_ += pad;
//            }
//        }
//        else
    {
      capacity_ = max
      (
        nElem,
        label(SizeInc + capacity_ * SizeMult / SizeDiv)
      );
    }
    Field<T>::setSize(capacity_);
  }
  // adjust addressed size
  Field<T>::size(nElem);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::setSize
(
  const label nElem,
  const T& t
)
{
  label nextFree = Field<T>::size();
  setSize(nElem);
  // set new elements to constant value
  while (nextFree < nElem) {
    this->operator[](nextFree++) = t;
  }
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::resize
(
  const label nElem
)
{
  this->setSize(nElem);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::resize
(
  const label nElem,
  const T& t
)
{
  this->setSize(nElem, t);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::clear()
{
  Field<T>::size(0);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::clearStorage()
{
  Field<T>::clear();
  capacity_ = 0;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>&
mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::shrink()
{
  label nextFree = Field<T>::size();
  if (capacity_ > nextFree) {
    // use the full list when resizing
    Field<T>::size(capacity_);
    // the new size
    capacity_ = nextFree;
    Field<T>::setSize(capacity_);
    Field<T>::size(nextFree);
  }
  return *this;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::Xfer<mousse::List<T> >
mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::xfer()
{
  return xferMoveTo< List<T> >(*this);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>&
mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::append
(
  const T& t
)
{
  const label elemI = List<T>::size();
  setSize(elemI + 1);
  this->operator[](elemI) = t;
  return *this;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>&
mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::append
(
  const UList<T>& lst
)
{
  if (this == &lst) {
    FATAL_ERROR_IN
    (
      "DynamicField<T, SizeInc, SizeMult, SizeDiv>::append"
      "(const UList<T>&)"
    )
    << "attempted appending to self" << abort(FatalError);
  }
  label nextFree = List<T>::size();
  setSize(nextFree + lst.size());
  FOR_ALL(lst, elemI) {
    this->operator[](nextFree++) = lst[elemI];
  }
  return *this;
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline T mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::remove()
{
  const label elemI = List<T>::size() - 1;
  if (elemI < 0) {
    FATAL_ERROR_IN
    (
      "mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::remove()"
    )
    << "List is empty" << abort(FatalError);
  }
  const T& val = List<T>::operator[](elemI);
  List<T>::size(elemI);
  return val;
}


// Member Operators 
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline T& mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::operator()
(
  const label elemI
)
{
  if (elemI >= Field<T>::size()) {
    setSize(elemI + 1);
  }
  return this->operator[](elemI);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::operator=
(
  const T& t
)
{
  UList<T>::operator=(t);
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::operator=
(
  const DynamicField<T, SizeInc, SizeMult, SizeDiv>& lst
)
{
  if (this == &lst) {
    FATAL_ERROR_IN
    (
      "DynamicField<T, SizeInc, SizeMult, SizeDiv>::operator="
      "(const DynamicField<T, SizeInc, SizeMult, SizeDiv>&)"
    )
    << "attempted assignment to self" << abort(FatalError);
  }
  if (capacity_ >= lst.size()) {
    // can copy w/o reallocating, match initial size to avoid reallocation
    Field<T>::size(lst.size());
    Field<T>::operator=(lst);
  } else {
    // make everything available for the copy operation
    Field<T>::size(capacity_);
    Field<T>::operator=(lst);
    capacity_ = Field<T>::size();
  }
}


template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
inline void mousse::DynamicField<T, SizeInc, SizeMult, SizeDiv>::operator=
(
  const UList<T>& lst
)
{
  if (capacity_ >= lst.size()) {
    // can copy w/o reallocating, match initial size to avoid reallocation
    Field<T>::size(lst.size());
    Field<T>::operator=(lst);
  } else {
    // make everything available for the copy operation
    Field<T>::size(capacity_);
    Field<T>::operator=(lst);
    capacity_ = Field<T>::size();
  }
}

#include "dynamic_field.ipp"

#endif
