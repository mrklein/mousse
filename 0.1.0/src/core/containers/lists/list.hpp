#ifndef CORE_CONTAINERS_LISTS_LIST_HPP_
#define CORE_CONTAINERS_LISTS_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::List
// Description
//   A 1D array of objects of type \<T\>, where the size of the vector
//   is known and used for subscript bounds checking, etc.
//   Storage is allocated on free-store during construction.
// SourceFiles
//   list.cpp
//   list_io.cpp


#include "ulist.hpp"
#include "auto_ptr.hpp"
#include "xfer.hpp"

namespace mousse
{

class Istream;
class Ostream;

// Forward declaration of friend functions and operators
template<class T> class List;
template<class T> Istream& operator>>(Istream&, List<T>&);
template<class T, unsigned Size> class FixedList;
template<class T> class PtrList;
template<class T> class SLList;
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
  class DynamicList;
template<class T> class SortableList;
template<class T> class IndirectList;
template<class T> class UIndirectList;
template<class T> class BiIndirectList;
typedef UList<label> unallocLabelList;
template<class T>

class List
:
  public UList<T>
{
protected:

  //- Override size to be inconsistent with allocated storage.
  //  Use with care.
  inline void size(const label);

public:

  // Static Member Functions

    //- Return a null List
    inline static const List<T>& null();

  // Constructors

    //- Null constructor.
    inline List();

    //- Construct with given size.
    explicit List(const label);

    //- Construct with given size and value for all elements.
    List(const label, const T&);

    //- Copy constructor.
    List(const List<T>&);

    //- Construct by transferring the parameter contents
    List(const Xfer<List<T>>&);

    //- Construct as copy or re-use as specified.
    List(List<T>&, bool reUse);

    //- Construct as subset.
    List(const UList<T>&, const labelUList& mapAddressing);

    //- Construct given start and end iterators.
    template<class InputIterator>
    List(InputIterator first, InputIterator last);

    //- Construct as copy of FixedList<T, Size>
    template<unsigned Size>
    explicit List(const FixedList<T, Size>&);

    //- Construct as copy of PtrList<T>
    explicit List(const PtrList<T>&);

    //- Construct as copy of SLList<T>
    explicit List(const SLList<T>&);

    //- Construct as copy of UIndirectList<T>
    explicit List(const UIndirectList<T>&);

    //- Construct as copy of BiIndirectList<T>
    explicit List(const BiIndirectList<T>&);

    //- Construct from Istream.
    List(Istream&);

    //- Clone
    inline autoPtr<List<T>> clone() const;

  //- Destructor
  ~List();

  // Related types

    //- Declare type of subList
    typedef SubList<T> subList;

  // Member Functions

    //- Return the number of elements in the UList.
    inline label size() const;

    // Edit

      //- Alias for setSize(const label)
      inline void resize(const label);

      //- Alias for setSize(const label, const T&)
      inline void resize(const label, const T&);

      //- Reset size of List.
      void setSize(const label);

      //- Reset size of List and value for new elements.
      void setSize(const label, const T&);

      //- Clear the list, i.e. set size to zero.
      void clear();

      //- Append an element at the end of the list
      inline void append(const T&);

      //- Append a List at the end of this list
      inline void append(const UList<T>&);

      //- Append a UIndirectList at the end of this list
      inline void append(const UIndirectList<T>&);

      //- Transfer the contents of the argument List into this list
      //  and annul the argument list.
      void transfer(List<T>&);

      //- Transfer the contents of the argument List into this list
      //  and annul the argument list.
      template<unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
      void transfer(DynamicList<T, SizeInc, SizeMult, SizeDiv>&);

      //- Transfer the contents of the argument List into this list
      //  and annul the argument list.
      void transfer(SortableList<T>&);

      //- Transfer contents to the Xfer container
      inline Xfer<List<T>> xfer();

      //- Return subscript-checked element of UList.
      inline T& newElmt(const label);

  // Member operators

    //- Assignment from UList operator. Takes linear time.
    void operator=(const UList<T>&);

    //- Assignment operator. Takes linear time.
    void operator=(const List<T>&);

    //- Assignment from SLList operator. Takes linear time.
    void operator=(const SLList<T>&);

    //- Assignment from UIndirectList operator. Takes linear time.
    void operator=(const UIndirectList<T>&);

    //- Assignment from BiIndirectList operator. Takes linear time.
    void operator=(const BiIndirectList<T>&);

    //- Assignment of all entries to the given value
    inline void operator=(const T&);

  // Istream operator

    //- Read List from Istream, discarding contents of existing List.
    friend Istream& operator>> <T>
    (Istream&, List<T>&);

};

//- Read a bracket-delimited list, or handle a single value as list of size 1.
//  For example,
//  \code
//      wList = readList<word>(IStringStream("(patch1 patch2 patch3)")());
//      wList = readList<word>(IStringStream("patch0")());
//  \endcode
//  Mostly useful for handling command-line arguments.
template<class T>
List<T> readList(Istream&);
}  // namespace mousse

// Constructors 
template<class T>
inline mousse::List<T>::List()
{}

template<class T>
inline mousse::autoPtr<mousse::List<T>> mousse::List<T>::clone() const
{
  return autoPtr<List<T>>{new List<T>{*this}};
}

// Member Functions 
template<class T>
inline const mousse::List<T>& mousse::List<T>::null()
{
  return NullObjectRef<List<T>>();
}

template<class T>
inline void mousse::List<T>::resize(const label newSize)
{
  this->setSize(newSize);
}

template<class T>
inline void mousse::List<T>::resize(const label newSize, const T& a)
{
  this->setSize(newSize, a);
}

template<class T>
inline T& mousse::List<T>::newElmt(const label i)
{
  if (i >= this->size())
  {
    setSize(2*this->size());
  }
  return UList<T>::operator[](i);
}

template<class T>
inline void mousse::List<T>::size(const label n)
{
  UList<T>::size_ = n;
}

template<class T>
inline mousse::label mousse::List<T>::size() const
{
  return UList<T>::size_;
}

template<class T>
inline mousse::Xfer<mousse::List<T>> mousse::List<T>::xfer()
{
  return xferMove(*this);
}

template<class T>
inline void mousse::List<T>::append(const T& t)
{
  setSize(size()+1, t);
}

template<class T>
inline void mousse::List<T>::append(const UList<T>& lst)
{
  if (this == &lst)
  {
    FATAL_ERROR_IN
    (
      "List<T>::append(const UList<T>&)"
    )
    << "attempted appending to self" << abort(FatalError);
  }
  label nextFree = this->size();
  setSize(nextFree + lst.size());
  FOR_ALL(lst, elemI)
  {
    this->operator[](nextFree++) = lst[elemI];
  }
}

template<class T>
inline void mousse::List<T>::append(const UIndirectList<T>& lst)
{
  label nextFree = this->size();
  setSize(nextFree + lst.size());
  FOR_ALL(lst, elemI)
  {
    this->operator[](nextFree++) = lst[elemI];
  }
}

// Member Operators 
template<class T>
inline void mousse::List<T>::operator=(const T& t)
{
  UList<T>::operator=(t);
}

#ifdef NoRepository
#include "list.cpp"
#endif
#endif
