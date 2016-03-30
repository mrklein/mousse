#ifndef CORE_MEMORY_AUTO_PTR_HPP_
#define CORE_MEMORY_AUTO_PTR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::autoPtr
// Description
//   An auto-pointer similar to the STL auto_ptr but with automatic casting
//   to a reference to the type and with pointer allocation checking on access.

#include "error.hpp"
#include <cstddef>
#if defined(__GNUC__)
#include <typeinfo>
#endif


namespace mousse {

template<class T>
class autoPtr
{
  // Public data
    //- Pointer to object
    mutable T* ptr_;
public:
  // Constructors
    //- Store object pointer
    inline explicit autoPtr(T* = 0);
    //- Construct as copy by transferring pointer to this autoPtr and
    //  setting the arguments pointer to NULL
    inline autoPtr(const autoPtr<T>&);
    //- Construct either by transferring pointer or cloning. Should
    //  only be called with type that supports cloning.
    inline autoPtr(const autoPtr<T>&, const bool reUse);
  //- Destructor, delete object if pointer is not NULL
  inline ~autoPtr();
  // Member Functions
    // Check
      //- Return true if the autoPtr is empty (ie, no pointer set).
      inline bool empty() const;
      //- Return true if the autoPtr valid (ie, the pointer is set).
      inline bool valid() const;
    // Edit
      //- Return object pointer for reuse
      inline T* ptr();
      //- Set pointer to that given.
      //  If object pointer already set issue a FatalError.
      inline void set(T*);
      //- If object pointer already set, delete object and set to given
      //  pointer
      inline void reset(T* = 0);
      //- Delete object (if the pointer is valid) and set pointer to NULL.
      inline void clear();
    // Member operators
      //- Return reference to the object data
      inline T& operator()();
      //- Return const reference to the object data
      inline const T& operator()() const;
      //- Const cast to the underlying type reference
      inline operator const T&() const;
      //- Return object pointer
      inline T* operator->();
      //- Return const object pointer
      inline const T* operator->() const;
      //- Take over the object pointer from parameter
      inline void operator=(const autoPtr<T>&);
};
}  // namespace mousse


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
  if (reUse) {
    ptr_ = ap.ptr_;
    ap.ptr_ = 0;
  } else if (ap.valid()) {
    ptr_ = ap().clone().ptr();
  } else {
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
  if (ptr_) {
    FATAL_ERROR_IN("void mousse::autoPtr<T>::set(T*)")
      << "object of type " << typeid(T).name()
      << " already allocated"
      << abort(FatalError);
  }
  ptr_ = p;
}


template<class T>
inline void mousse::autoPtr<T>::reset(T* p)
{
  if (ptr_) {
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
  if (!ptr_) {
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
  if (!ptr_) {
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
  if (!ptr_) {
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
  if (this != &ap) {
    reset(const_cast<autoPtr<T>&>(ap).ptr());
  }
}

#endif
