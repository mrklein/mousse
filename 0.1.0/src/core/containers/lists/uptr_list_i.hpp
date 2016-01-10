// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
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
  ptr_(ptr)
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
