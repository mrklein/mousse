// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "map_distribute.hpp"
#include <typeinfo>

// Constructors
template<class T>
inline mousse::autoPtr<T>::autoPtr(T* p)
:
  ptr_{p}
{}


template<class T>
inline mousse::autoPtr<T>::autoPtr(const autoPtr<T>& ap)
:
  ptr_{ap.ptr_}
{
  ap.ptr_ = 0;
}


template<class T>
inline mousse::autoPtr<T>::autoPtr(const autoPtr<T>& ap, const bool reUse)
{
  if (reUse)
  {
    ptr_ = ap.ptr_;
    ap.ptr_ = 0;
  }
  else if (ap.valid())
  {
    ptr_ = ap().clone().ptr();
  }
  else
  {
    ptr_ = NULL;
  }
}


template<class T>
inline mousse::autoPtr<T>::~autoPtr()
{
  clear();
}


// Member Functions
template<class T>
inline bool mousse::autoPtr<T>::empty() const
{
  return !ptr_;
}


template<class T>
inline bool mousse::autoPtr<T>::valid() const
{
  return ptr_;
}


template<class T>
inline T* mousse::autoPtr<T>::ptr()
{
  T* ptr = ptr_;
  ptr_ = 0;
  return ptr;
}


template<class T>
inline void mousse::autoPtr<T>::set(T* p)
{
  if (ptr_)
  {
    FatalErrorIn("void mousse::autoPtr<T>::set(T*)")
      << "object of type " << typeid(T).name()
      << " already allocated"
      << abort(FatalError);
  }
  ptr_ = p;
}


template<class T>
inline void mousse::autoPtr<T>::reset(T* p)
{
  if (ptr_)
  {
    delete ptr_;
  }
  ptr_ = p;
}


template<class T>
inline void mousse::autoPtr<T>::clear()
{
  reset(0);
}


// Member Operators
template<class T>
inline T& mousse::autoPtr<T>::operator()()
{
  if (!ptr_)
  {
    FATAL_ERROR_IN("T& mousse::autoPtr<T>::operator()()")
      << "object of type " << typeid(T).name()
      << " is not allocated"
      << abort(FatalError);
  }
  return *ptr_;
}


template<class T>
inline const T& mousse::autoPtr<T>::operator()() const
{
  if (!ptr_)
  {
    FATAL_ERROR_IN("const T& mousse::autoPtr<T>::operator()() const")
      << "object of type " << typeid(T).name()
      << " is not allocated"
      << abort(FatalError);
  }
  return *ptr_;
}


template<class T>
inline mousse::autoPtr<T>::operator const T&() const
{
  return operator()();
}
template<class T>
inline T* mousse::autoPtr<T>::operator->()
{
  if (!ptr_)
  {
    FATAL_ERROR_IN("mousse::autoPtr<T>::operator->()")
      << "object of type " << typeid(T).name()
      << " is not allocated"
      << abort(FatalError);
  }
  return ptr_;
}


template<class T>
inline const T* mousse::autoPtr<T>::operator->() const
{
  return const_cast<autoPtr<T>&>(*this).operator->();
}


template<class T>
inline void mousse::autoPtr<T>::operator=(const autoPtr<T>& ap)
{
  if (this != &ap)
  {
    reset(const_cast<autoPtr<T>&>(ap).ptr());
  }
}
