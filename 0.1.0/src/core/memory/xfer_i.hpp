// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "null_object.hpp"
// Static Member Functions
template<class T>
inline const mousse::Xfer<T>& mousse::Xfer<T>::null()
{
  return NullObjectRef<Xfer<T> >();
}
// Constructors 
template<class T>
inline mousse::Xfer<T>::Xfer(T* p)
:
  ptr_(p ? p : new T)
{}
template<class T>
inline mousse::Xfer<T>::Xfer(T& t, bool allowTransfer)
:
  ptr_(new T)
{
  if (allowTransfer)
  {
    ptr_->transfer(t);
  }
  else
  {
    ptr_->operator=(t);
  }
}
template<class T>
inline mousse::Xfer<T>::Xfer(const T& t)
:
  ptr_(new T)
{
  ptr_->operator=(t);
}
template<class T>
inline mousse::Xfer<T>::Xfer(const Xfer<T>& t)
:
  ptr_(new T)
{
  ptr_->transfer(*(t.ptr_));
}
// Destructor 
template<class T>
inline mousse::Xfer<T>::~Xfer()
{
  delete ptr_;
  ptr_ = 0;
}
//  Member Functions
// Member Operators 
template<class T>
inline void mousse::Xfer<T>::operator=(T& t)
{
  ptr_->transfer(t);
}
template<class T>
inline void mousse::Xfer<T>::operator=(const Xfer<T>& t)
{
  // silently ignore attempted copy to self
  if (this != &t)
  {
    ptr_->transfer(*(t.ptr_));
  }
}
template<class T>
inline T& mousse::Xfer<T>::operator()() const
{
  return *ptr_;
}
template<class T>
inline T* mousse::Xfer<T>::operator->() const
{
  return ptr_;
}
//  Helper Functions
template<class T>
inline mousse::Xfer<T> mousse::xferCopy(const T& t)
{
  return mousse::Xfer<T>(t);
}
template<class T>
inline mousse::Xfer<T> mousse::xferMove(T& t)
{
  return mousse::Xfer<T>(t, true);
}
template<class T>
inline mousse::Xfer<T> mousse::xferTmp(mousse::tmp<T>& tt)
{
  return mousse::Xfer<T>(tt(), tt.isTmp());
}
template<class To, class From>
inline mousse::Xfer<To> mousse::xferCopyTo(const From& t)
{
  mousse::Xfer<To> xf;
  xf() = t;
  return xf;
}
template<class To, class From>
inline mousse::Xfer<To> mousse::xferMoveTo(From& t)
{
  mousse::Xfer<To> xf;
  xf().transfer(t);
  return xf;
}
