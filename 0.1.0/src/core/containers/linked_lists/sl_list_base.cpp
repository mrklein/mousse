// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "sl_list_base.hpp"
mousse::SLListBase::iterator mousse::SLListBase::endIter_
(
  const_cast<SLListBase&>(static_cast<const SLListBase&>(SLListBase()))
);
mousse::SLListBase::const_iterator mousse::SLListBase::endConstIter_
(
  static_cast<const SLListBase&>(SLListBase()),
  reinterpret_cast<const link*>(0)
);
// Member Functions 
void mousse::SLListBase::insert(SLListBase::link* a)
{
  nElmts_++;
  if (last_)
  {
    a->next_ = last_->next_;
  }
  else
  {
    last_ = a;
  }
  last_->next_ = a;
}
void mousse::SLListBase::append(SLListBase::link* a)
{
  nElmts_++;
  if (last_)
  {
    a->next_ = last_->next_;
    last_ = last_->next_ = a;
  }
  else
  {
    last_ = a->next_ = a;
  }
}
mousse::SLListBase::link* mousse::SLListBase::removeHead()
{
  nElmts_--;
  if (last_ == 0)
  {
    FATAL_ERROR_IN("SLListBase::remove()")
      << "remove from empty list"
      << abort(FatalError);
  }
  SLListBase::link* f = last_->next_;
  if (f == last_)
  {
    last_ = 0;
  }
  else
  {
    last_->next_ = f->next_;
  }
  return f;
}
mousse::SLListBase::link* mousse::SLListBase::remove(SLListBase::link* it)
{
  SLListBase::iterator iter = begin();
  SLListBase::link *prev = &(*iter);
  if (it == prev)
  {
    return removeHead();
  }
  nElmts_--;
  for (++iter; iter != end(); ++iter)
  {
    SLListBase::link *p = &(*iter);
    if (p == it)
    {
      prev->next_ = p->next_;
      if (p == last_)
      {
        last_ = prev;
      }
      return it;
    }
    prev = p;
  }
  return 0;
}
