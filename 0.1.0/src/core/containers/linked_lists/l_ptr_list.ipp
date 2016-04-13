// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "l_ptr_list.hpp"


// Constructors 
template<class LListBase, class T>
mousse::LPtrList<LListBase, T>::LPtrList(const LPtrList<LListBase, T>& lst)
:
  LList<LListBase, T*>{}
{
  for (const_iterator iter = lst.begin(); iter != lst.end(); ++iter) {
    this->append(iter().clone().ptr());
  }
}


// Destructor 
template<class LListBase, class T>
mousse::LPtrList<LListBase, T>::~LPtrList()
{
  this->clear();
}


// Member Functions 
template<class LListBase, class T>
bool mousse::LPtrList<LListBase, T>::eraseHead()
{
  T* tPtr;
  if ((tPtr = this->removeHead())) {
    delete tPtr;
    return true;
  } else {
    return false;
  }
}


template<class LListBase, class T>
void mousse::LPtrList<LListBase, T>::clear()
{
  const label oldSize = this->size();
  for (label i=0; i<oldSize; ++i) {
    eraseHead();
  }
  LList<LListBase, T*>::clear();
}


template<class LListBase, class T>
void mousse::LPtrList<LListBase, T>::transfer(LPtrList<LListBase, T>& lst)
{
  clear();
  LList<LListBase, T*>::transfer(lst);
}


// Member Operators 
template<class LListBase, class T>
void mousse::LPtrList<LListBase, T>::operator=(const LPtrList<LListBase, T>& lst)
{
  clear();
  for (const_iterator iter = lst.begin(); iter != lst.end(); ++iter) {
    this->append(iter().clone().ptr());
  }
}

// Friend Operators 
#include "l_ptr_list_io.ipp"
