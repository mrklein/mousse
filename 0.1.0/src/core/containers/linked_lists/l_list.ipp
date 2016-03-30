// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "l_list.hpp"


// Constructors 
template<class LListBase, class T>
mousse::LList<LListBase, T>::LList(const LList<LListBase, T>& lst)
:
  LListBase{}
{
  for (const_iterator iter = lst.begin(); iter != lst.end(); ++iter) {
    this->append(iter());
  }
}


template<class LListBase, class T>
mousse::LList<LListBase, T>::~LList()
{
  this->clear();
}


// Member Functions 
template<class LListBase, class T>
void mousse::LList<LListBase, T>::clear()
{
  label oldSize = this->size();
  for (label i=0; i<oldSize; ++i) {
    this->removeHead();
  }
  LListBase::clear();
}


template<class LListBase, class T>
void mousse::LList<LListBase, T>::transfer(LList<LListBase, T>& lst)
{
  clear();
  LListBase::transfer(lst);
}


// Member Operators 
template<class LListBase, class T>
void mousse::LList<LListBase, T>::operator=(const LList<LListBase, T>& lst)
{
  this->clear();
  for (const_iterator iter = lst.begin(); iter != lst.end(); ++iter) {
    this->append(iter());
  }
}


// Friend Operators 
#include "l_list_io.ipp"

