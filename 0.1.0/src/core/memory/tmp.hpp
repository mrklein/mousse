// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tmp
// Description
//   A class for managing temporary objects
#ifndef tmp_hpp_
#define tmp_hpp_
#include "ref_count.hpp"
#include "error.hpp"
#include <cstddef>
#include <typeinfo>
namespace mousse
{
template<class T>
class tmp
{
  // Private data
    //- Flag for whether object is a temporary or a constant object
    bool isTmp_;
    //- Pointer to temporary object
    mutable T* ptr_;
    //- Const reference to constant object
    const T& ref_;
public:
  // Constructors
    //- Store object pointer
    inline explicit tmp(T* = 0);
    //- Store object const reference
    inline tmp(const T&);
    //- Construct copy and increment reference count
    inline tmp(const tmp<T>&);
    //- Construct copy transferring content of temporary if required
    inline tmp(const tmp<T>&, bool allowTransfer);
  //- Destructor, delete object when reference count == 0
  inline ~tmp();
  // Member Functions
    // Access
      //- Return true if this is really a temporary object
      inline bool isTmp() const;
      //- Return true if this temporary object empty,
      //  ie, a temporary without allocation
      inline bool empty() const;
      //- Is this temporary object valid,
      //  ie, it is a reference or a temporary that has been allocated
      inline bool valid() const;
    // Edit
      //- Return tmp pointer for reuse
      inline T* ptr() const;
      //- If object pointer points to valid object:
      //  delete object and set pointer to NULL
      inline void clear() const;
  // Member operators
    //- Dereference operator
    inline T& operator()();
    //- Const dereference operator
    inline const T& operator()() const;
    //- Const cast to the underlying type reference
    inline operator const T&() const;
    //- Return object pointer
    inline T* operator->();
    //- Return const object pointer
    inline const T* operator->() const;
    //- Assignment operator
    inline void operator=(const tmp<T>&);
};
}  // namespace mousse

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
#endif
