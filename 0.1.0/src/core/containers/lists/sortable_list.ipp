// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list_ops.hpp"

// Constructors 
template<class T>
mousse::SortableList<T>::SortableList()
{}


template<class T>
mousse::SortableList<T>::SortableList(const UList<T>& values)
:
  List<T>{values}
{
  sort();
}


template<class T>
mousse::SortableList<T>::SortableList(const Xfer<List<T> >& values)
:
  List<T>{values}
{
  sort();
}


template<class T>
mousse::SortableList<T>::SortableList(const label size)
:
  List<T>{size}
{}


template<class T>
mousse::SortableList<T>::SortableList(const label size, const T& val)
:
  List<T>{size, val}
{}


template<class T>
mousse::SortableList<T>::SortableList(const SortableList<T>& lst)
:
  List<T>{lst},
  indices_{lst.indices()}
{}


// Member Functions 
template<class T>
void mousse::SortableList<T>::clear()
{
  List<T>::clear();
  indices_.clear();
}


template<class T>
mousse::List<T>& mousse::SortableList<T>::shrink()
{
  indices_.clear();
  return static_cast<List<T>&>(*this);
}


template<class T>
void mousse::SortableList<T>::sort()
{
  sortedOrder(*this, indices_);
  List<T> lst{this->size()};
  FOR_ALL(indices_, i) {
    lst[i] = this->operator[](indices_[i]);
  }
  List<T>::transfer(lst);
}


template<class T>
void mousse::SortableList<T>::reverseSort()
{
  sortedOrder(*this, indices_, typename UList<T>::greater(*this));
  List<T> lst{this->size()};
  FOR_ALL(indices_, i) {
    lst[i] = this->operator[](indices_[i]);
  }
  List<T>::transfer(lst);
}


template<class T>
mousse::Xfer<mousse::List<T> > mousse::SortableList<T>::xfer()
{
  return xferMoveTo<List<T> >(*this);
}


// Member Operators 
template<class T>
inline void mousse::SortableList<T>::operator=(const T& t)
{
  UList<T>::operator=(t);
}


template<class T>
inline void mousse::SortableList<T>::operator=(const UList<T>& rhs)
{
  List<T>::operator=(rhs);
  indices_.clear();
}


template<class T>
inline void mousse::SortableList<T>::operator=(const SortableList<T>& rhs)
{
  List<T>::operator=(rhs);
  indices_ = rhs.indices();
}
