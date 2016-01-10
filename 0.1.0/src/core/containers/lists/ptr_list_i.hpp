// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "auto_ptr.hpp"
#include "tmp.hpp"
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
  autoPtr<T> old(ptrs_[i]);
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
  if (!ptrs_[i])
  {
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
  if (!ptrs_[i])
  {
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
  ptr_(ptr)
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
  ptr_(ptr)
{}
template<class T>
inline mousse::PtrList<T>::const_iterator::const_iterator(const iterator& iter)
:
  ptr_(iter.ptr_)
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
