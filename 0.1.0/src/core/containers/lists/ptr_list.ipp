// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "ptr_list.hpp"
#include "sl_ptr_list.hpp"


// Constructors 
template<class T>
mousse::PtrList<T>::PtrList()
:
  ptrs_{}
{}


template<class T>
mousse::PtrList<T>::PtrList(const label s)
:
  ptrs_{s, reinterpret_cast<T*>(0)}
{}


template<class T>
mousse::PtrList<T>::PtrList(const PtrList<T>& a)
:
  ptrs_{a.size()}
{
  FOR_ALL(*this, i) {
    ptrs_[i] = (a[i]).clone().ptr();
  }
}


template<class T>
template<class CloneArg>
mousse::PtrList<T>::PtrList(const PtrList<T>& a, const CloneArg& cloneArg)
:
  ptrs_{a.size()}
{
  FOR_ALL(*this, i) {
    ptrs_[i] = (a[i]).clone(cloneArg).ptr();
  }
}


template<class T>
mousse::PtrList<T>::PtrList(const Xfer<PtrList<T> >& lst)
{
  transfer(lst());
}


template<class T>
mousse::PtrList<T>::PtrList(PtrList<T>& a, bool reUse)
:
  ptrs_{a.size()}
{
  if (reUse) {
    FOR_ALL(*this, i) {
      ptrs_[i] = a.ptrs_[i];
      a.ptrs_[i] = NULL;
    }
    a.setSize(0);
  } else {
    FOR_ALL(*this, i) {
      ptrs_[i] = (a[i]).clone().ptr();
    }
  }
}


template<class T>
mousse::PtrList<T>::PtrList(const SLPtrList<T>& sll)
:
  ptrs_(sll.size())
{
  if (sll.size()) {
    label i = 0;
    for
    (
      typename SLPtrList<T>::const_iterator iter = sll.begin();
      iter != sll.end();
      ++iter
    ) {
      ptrs_[i++] = (iter()).clone().ptr();
    }
  }
}


// Destructor 
template<class T>
mousse::PtrList<T>::~PtrList()
{
  FOR_ALL(*this, i) {
    if (ptrs_[i]) {
      delete ptrs_[i];
    }
  }
}


// Member Functions 
template<class T>
void mousse::PtrList<T>::setSize(const label newSize)
{
  if (newSize < 0) {
    FATAL_ERROR_IN("PtrList<T>::setSize(const label)")
      << "bad set size " << newSize
      << " for type " << typeid(T).name()
      << abort(FatalError);
  }
  label oldSize = size();
  if (newSize == 0) {
    clear();
  } else if (newSize < oldSize) {
    label i;
    for (i=newSize; i<oldSize; i++) {
      if (ptrs_[i]) {
        delete ptrs_[i];
      }
    }
    ptrs_.setSize(newSize);
  } else { // newSize > oldSize 
    ptrs_.setSize(newSize);
    label i;
    for (i=oldSize; i<newSize; i++) {
      ptrs_[i] = NULL;
    }
  }
}


template<class T>
void mousse::PtrList<T>::clear()
{
  FOR_ALL(*this, i) {
    if (ptrs_[i]) {
      delete ptrs_[i];
    }
  }
  ptrs_.clear();
}


template<class T>
void mousse::PtrList<T>::transfer(PtrList<T>& a)
{
  clear();
  ptrs_.transfer(a.ptrs_);
}


template<class T>
void mousse::PtrList<T>::reorder(const labelUList& oldToNew)
{
  if (oldToNew.size() != size()) {
    FATAL_ERROR_IN("PtrList<T>::reorder(const labelUList&)")
      << "Size of map (" << oldToNew.size()
      << ") not equal to list size (" << size()
      << ") for type " << typeid(T).name()
      << abort(FatalError);
  }
  List<T*> newPtrs_{ptrs_.size(), reinterpret_cast<T*>(0)};
  FOR_ALL(*this, i) {
    label newI = oldToNew[i];
    if (newI < 0 || newI >= size()) {
      FATAL_ERROR_IN("PtrList<T>::reorder(const labelUList&)")
        << "Illegal index " << newI << nl
        << "Valid indices are 0.." << size()-1
        << " for type " << typeid(T).name()
        << abort(FatalError);
    }
    if (newPtrs_[newI]) {
      FATAL_ERROR_IN("PtrList<T>::reorder(const labelUList&)")
        << "reorder map is not unique; element " << newI
        << " already set for type " << typeid(T).name()
        << abort(FatalError);
    }
    newPtrs_[newI] = ptrs_[i];
  }
  FOR_ALL(newPtrs_, i) {
    if (!newPtrs_[i]) {
      FATAL_ERROR_IN("PtrList<T>::reorder(const labelUList&)")
        << "Element " << i << " not set after reordering with type "
        << typeid(T).name() << nl << abort(FatalError);
    }
  }
  ptrs_.transfer(newPtrs_);
}


// Member Operators 
template<class T>
mousse::PtrList<T>& mousse::PtrList<T>::operator=(const PtrList<T>& a)
{
  if (this == &a) {
    FATAL_ERROR_IN("PtrList<T>::operator=(const PtrList<T>&)")
      << "attempted assignment to self for type " << typeid(T).name()
      << abort(FatalError);
  }
  if (size() == 0) {
    setSize(a.size());
    FOR_ALL(*this, i) {
      ptrs_[i] = (a[i]).clone().ptr();
    }
  } else if (a.size() == size()) {
    FOR_ALL(*this, i) {
      (*this)[i] = a[i];
    }
  } else {
    FATAL_ERROR_IN("PtrList::operator=(const PtrList<T>&)")
      << "bad size: " << a.size()
      << " for type " << typeid(T).name()
      << abort(FatalError);
  }
  return *this;
}

#include "ptr_list_io.ipp"
