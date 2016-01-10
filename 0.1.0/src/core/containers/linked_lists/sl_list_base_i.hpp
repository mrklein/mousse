// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
// Constructors
inline mousse::SLListBase::link::link()
:
  next_(0)
{}
inline mousse::SLListBase::link::link(link* p)
:
  next_(p)
{}
inline mousse::SLListBase::SLListBase()
:
  last_(0),
  nElmts_(0)
{}
inline mousse::SLListBase::SLListBase(link* a)
:
  last_(a->next_ = a),
  nElmts_(1)
{}
// Destructor
inline mousse::SLListBase::~SLListBase()
{}
// Member Functions
inline mousse::label mousse::SLListBase::size() const
{
  return nElmts_;
}
inline bool mousse::SLListBase::empty() const
{
  return !nElmts_;
}
inline mousse::SLListBase::link*
mousse::SLListBase::first()
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("SLListBase::first()")
      << "list is empty"
      << abort(FatalError);
  }
  return last_->next_;
}
inline const mousse::SLListBase::link*
mousse::SLListBase::first() const
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("SLListBase::first() const")
      << "list is empty"
      << abort(FatalError);
  }
  return last_->next_;
}
inline mousse::SLListBase::link*
mousse::SLListBase::last()
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("SLListBase::last()")
      << "list is empty"
      << abort(FatalError);
  }
  return last_;
}
inline const mousse::SLListBase::link*
mousse::SLListBase::last() const
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("SLListBase::last() const")
      << "list is empty"
      << abort(FatalError);
  }
  return last_;
}
inline void mousse::SLListBase::clear()
{
  last_ = 0;
  nElmts_ = 0;
}
inline void mousse::SLListBase::transfer(SLListBase& lst)
{
  last_   = lst.last_;
  nElmts_ = lst.nElmts_;
  lst.clear();
}
inline mousse::SLListBase::link* mousse::SLListBase::remove
(
  SLListBase::iterator& it
)
{
  return remove(it.curElmt_);
}
// STL iterator
inline mousse::SLListBase::iterator::iterator(SLListBase& s, link* elmt)
:
  curList_(s),
  curElmt_(elmt),
  curLink_(*curElmt_)
{}
inline mousse::SLListBase::iterator::iterator(SLListBase& s)
:
  curList_(s),
  curElmt_(NULL),
  curLink_()
{}
inline void mousse::SLListBase::iterator::operator=(const iterator& iter)
{
  curElmt_ = iter.curElmt_;
  curLink_ = iter.curLink_;
}
inline bool mousse::SLListBase::iterator::operator==(const iterator& iter) const
{
  return curElmt_ == iter.curElmt_;
}
inline bool mousse::SLListBase::iterator::operator!=(const iterator& iter) const
{
  return curElmt_ != iter.curElmt_;
}
inline mousse::SLListBase::link& mousse::SLListBase::iterator::operator*()
{
  return *curElmt_;
}
inline mousse::SLListBase::iterator& mousse::SLListBase::iterator::operator++()
{
  if (curElmt_ == curList_.last_ || curList_.last_ == 0)
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
inline mousse::SLListBase::iterator
mousse::SLListBase::iterator::operator++(int)
{
  iterator tmp = *this;
  ++*this;
  return tmp;
}
inline mousse::SLListBase::iterator
mousse::SLListBase::begin()
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
inline const mousse::SLListBase::iterator&
mousse::SLListBase::end()
{
  return endIter_;
}

// STL const_iterator
inline mousse::SLListBase::const_iterator::const_iterator
(
  const SLListBase& s,
  const link* elmt
)
:
  curList_(s),
  curElmt_(elmt)
{}
inline mousse::SLListBase::const_iterator::const_iterator(const iterator& iter)
:
  curList_(iter.curList_),
  curElmt_(iter.curElmt_)
{}
inline void mousse::SLListBase::const_iterator::operator=
(
  const const_iterator& iter
)
{
  curElmt_ = iter.curElmt_;
}
inline bool mousse::SLListBase::const_iterator::operator==
(
  const const_iterator& iter
) const
{
  return curElmt_ == iter.curElmt_;
}
inline bool mousse::SLListBase::const_iterator::operator!=
(
  const const_iterator& iter
) const
{
  return curElmt_ != iter.curElmt_;
}
inline const mousse::SLListBase::link&
mousse::SLListBase::const_iterator::operator*()
{
  return *curElmt_;
}
inline mousse::SLListBase::const_iterator&
mousse::SLListBase::const_iterator::operator++()
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
inline mousse::SLListBase::const_iterator
mousse::SLListBase::const_iterator::operator++(int)
{
  const_iterator tmp = *this;
  ++*this;
  return tmp;
}
inline mousse::SLListBase::const_iterator
mousse::SLListBase::cbegin() const
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
inline const mousse::SLListBase::const_iterator&
mousse::SLListBase::cend() const
{
  return endConstIter_;
}
inline mousse::SLListBase::const_iterator
mousse::SLListBase::begin() const
{
  return this->cbegin();
}
inline const mousse::SLListBase::const_iterator&
mousse::SLListBase::end() const
{
  return endConstIter_;
}
