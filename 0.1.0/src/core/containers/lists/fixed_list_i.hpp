// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ulist.hpp"
#include "sl_list.hpp"
#include "contiguous.hpp"

// Constructors
template<class T, unsigned Size>
inline mousse::FixedList<T, Size>::FixedList()
{}
template<class T, unsigned Size>
inline mousse::FixedList<T, Size>::FixedList(const T v[Size])
{
  for (unsigned i=0; i<Size; i++)
  {
    v_[i] = v[i];
  }
}
template<class T, unsigned Size>
inline mousse::FixedList<T, Size>::FixedList(const T& t)
{
  for (unsigned i=0; i<Size; i++)
  {
    v_[i] = t;
  }
}
template<class T, unsigned Size>
inline mousse::FixedList<T, Size>::FixedList(const UList<T>& lst)
{
  checkSize(lst.size());
  for (unsigned i=0; i<Size; i++)
  {
    v_[i] = lst[i];
  }
}
template<class T, unsigned Size>
inline mousse::FixedList<T, Size>::FixedList(const SLList<T>& lst)
{
  checkSize(lst.size());
  label i = 0;
  for
  (
    typename SLList<T>::const_iterator iter = lst.begin();
    iter != lst.end();
    ++iter
  )
  {
    operator[](i++) = iter();
  }
}
template<class T, unsigned Size>
inline mousse::FixedList<T, Size>::FixedList(const FixedList<T, Size>& lst)
{
  for (unsigned i=0; i<Size; i++)
  {
    v_[i] = lst[i];
  }
}
template<class T, unsigned Size>
inline mousse::autoPtr< mousse::FixedList<T, Size> >
mousse::FixedList<T, Size>::clone() const
{
  return autoPtr< FixedList<T, Size> >(new FixedList<T, Size>(*this));
}
// Member Functions
template<class T, unsigned Size>
inline const mousse::FixedList<T, Size>& mousse::FixedList<T, Size>::null()
{
  return NullObjectRef<FixedList<T, Size> >();
}
template<class T, unsigned Size>
inline mousse::label mousse::FixedList<T, Size>::fcIndex(const label i) const
{
  return (i == Size-1 ? 0 : i+1);
}
template<class T, unsigned Size>
inline mousse::label mousse::FixedList<T, Size>::rcIndex(const label i) const
{
  return (i ? i-1 : Size-1);
}
// Check start is within valid range (0 ... size-1).
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::checkStart(const label start) const
{
  if (start < 0 || (start && unsigned(start) >= Size))
  {
    FATAL_ERROR_IN("FixedList<T, Size>::checkStart(const label)")
      << "start " << start << " out of range 0 ... " << (Size-1)
      << abort(FatalError);
  }
}
// Check size is within valid range (0 ... size).
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::checkSize(const label size) const
{
  if (size < 0 || unsigned(size) > Size)
  {
    FATAL_ERROR_IN("FixedList<T, Size>::checkSize(const label)")
      << "size " << size << " out of range 0 ... " << (Size)
      << abort(FatalError);
  }
}
// Check index i is within valid range (0 ... size-1)
// The check for zero-sized list is already done in static assert
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::checkIndex(const label i) const
{
  if (i < 0 || unsigned(i) >= Size)
  {
    FATAL_ERROR_IN("FixedList<T, Size>::checkIndex(const label)")
      << "index " << i << " out of range 0 ... " << (Size-1)
      << abort(FatalError);
  }
}
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::resize(const label s)
{
# ifdef FULLDEBUG
  checkSize(s);
# endif
}
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::setSize(const label s)
{
# ifdef FULLDEBUG
  checkSize(s);
# endif
}
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::transfer(const FixedList<T, Size>& lst)
{
  for (unsigned i=0; i<Size; i++)
  {
    v_[i] = lst[i];
  }
}
template<class T, unsigned Size>
inline const T*
mousse::FixedList<T, Size>::cdata() const
{
  return v_;
}
template<class T, unsigned Size>
inline T*
mousse::FixedList<T, Size>::data()
{
  return v_;
}
template<class T, unsigned Size>
inline T& mousse::FixedList<T, Size>::first()
{
  return v_[0];
}
template<class T, unsigned Size>
inline const T& mousse::FixedList<T, Size>::first() const
{
  return v_[0];
}
template<class T, unsigned Size>
inline T& mousse::FixedList<T, Size>::last()
{
  return v_[Size-1];
}
template<class T, unsigned Size>
inline const T& mousse::FixedList<T, Size>::last() const
{
  return v_[Size-1];
}
// Member Operators
// element access
template<class T, unsigned Size>
inline T& mousse::FixedList<T, Size>::operator[](const label i)
{
# ifdef FULLDEBUG
  checkIndex(i);
# endif
  return v_[i];
}
// const element access
template<class T, unsigned Size>
inline const T& mousse::FixedList<T, Size>::operator[](const label i) const
{
# ifdef FULLDEBUG
  checkIndex(i);
# endif
  return v_[i];
}
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::operator=(const T lst[Size])
{
  for (unsigned i=0; i<Size; i++)
  {
    v_[i] = lst[i];
  }
}
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::operator=(const UList<T>& lst)
{
  checkSize(lst.size());
  for (unsigned i=0; i<Size; i++)
  {
    v_[i] = lst[i];
  }
}
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::operator=(const SLList<T>& lst)
{
  checkSize(lst.size());
  label i = 0;
  for
  (
    typename SLList<T>::const_iterator iter = lst.begin();
    iter != lst.end();
    ++iter
  )
  {
    operator[](i++) = iter();
  }
}
template<class T, unsigned Size>
inline void mousse::FixedList<T, Size>::operator=(const T& t)
{
  for (unsigned i=0; i<Size; i++)
  {
    v_[i] = t;
  }
}

// STL Member Functions
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::iterator
mousse::FixedList<T, Size>::begin()
{
  return v_;
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::const_iterator
mousse::FixedList<T, Size>::begin() const
{
  return v_;
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::const_iterator
mousse::FixedList<T, Size>::cbegin() const
{
  return v_;
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::iterator
mousse::FixedList<T, Size>::end()
{
  return &v_[Size];
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::const_iterator
mousse::FixedList<T, Size>::end() const
{
  return &v_[Size];
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::const_iterator
mousse::FixedList<T, Size>::cend() const
{
  return &v_[Size];
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::iterator
mousse::FixedList<T, Size>::rbegin()
{
  return &v_[Size-1];
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::const_iterator
mousse::FixedList<T, Size>::rbegin() const
{
  return &v_[Size-1];
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::const_iterator
mousse::FixedList<T, Size>::crbegin() const
{
  return &v_[Size-1];
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::iterator
mousse::FixedList<T, Size>::rend()
{
  return &v_[-1];
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::const_iterator
mousse::FixedList<T, Size>::rend() const
{
  return &v_[-1];
}
template<class T, unsigned Size>
inline typename mousse::FixedList<T, Size>::const_iterator
mousse::FixedList<T, Size>::crend() const
{
  return &v_[-1];
}
template<class T, unsigned Size>
inline mousse::label mousse::FixedList<T, Size>::size() const
{
  return Size;
}
template<class T, unsigned Size>
inline mousse::label mousse::FixedList<T, Size>::max_size() const
{
  return Size;
}
template<class T, unsigned Size>
inline bool mousse::FixedList<T, Size>::empty() const
{
  return false;
}
template<class T, unsigned Size>
template<class HashT>
inline unsigned mousse::FixedList<T, Size>::Hash<HashT>::operator()
(
  const FixedList<T, Size>& lst,
  unsigned seed
) const
{
  if (contiguous<T>())
  {
    // hash directly
    return Hasher(lst.v_, sizeof(lst.v_), seed);
  }
  else
  {
    // hash incrementally
    unsigned val = seed;
    for (unsigned i=0; i<Size; i++)
    {
      val = HashT()(lst[i], val);
    }
    return val;
  }
}
