// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include <typeinfo>
// Constructors 
template<class T>
inline mousse::tmp<T>::tmp(T* tPtr)
:
  isTmp_(true),
  ptr_(tPtr),
  ref_(*tPtr)
{}
template<class T>
inline mousse::tmp<T>::tmp(const T& tRef)
:
  isTmp_(false),
  ptr_(0),
  ref_(tRef)
{}
template<class T>
inline mousse::tmp<T>::tmp(const tmp<T>& t)
:
  isTmp_(t.isTmp_),
  ptr_(t.ptr_),
  ref_(t.ref_)
{
  if (isTmp_)
  {
    if (ptr_)
    {
      ptr_->operator++();
    }
    else
    {
      FATAL_ERROR_IN("mousse::tmp<T>::tmp(const tmp<T>&)")
        << "attempted copy of a deallocated temporary"
        << " of type " << typeid(T).name()
        << abort(FatalError);
    }
  }
}
template<class T>
inline mousse::tmp<T>::tmp(const tmp<T>& t, bool allowTransfer)
:
  isTmp_(t.isTmp_),
  ptr_(t.ptr_),
  ref_(t.ref_)
{
  if (isTmp_)
  {
    if (allowTransfer)
    {
      const_cast<tmp<T>&>(t).ptr_ = 0;
    }
    else
    {
      if (ptr_)
      {
        ptr_->operator++();
      }
      else
      {
        FATAL_ERROR_IN
        (
          "mousse::tmp<T>::tmp(const tmp<T>&, bool allowTransfer)"
        )   << "attempted copy of a deallocated temporary"
          << " of type " << typeid(T).name()
          << abort(FatalError);
      }
    }
  }
}
template<class T>
inline mousse::tmp<T>::~tmp()
{
  if (isTmp_ && ptr_)
  {
    if (ptr_->okToDelete())
    {
      delete ptr_;
      ptr_ = 0;
    }
    else
    {
      ptr_->operator--();
    }
  }
}
// Member Functions 
template<class T>
inline bool mousse::tmp<T>::isTmp() const
{
  return isTmp_;
}
template<class T>
inline bool mousse::tmp<T>::empty() const
{
  return (isTmp_ && !ptr_);
}
template<class T>
inline bool mousse::tmp<T>::valid() const
{
  return (!isTmp_ || (isTmp_ && ptr_));
}
template<class T>
inline T* mousse::tmp<T>::ptr() const
{
  if (isTmp_)
  {
    if (!ptr_)
    {
      FATAL_ERROR_IN("mousse::tmp<T>::ptr() const")
        << "temporary of type " << typeid(T).name() << " deallocated"
        << abort(FatalError);
    }
    T* ptr = ptr_;
    ptr_ = 0;
    ptr->resetRefCount();
    return ptr;
  }
  else
  {
    return new T(ref_);
  }
}
template<class T>
inline void mousse::tmp<T>::clear() const
{
  if (isTmp_ && ptr_)  // skip this bit:  && ptr_->okToDelete())
  {
    delete ptr_;
    ptr_ = 0;
  }
}
// Member Operators 
template<class T>
inline T& mousse::tmp<T>::operator()()
{
  if (isTmp_)
  {
    if (!ptr_)
    {
      FATAL_ERROR_IN("T& mousse::tmp<T>::operator()()")
        << "temporary of type " << typeid(T).name() << " deallocated"
        << abort(FatalError);
    }
    return *ptr_;
  }
  else
  {
    // Note: const is cast away!
    // Perhaps there should be two refs, one for const and one for non const
    // and if the ref is actually const then you cannot return it here.
    //
    // Another possibility would be to store a const ref and a flag to say
    // whether the tmp was constructed with a const or a non-const argument.
    //
    // eg, enum refType { POINTER = 0, REF = 1, CONSTREF = 2 };
    return const_cast<T&>(ref_);
  }
}
template<class T>
inline const T& mousse::tmp<T>::operator()() const
{
  if (isTmp_)
  {
    if (!ptr_)
    {
      FATAL_ERROR_IN("const T& mousse::tmp<T>::operator()() const")
        << "temporary of type " << typeid(T).name() << " deallocated"
        << abort(FatalError);
    }
    return *ptr_;
  }
  else
  {
    return ref_;
  }
}
template<class T>
inline mousse::tmp<T>::operator const T&() const
{
  return operator()();
}
template<class T>
inline T* mousse::tmp<T>::operator->()
{
  if (isTmp_)
  {
    if (!ptr_)
    {
      FATAL_ERROR_IN("mousse::tmp<T>::operator->()")
        << "temporary of type " << typeid(T).name() << " deallocated"
        << abort(FatalError);
    }
    return ptr_;
  }
  else
  {
    return &const_cast<T&>(ref_);
  }
}
template<class T>
inline const T* mousse::tmp<T>::operator->() const
{
  return const_cast<tmp<T>&>(*this).operator->();
}
template<class T>
inline void mousse::tmp<T>::operator=(const tmp<T>& t)
{
  if (isTmp_ && ptr_)
  {
    if (ptr_->okToDelete())
    {
      delete ptr_;
      ptr_ = 0;
    }
    else
    {
      ptr_->operator--();
    }
  }
  if (t.isTmp_)
  {
    isTmp_ = true;
    ptr_ = t.ptr_;
    if (ptr_)
    {
      ptr_->operator++();
    }
    else
    {
      FATAL_ERROR_IN("mousse::tmp<T>::operator=(const tmp<T>&)")
        << "attempted copy of a deallocated temporary"
        << " of type " << typeid(T).name()
        << abort(FatalError);
    }
  }
  else
  {
    FATAL_ERROR_IN("mousse::tmp<T>::operator=(const tmp<T>&)")
      << "attempted to assign to a const reference to constant object"
      << " of type " << typeid(T).name()
      << abort(FatalError);
  }
}
