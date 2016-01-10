// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list.hpp"
#include "list_loop_m.hpp"
#include "fixed_list.hpp"
#include "ptr_list.hpp"
#include "sl_list.hpp"
#include "indirect_list.hpp"
#include "uindirect_list.hpp"
#include "bi_indirect_list.hpp"
#include "contiguous.hpp"

// Constructors
// Construct with length specified
template<class T>
mousse::List<T>::List(const label s)
:
  UList<T>{NULL, s}
{
  if (this->size_ < 0)
  {
    FATAL_ERROR_IN("List<T>::List(const label size)")
      << "bad size " << this->size_
      << abort(FatalError);
  }
  if (this->size_)
  {
    this->v_ = new T[this->size_];
  }
}


// Construct with length and single value specified
template<class T>
mousse::List<T>::List(const label s, const T& a)
:
  UList<T>{NULL, s}
{
  if (this->size_ < 0)
  {
    FATAL_ERROR_IN("List<T>::List(const label size, const T&)")
      << "bad size " << this->size_
      << abort(FatalError);
  }
  if (this->size_)
  {
    this->v_ = new T[this->size_];
    LIST_ACCESS(T, (*this), vp);
    LIST_FOR_ALL((*this), i)
      LIST_ELEM((*this), vp, i) = a;
    LIST_END_FOR_ALL
  }
}


// Construct as copy
template<class T>
mousse::List<T>::List(const List<T>& a)
:
  UList<T>{NULL, a.size_}
{
  if (this->size_)
  {
    this->v_ = new T[this->size_];
#ifdef USEMEMCPY
    if (contiguous<T>())
    {
      memcpy(this->v_, a.v_, this->byteSize());
    }
    else
#endif
    {
      LIST_ACCESS(T, (*this), vp);
      LIST_CONST_ACCESS(T, a, ap);
      LIST_FOR_ALL((*this), i)
        LIST_ELEM((*this), vp, i) = LIST_ELEM(a, ap, i);
      LIST_END_FOR_ALL
    }
  }
}

// Construct by transferring the parameter contents
template<class T>
mousse::List<T>::List(const Xfer<List<T> >& lst)
{
  transfer(lst());
}

// Construct as copy or re-use as specified.
template<class T>
mousse::List<T>::List(List<T>& a, bool reUse)
:
  UList<T>{NULL, a.size_}
{
  if (reUse)
  {
    this->v_ = a.v_;
    a.v_ = 0;
    a.size_ = 0;
  }
  else if (this->size_)
  {
    this->v_ = new T[this->size_];
#ifdef USEMEMCPY
    if (contiguous<T>())
    {
      memcpy(this->v_, a.v_, this->byteSize());
    }
    else
#endif
    {
      LIST_ACCESS(T, (*this), vp);
      LIST_CONST_ACCESS(T, a, ap);
      LIST_FOR_ALL((*this), i)
        LIST_ELEM((*this), vp, i) = LIST_ELEM(a, ap, i);
      LIST_END_FOR_ALL
    }
  }
}


// Construct as subset
template<class T>
mousse::List<T>::List(const UList<T>& a, const labelUList& map)
:
  UList<T>{NULL, map.size()}
{
  if (this->size_)
  {
    // Note:cannot use LIST_ELEM since third argument has to be index.
    this->v_ = new T[this->size_];
    FOR_ALL(*this, i)
    {
      this->v_[i] = a[map[i]];
    }
  }
}


// Construct given start and end iterators.
template<class T>
template<class InputIterator>
mousse::List<T>::List(InputIterator first, InputIterator last)
{
  label s = 0;
  for
  (
    InputIterator iter = first;
    iter != last;
    ++iter
  )
  {
    s++;
  }
  setSize(s);
  s = 0;
  for
  (
    InputIterator iter = first;
    iter != last;
    ++iter
  )
  {
    this->operator[](s++) = *iter;
  }
}


// Construct as copy of FixedList<T, Size>
template<class T>
template<unsigned Size>
mousse::List<T>::List(const FixedList<T, Size>& lst)
:
  UList<T>{NULL, Size}
{
  if (this->size_)
  {
    this->v_ = new T[this->size_];
    FOR_ALL(*this, i)
    {
      this->operator[](i) = lst[i];
    }
  }
}


// Construct as copy of PtrList<T>
template<class T>
mousse::List<T>::List(const PtrList<T>& lst)
:
  UList<T>{NULL, lst.size()}
{
  if (this->size_)
  {
    this->v_ = new T[this->size_];
    FOR_ALL(*this, i)
    {
      this->operator[](i) = lst[i];
    }
  }
}


// Construct as copy of SLList<T>
template<class T>
mousse::List<T>::List(const SLList<T>& lst)
:
  UList<T>{NULL, lst.size()}
{
  if (this->size_)
  {
    this->v_ = new T[this->size_];
    label i = 0;
    for
    (
      typename SLList<T>::const_iterator iter = lst.begin();
      iter != lst.end();
      ++iter
    )
    {
      this->operator[](i++) = iter();
    }
  }
}


// Construct as copy of UIndirectList<T>
template<class T>
mousse::List<T>::List(const UIndirectList<T>& lst)
:
  UList<T>{NULL, lst.size()}
{
  if (this->size_)
  {
    this->v_ = new T[this->size_];
    FOR_ALL(*this, i)
    {
      this->operator[](i) = lst[i];
    }
  }
}


// Construct as copy of BiIndirectList<T>
template<class T>
mousse::List<T>::List(const BiIndirectList<T>& lst)
:
  UList<T>{NULL, lst.size()}
{
  if (this->size_)
  {
    this->v_ = new T[this->size_];
    FOR_ALL(*this, i)
    {
      this->operator[](i) = lst[i];
    }
  }
}


// Destructor
// Destroy list elements
template<class T>
mousse::List<T>::~List()
{
  if (this->v_) delete[] this->v_;
}


// Member Functions
template<class T>
void mousse::List<T>::setSize(const label newSize)
{
  if (newSize < 0)
  {
    FATAL_ERROR_IN("List<T>::setSize(const label)")
      << "bad set size " << newSize
      << abort(FatalError);
  }
  if (newSize != this->size_)
  {
    if (newSize > 0)
    {
      T* nv = new T[label(newSize)];
      if (this->size_)
      {
        label i = min(this->size_, newSize);
#ifdef USEMEMCPY
        if (contiguous<T>())
        {
          memcpy(nv, this->v_, i*sizeof(T));
        }
        else
#endif
        {
          T* vv = &this->v_[i];
          T* av = &nv[i];
          while (i--) *--av = *--vv;
        }
      }
      if (this->v_) delete[] this->v_;
      this->size_ = newSize;
      this->v_ = nv;
    }
    else
    {
      clear();
    }
  }
}


template<class T>
void mousse::List<T>::setSize(const label newSize, const T& a)
{
  label oldSize = label(this->size_);
  this->setSize(newSize);
  if (newSize > oldSize)
  {
    label i = newSize - oldSize;
    T* vv = &this->v_[newSize];
    while (i--) *--vv = a;
  }
}


template<class T>
void mousse::List<T>::clear()
{
  if (this->v_) delete[] this->v_;
  this->size_ = 0;
  this->v_ = 0;
}


// Transfer the contents of the argument List into this List
// and annul the argument list
template<class T>
void mousse::List<T>::transfer(List<T>& a)
{
  if (this->v_) delete[] this->v_;
  this->size_ = a.size_;
  this->v_ = a.v_;
  a.size_ = 0;
  a.v_ = 0;
}


// Transfer the contents of the argument DynamicList into this List
// and annul the argument list
template<class T>
template<unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
void mousse::List<T>::transfer(DynamicList<T, SizeInc, SizeMult, SizeDiv>& a)
{
  // shrink the allocated space to the number of elements used
  a.shrink();
  transfer(static_cast<List<T>&>(a));
  a.clearStorage();
}


// Transfer the contents of the argument SortableList into this List
// and annul the argument list
template<class T>
void mousse::List<T>::transfer(SortableList<T>& a)
{
  // shrink away the sort indices
  a.shrink();
  transfer(static_cast<List<T>&>(a));
}



// Member Operators
// Assignment to UList operator. Takes linear time.
template<class T>
void mousse::List<T>::operator=(const UList<T>& a)
{
  if (a.size_ != this->size_)
  {
    if (this->v_) delete[] this->v_;
    this->v_ = 0;
    this->size_ = a.size_;
    if (this->size_) this->v_ = new T[this->size_];
  }
  if (this->size_)
  {
#ifdef USEMEMCPY
    if (contiguous<T>())
    {
      memcpy(this->v_, a.v_, this->byteSize());
    }
    else
#endif
    {
      LIST_ACCESS(T, (*this), vp);
      LIST_CONST_ACCESS(T, a, ap);
      LIST_FOR_ALL((*this), i)
        LIST_ELEM((*this), vp, i) = LIST_ELEM(a, ap, i);
      LIST_END_FOR_ALL
    }
  }
}


// Assignment operator. Takes linear time.
template<class T>
void mousse::List<T>::operator=(const List<T>& a)
{
  if (this == &a)
  {
    FATAL_ERROR_IN("List<T>::operator=(const List<T>&)")
      << "attempted assignment to self"
      << abort(FatalError);
  }
  operator=(static_cast<const UList<T>&>(a));
}


// Assignment operator. Takes linear time.
template<class T>
void mousse::List<T>::operator=(const SLList<T>& lst)
{
  if (lst.size() != this->size_)
  {
    if (this->v_) delete[] this->v_;
    this->v_ = 0;
    this->size_ = lst.size();
    if (this->size_) this->v_ = new T[this->size_];
  }
  if (this->size_)
  {
    label i = 0;
    for
    (
      typename SLList<T>::const_iterator iter = lst.begin();
      iter != lst.end();
      ++iter
    )
    {
      this->operator[](i++) = iter();
    }
  }
}


// Assignment operator. Takes linear time.
template<class T>
void mousse::List<T>::operator=(const UIndirectList<T>& lst)
{
  if (lst.size() != this->size_)
  {
    if (this->v_) delete[] this->v_;
    this->v_ = 0;
    this->size_ = lst.size();
    if (this->size_) this->v_ = new T[this->size_];
  }
  FOR_ALL(*this, i)
  {
    this->operator[](i) = lst[i];
  }
}


// Assignment operator. Takes linear time.
template<class T>
void mousse::List<T>::operator=(const BiIndirectList<T>& lst)
{
  if (lst.size() != this->size_)
  {
    if (this->v_) delete[] this->v_;
    this->v_ = 0;
    this->size_ = lst.size();
    if (this->size_) this->v_ = new T[this->size_];
  }
  FOR_ALL(*this, i)
  {
    this->operator[](i) = lst[i];
  }
}

//  IOStream operators
#include "list_io.cpp"
