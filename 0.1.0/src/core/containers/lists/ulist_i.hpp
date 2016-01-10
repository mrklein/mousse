// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "ptraits.hpp"
#include "swap.hpp"
// Constructors 
template<class T>
inline mousse::UList<T>::UList()
:
  size_(0),
  v_(0)
{}
template<class T>
inline mousse::UList<T>::UList(T* __restrict__ v, label size)
:
  size_(size),
  v_(v)
{}
// Member Functions 
template<class T>
inline const mousse::UList<T>& mousse::UList<T>::null()
{
  return NullObjectRef<UList<T> >();
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
