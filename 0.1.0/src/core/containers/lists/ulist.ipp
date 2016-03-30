// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "ulist.hpp"
#include "list_loop_m.hpp"
#include "contiguous.hpp"
#include <algorithm>


// Member Functions 
template<class T>
void mousse::UList<T>::assign(const UList<T>& a)
{
  if (a.size_ != this->size_) {
    FATAL_ERROR_IN("UList<T>::assign(const UList<T>&)")
      << "ULists have different sizes: "
      << this->size_ << " " << a.size_
      << abort(FatalError);
  }
  if (this->size_) {
    #ifdef USEMEMCPY
    if (contiguous<T>()) {
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


// Member Operators 
template<class T>
void mousse::UList<T>::operator=(const T& t)
{
  LIST_ACCESS(T, (*this), vp);
  LIST_FOR_ALL((*this), i)
    LIST_ELEM((*this), vp, i) = t;
  LIST_END_FOR_ALL
}


// STL Member Functions 
template<class T>
void mousse::UList<T>::swap(UList<T>& a)
{
  Swap(size_, a.size_);
  Swap(v_, a.v_);
}


// Member Functions 
template<class T>
std::streamsize mousse::UList<T>::byteSize() const
{
  if (!contiguous<T>()) {
    FATAL_ERROR_IN("UList<T>::byteSize()")
      << "Cannot return the binary size of a list of non-primitive elements"
      << abort(FatalError);
  }
  return this->size_*sizeof(T);
}


template<class T>
void mousse::sort(UList<T>& a)
{
  std::sort(a.begin(), a.end());
}


template<class T, class Cmp>
void mousse::sort(UList<T>& a, const Cmp& cmp)
{
  std::sort(a.begin(), a.end(), cmp);
}


template<class T>
void mousse::stableSort(UList<T>& a)
{
  std::stable_sort(a.begin(), a.end());
}


template<class T, class Cmp>
void mousse::stableSort(UList<T>& a, const Cmp& cmp)
{
  std::stable_sort(a.begin(), a.end(), cmp);
}


template<class T>
void mousse::shuffle(UList<T>& a)
{
  std::random_shuffle(a.begin(), a.end());
}


// Member Operators 
template<class T>
bool mousse::UList<T>::operator==(const UList<T>& a) const
{
  if (this->size_ != a.size_) {
    return false;
  }
  bool equal = true;
  LIST_CONST_ACCESS(T, (*this), vp);
  LIST_CONST_ACCESS(T, (a), ap);
  LIST_FOR_ALL((*this), i)
    equal = equal && (LIST_ELEM((*this), vp, i) == LIST_ELEM((a), ap, i));
  LIST_END_FOR_ALL
  return equal;
}


template<class T>
bool mousse::UList<T>::operator!=(const UList<T>& a) const
{
  return !operator==(a);
}


template<class T>
bool mousse::UList<T>::operator<(const UList<T>& a) const
{
  for
  (
    const_iterator vi = begin(), ai = a.begin();
    vi < end() && ai < a.end();
    vi++, ai++
  ) {
    if (*vi < *ai) {
      return true;
    } else if (*vi > *ai) {
      return false;
    }
  }
  if (this->size_ < a.size_) {
    return true;
  } else {
    return false;
  }
}


template<class T>
bool mousse::UList<T>::operator>(const UList<T>& a) const
{
  return a.operator<(*this);
}


template<class T>
bool mousse::UList<T>::operator<=(const UList<T>& a) const
{
  return !operator>(a);
}


template<class T>
bool mousse::UList<T>::operator>=(const UList<T>& a) const
{
  return !operator<(a);
}

//  IOStream operators
#include "ulist_io.ipp"
