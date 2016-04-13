// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_ranges.hpp"
#include "list_ops.hpp"


// Static Data Members
const mousse::labelRanges::const_iterator mousse::labelRanges::endIter_;


// Private Member Functions 
void mousse::labelRanges::insertBefore
(
  const label insert,
  const labelRange& range
)
{
  // insert via copying up
  label nElem = this->size();
  if (labelRange::debug) {
    Info <<"before insert "
      << nElem << " elements, insert at " << insert << nl
      << *this << endl;
  }
  ParentType::setSize(nElem+1);
  if (labelRange::debug) {
    Info <<"copy between " << nElem << " and " << insert << nl;
  }
  for (label i = nElem-1; i >= insert; --i) {
    if (labelRange::debug) {
      Info <<"copy from " << (i) << " to " << (i+1) << nl;
    }
    ParentType::operator[](i+1) = ParentType::operator[](i);
  }
  // finally insert the range
  if (labelRange::debug) {
    Info << "finally insert the range at " << insert << nl;
  }
  ParentType::operator[](insert) = range;
}


void mousse::labelRanges::purgeEmpty()
{
  // purge empty ranges by copying down
  label nElem = 0;
  FOR_ALL(*this, elemI)
  {
    if (!ParentType::operator[](elemI).empty()) {
      if (nElem != elemI) {
        ParentType::operator[](nElem) = ParentType::operator[](elemI);
      }
      ++nElem;
    }
  }
  // truncate
  this->ParentType::setSize(nElem);
}


mousse::Ostream& mousse::labelRanges::printRange
(
  Ostream& os,
  const labelRange& range
) const
{
  if (range.empty()) {
    os << "empty";
  } else {
    os << range << " = " << range.first() << ":" << range.last();
  }
  return os;
}


// Constructors 
mousse::labelRanges::labelRanges(Istream& is)
{
  is >> *this;
}


// Member Functions 
bool mousse::labelRanges::add(const labelRange& range)
{
  if (range.empty()) {
    return false;
  } else if (this->empty()) {
    this->append(range);
    return true;
  }
  // find the correct place for insertion
  FOR_ALL(*this, elemI)
  {
    labelRange& currRange = ParentType::operator[](elemI);
    if (currRange.intersects(range, true)) {
      // absorb into the existing (adjacent/overlapping) range
      currRange += range;
      // might connect with the next following range(s)
      for (; elemI < this->size()-1; ++elemI) {
        labelRange& nextRange = ParentType::operator[](elemI+1);
        if (currRange.intersects(nextRange, true)) {
          currRange += nextRange;
          nextRange.clear();
        } else {
          break;
        }
      }
      // done - remove any empty ranges that might have been created
      purgeEmpty();
      return true;
      break;
    } else if (range < currRange) {
      insertBefore(elemI, range);
      return true;
      break;
    }
  }
  // not found: simply append
  this->append(range);
  return true;
}


bool mousse::labelRanges::remove(const labelRange& range)
{
  bool status = false;
  if (range.empty() || this->empty()) {
    return status;
  }
  FOR_ALL(*this, elemI)
  {
    labelRange& currRange = ParentType::operator[](elemI);
    if (range.first() > currRange.first()) {
      if (range.last() < currRange.last()) {
        // removal of range fragments of currRange
        if (labelRange::debug) {
          Info <<"Fragment removal ";
          printRange(Info, range) << " from ";
          printRange(Info, currRange) << endl;
        }
        // left-hand-side fragment: insert before current range
        label lower = currRange.first();
        label upper = range.first() - 1;
        labelRange fragment(lower, upper - lower + 1);
        // right-hand-side fragment
        lower = range.last() + 1;
        upper = currRange.last();
        currRange = labelRange(lower, upper - lower + 1);
        status = true;
        insertBefore(elemI, fragment);
        if (labelRange::debug) {
          Info << "fragment ";
          printRange(Info, fragment) << endl;
          Info << "yields ";
          printRange(Info, currRange) << endl;
        }
        // fragmentation can only affect a single range
        // thus we are done
        break;
      } else if (range.first() <= currRange.last()) {
        // keep left-hand-side, remove right-hand-side
        if (labelRange::debug) {
          Info << "RHS removal ";
          printRange(Info, range) << " from ";
          printRange(Info, currRange) << endl;
        }
        const label lower = currRange.first();
        const label upper = range.first() - 1;
        currRange = labelRange(lower, upper - lower + 1);
        status = true;
        if (labelRange::debug) {
          Info << "yields ";
          printRange(Info, currRange) << endl;
        }
      }
    } else if (range.first() <= currRange.first()) {
      if (range.last() >= currRange.first()) {
        // remove left-hand-side, keep right-hand-side
        if (labelRange::debug) {
          Info << "LHS removal ";
          printRange(Info, range) << " from ";
          printRange(Info, currRange) << endl;
        }
        const label lower = range.last() + 1;
        const label upper = currRange.last();
        currRange = labelRange(lower, upper - lower + 1);
        status = true;
        if (labelRange::debug) {
          Info << "yields ";
          printRange(Info, currRange) << endl;
        }
      }
    }
  }
  purgeEmpty();
  return status;
}


// Friend Operators 
mousse::Istream& mousse::operator>>(Istream& is, labelRanges& ranges)
{
  is >> static_cast<labelRanges::ParentType&>(ranges);
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const labelRanges& ranges)
{
  os << static_cast<const labelRanges::ParentType&>(ranges);
  return os;
}
