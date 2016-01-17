// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_list.hpp"
#include "list_loop_m.hpp"

// STL Member Functions 
template<class T, unsigned Size>
void mousse::FixedList<T, Size>::swap(FixedList<T, Size>& a)
{
  LIST_ACCESS(T, (*this), vp);
  LIST_ACCESS(T, a, ap);
  T tmp;
  LIST_FOR_ALL((*this), i)
    tmp = List_CELEM((*this), vp, i);
    LIST_ELEM((*this), vp, i) = LIST_CELEM(a, ap, i);
    LIST_ELEM(a, ap, i) = tmp;
  LIST_END_FOR_ALL
}


// Member Operators 
template<class T, unsigned Size>
bool mousse::FixedList<T, Size>::operator==(const FixedList<T, Size>& a) const
{
  bool equal = true;
  LIST_CONST_ACCESS(T, (*this), vp);
  LIST_CONST_ACCESS(T, (a), ap);
  LIST_FOR_ALL((*this), i)
    equal = equal && (LIST_ELEM((*this), vp, i) == LIST_ELEM((a), ap, i));
  LIST_END_FOR_ALL
  return equal;
}


template<class T, unsigned Size>
bool mousse::FixedList<T, Size>::operator!=(const FixedList<T, Size>& a) const
{
  return !operator==(a);
}


template<class T, unsigned Size>
bool mousse::FixedList<T, Size>::operator<(const FixedList<T, Size>& a) const
{
  for
  (
    const_iterator vi = cbegin(), ai = a.cbegin();
    vi < cend() && ai < a.cend();
    vi++, ai++
  )
  {
    if (*vi < *ai)
    {
      return true;
    }
    else if (*vi > *ai)
    {
      return false;
    }
  }
  if (Size < a.Size)
  {
    return true;
  }
  else
  {
    return false;
  }
}


template<class T, unsigned Size>
bool mousse::FixedList<T, Size>::operator>(const FixedList<T, Size>& a) const
{
  return a.operator<(*this);
}


template<class T, unsigned Size>
bool mousse::FixedList<T, Size>::operator<=(const FixedList<T, Size>& a) const
{
  return !operator>(a);
}


template<class T, unsigned Size>
bool mousse::FixedList<T, Size>::operator>=(const FixedList<T, Size>& a) const
{
  return !operator<(a);
}

//  IOStream operators
#include "fixed_list_io.cpp"
