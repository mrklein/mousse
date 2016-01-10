// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "uptr_list.hpp"

// Constructors 
template<class T>
mousse::UPtrList<T>::UPtrList()
:
  ptrs_{}
{}


template<class T>
mousse::UPtrList<T>::UPtrList(const label s)
:
  ptrs_{s, reinterpret_cast<T*>(0)}
{}


template<class T>
mousse::UPtrList<T>::UPtrList(const Xfer<UPtrList<T> >& lst)
{
  transfer(lst());
}


template<class T>
mousse::UPtrList<T>::UPtrList(UPtrList<T>& a, bool reUse)
:
  ptrs_{a.ptrs_, reUse}
{}


// Member Functions 
template<class T>
void mousse::UPtrList<T>::setSize(const label newSize)
{
  label oldSize = size();
  if (newSize <= 0)
  {
    clear();
  }
  else if (newSize < oldSize)
  {
    ptrs_.setSize(newSize);
  }
  else if (newSize > oldSize)
  {
    ptrs_.setSize(newSize);
    // set new elements to NULL
    for (label i=oldSize; i<newSize; i++)
    {
      ptrs_[i] = NULL;
    }
  }
}


template<class T>
void mousse::UPtrList<T>::clear()
{
  ptrs_.clear();
}


template<class T>
void mousse::UPtrList<T>::transfer(UPtrList<T>& a)
{
  ptrs_.transfer(a.ptrs_);
}


template<class T>
void mousse::UPtrList<T>::reorder(const labelUList& oldToNew)
{
  if (oldToNew.size() != size())
  {
    FATAL_ERROR_IN("UPtrList<T>::reorder(const labelUList&)")
      << "Size of map (" << oldToNew.size()
      << ") not equal to list size (" << size()
      << ")." << abort(FatalError);
  }
  List<T*> newPtrs_{ptrs_.size(), reinterpret_cast<T*>(0)};
  FOR_ALL(*this, i)
  {
    label newI = oldToNew[i];
    if (newI < 0 || newI >= size())
    {
      FATAL_ERROR_IN("UPtrList<T>::reorder(const labelUList&)")
        << "Illegal index " << newI << nl
        << "Valid indices are 0.." << size()-1
        << abort(FatalError);
    }
    if (newPtrs_[newI])
    {
      FATAL_ERROR_IN("UPtrList<T>::reorder(const labelUList&)")
        << "reorder map is not unique; element " << newI
        << " already set." << abort(FatalError);
    }
    newPtrs_[newI] = ptrs_[i];
  }
  FOR_ALL(newPtrs_, i)
  {
    if (!newPtrs_[i])
    {
      FATAL_ERROR_IN("UPtrList<T>::reorder(const labelUList&)")
        << "Element " << i << " not set after reordering." << nl
        << abort(FatalError);
    }
  }
  ptrs_.transfer(newPtrs_);
}
#include "uptr_list_io.cpp"
