// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"

// Constructors 
inline mousse::DLListBase::link::link()
:
  prev_(0),
  next_(0)
{}
inline mousse::DLListBase::DLListBase()
:
  first_(0),
  last_(0),
  nElmts_(0)
{}
inline mousse::DLListBase::DLListBase(link* a)
:
  first_(a),
  last_(a),
  nElmts_(1)
{
  a->prev_ = a;
  a->next_ = a;
}
// Destructor 
inline mousse::DLListBase::~DLListBase()
{}
// Member Functions 
inline bool mousse::DLListBase::link::registered() const
{
  return prev_ != 0 && next_ != 0;
}
inline void mousse::DLListBase::link::deregister()
{
  prev_ = 0;
  next_ = 0;
}
inline mousse::label mousse::DLListBase::size() const
{
  return nElmts_;
}
inline bool mousse::DLListBase::empty() const
{
  return !nElmts_;
}
inline mousse::DLListBase::link*
mousse::DLListBase::first()
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("DLListBase::first()")
      << "list is empty"
      << abort(FatalError);
  }
  return first_;
}
inline const mousse::DLListBase::link*
mousse::DLListBase::first() const
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("DLListBase::first() const")
      << "list is empty"
      << abort(FatalError);
  }
  return first_;
}
inline mousse::DLListBase::link*
mousse::DLListBase::last()
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("DLListBase::last()")
      << "list is empty"
      << abort(FatalError);
  }
  return last_;
}
inline const mousse::DLListBase::link*
mousse::DLListBase::last() const
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("DLListBase::last() const")
      << "list is empty"
      << abort(FatalError);
  }
  return last_;
}
inline void mousse::DLListBase::clear()
{
  first_ = 0;
  last_  = 0;
  nElmts_ = 0;
}
inline void mousse::DLListBase::transfer(DLListBase& lst)
{
  first_  = lst.first_;
  last_   = lst.last_;
  nElmts_ = lst.nElmts_;
  lst.clear();
}
inline mousse::DLListBase::link*
mousse::DLListBase::remove
(
  DLListBase::iterator& it
)
{
  return remove(it.curElmt_);
}
inline mousse::DLListBase::link*
mousse::DLListBase::replace
(
  DLListBase::iterator& oldIter,
  DLListBase::link* newLink
)
{
  return replace(oldIter.curElmt_, newLink);
}
// STL iterator 
inline mousse::DLListBase::iterator::iterator(DLListBase& s, link* elmt)
:
  curList_(s),
  curElmt_(elmt),
  curLink_(*curElmt_)
{}
inline mousse::DLListBase::iterator::iterator(DLListBase& s)
:
  curList_(s),
  curElmt_(NULL),
  curLink_()
{}
inline void mousse::DLListBase::iterator::operator=(const iterator& iter)
{
  curElmt_ = iter.curElmt_;
  curLink_ = iter.curLink_;
}
inline bool mousse::DLListBase::iterator::operator==(const iterator& iter) const
{
  return curElmt_ == iter.curElmt_;
}
inline bool mousse::DLListBase::iterator::operator!=(const iterator& iter) const
{
  return curElmt_ != iter.curElmt_;
}
inline mousse::DLListBase::link&
mousse::DLListBase::iterator::operator*()
{
  return *curElmt_;
}
inline mousse::DLListBase::iterator&
mousse::DLListBase::iterator::operator++()
{
  // Check if the curElmt_ is the last element (if it points to itself)
  // or if the list is empty because the last element may have been removed
  if (curLink_.next_ == curElmt_ || curList_.last_ == 0)
  {
    curElmt_ = 0;
  }
  else
  {
    curElmt_ = curLink_.next_;
    curLink_ = *curElmt_;
  }
  return *this;
}
inline mousse::DLListBase::iterator
mousse::DLListBase::iterator::operator++(int)
{
  iterator tmp = *this;
  ++*this;
  return tmp;
}
inline mousse::DLListBase::iterator
mousse::DLListBase::begin()
{
  if (size())
  {
    return iterator(*this, first());
  }
  else
  {
    return endIter_;
  }
}
inline const mousse::DLListBase::iterator& mousse::DLListBase::end()
{
  return endIter_;
}
// STL const_iterator 
inline mousse::DLListBase::const_iterator::const_iterator
(
  const DLListBase& s,
  const link* elmt
)
:
  curList_(s),
  curElmt_(elmt)
{}
inline mousse::DLListBase::const_iterator::const_iterator(const iterator& iter)
:
  curList_(iter.curList_),
  curElmt_(iter.curElmt_)
{}
inline void mousse::DLListBase::const_iterator::operator=
(
  const const_iterator& iter
)
{
  curElmt_ = iter.curElmt_;
}
inline bool mousse::DLListBase::const_iterator::operator==
(
  const const_iterator& iter
) const
{
  return curElmt_ == iter.curElmt_;
}
inline bool mousse::DLListBase::const_iterator::operator!=
(
  const const_iterator& iter
) const
{
  return curElmt_ != iter.curElmt_;
}
inline const mousse::DLListBase::link&
mousse::DLListBase::const_iterator::operator*()
{
  return *curElmt_;
}
inline mousse::DLListBase::const_iterator&
mousse::DLListBase::const_iterator::operator++()
{
  if (curElmt_ == curList_.last_)
  {
    curElmt_ = 0;
  }
  else
  {
    curElmt_ = curElmt_->next_;
  }
  return *this;
}
inline mousse::DLListBase::const_iterator
mousse::DLListBase::const_iterator::operator++(int)
{
  const_iterator tmp = *this;
  ++*this;
  return tmp;
}
inline mousse::DLListBase::const_iterator
mousse::DLListBase::cbegin() const
{
  if (size())
  {
    return const_iterator(*this, first());
  }
  else
  {
    return endConstIter_;
  }
}
inline const mousse::DLListBase::const_iterator&
mousse::DLListBase::cend() const
{
  return endConstIter_;
}
inline mousse::DLListBase::const_iterator
mousse::DLListBase::begin() const
{
  return this->cbegin();
}
inline const mousse::DLListBase::const_iterator&
mousse::DLListBase::end() const
{
  return endConstIter_;
}
// STL const_reverse_iterator 
inline mousse::DLListBase::const_reverse_iterator::const_reverse_iterator
(
  const DLListBase& s,
  const link* elmt
)
:
  curList_(s),
  curElmt_(elmt)
{}
inline void mousse::DLListBase::const_reverse_iterator::operator=
(
  const const_reverse_iterator& iter
)
{
  curElmt_ = iter.curElmt_;
}
inline bool mousse::DLListBase::const_reverse_iterator::operator==
(
  const const_reverse_iterator& iter
) const
{
  return curElmt_ == iter.curElmt_;
}
inline bool mousse::DLListBase::const_reverse_iterator::operator!=
(
  const const_reverse_iterator& iter
) const
{
  return curElmt_ != iter.curElmt_;
}
inline const mousse::DLListBase::link&
mousse::DLListBase::const_reverse_iterator::operator*()
{
  return *curElmt_;
}
inline mousse::DLListBase::const_reverse_iterator&
mousse::DLListBase::const_reverse_iterator::operator++()
{
  if (curElmt_ == curList_.first_)
  {
    curElmt_ = 0;
  }
  else
  {
    curElmt_ = curElmt_->prev_;
  }
  return *this;
}
inline mousse::DLListBase::const_reverse_iterator
mousse::DLListBase::const_reverse_iterator::operator++(int)
{
  const_reverse_iterator tmp = *this;
  ++*this;
  return tmp;
}
inline mousse::DLListBase::const_reverse_iterator
mousse::DLListBase::crbegin() const
{
  if (size())
  {
    return const_reverse_iterator(*this, last());
  }
  else
  {
    return endConstRevIter_;
  }
}
inline const mousse::DLListBase::const_reverse_iterator&
mousse::DLListBase::crend() const
{
  return endConstRevIter_;
}
inline mousse::DLListBase::const_reverse_iterator
mousse::DLListBase::rbegin() const
{
  return this->crbegin();
}
inline const mousse::DLListBase::const_reverse_iterator&
mousse::DLListBase::rend() const
{
  return endConstRevIter_;
}
