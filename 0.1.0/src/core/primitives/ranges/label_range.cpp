// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_range.hpp"
#include "token.hpp"


// Static Data Members

const mousse::labelRange::const_iterator mousse::labelRange::endIter_;
int mousse::labelRange::debug(::mousse::debug::debugSwitch("labelRange", 0));

// Constructors 
mousse::labelRange::labelRange(Istream& is)
:
  start_{0},
  size_{0}
{
  is >> *this;
}


// Member Functions 
bool mousse::labelRange::intersects
(
  const labelRange& range,
  const bool touches
) const
{
  label final = touches ? 1 : 0;
  return
  (
    this->size() && range.size()
    && ((range.first() >= this->first()
         && range.first() <= this->last() + final)
        || (this->first() >= range.first()
            && this->first() <= range.last() + final)));
}


mousse::labelRange mousse::labelRange::join(const labelRange& range) const
{
  // trivial cases first
  if (!size_) {
    return *this;
  } else if (!range.size_) {
    return range;
  }
  const label lower = mousse::min(this->first(), range.first());
  const label upper = mousse::max(this->last(),  range.last());
  const label sz = upper - lower + 1;
  return {lower, sz};
}


// Member Operators 
mousse::labelRange& mousse::labelRange::operator+=(const labelRange& rhs)
{
  if (!size_) {
    // trivial case
    operator=(rhs);
  } else if (rhs.size_) {
    const label lower = mousse::min(this->first(), rhs.first());
    const label upper = mousse::max(this->last(),  rhs.last());
    start_ = lower;
    size_  = upper - lower + 1;
  }
  return *this;
}


// Friend Operators 
mousse::Istream& mousse::operator>>(Istream& is, labelRange& range)
{
  is.readBegin("labelRange");
  is >> range.start_ >> range.size_;
  is.readEnd("labelRange");
  is.check("operator>>(Istream&, labelRange&)");
  // disallow invalid sizes
  if (range.size_ <= 0) {
    range.clear();
  }
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const labelRange& range)
{
  os << token::BEGIN_LIST
    << range.start_ << token::SPACE << range.size_
    << token::END_LIST;
  os.check("operator<<(Ostream&, const labelRange&)");
  return os;
}

