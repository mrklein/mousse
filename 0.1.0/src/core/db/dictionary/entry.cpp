// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "entry.hpp"
#include "dictionary.hpp"
#include "ostring_stream.hpp"


// Static Data Members
int mousse::entry::disableFunctionEntries
{
  mousse::debug::infoSwitch("disableFunctionEntries", 0)
};


// Constructors 
mousse::entry::entry(const keyType& keyword)
:
  IDLList<entry>::link{},
  keyword_{keyword}
{}


mousse::entry::entry(const entry& e)
:
  IDLList<entry>::link(),
  keyword_{e.keyword_}
{}


mousse::autoPtr<mousse::entry> mousse::entry::clone() const
{
  return clone(dictionary::null);
}


// Member Operators 
void mousse::entry::operator=(const entry& e)
{
  // check for assignment to self
  if (this == &e) {
    FATAL_ERROR_IN("entry::operator=(const entry&)")
      << "attempted assignment to self"
      << abort(FatalError);
  }
  keyword_ = e.keyword_;
}


bool mousse::entry::operator==(const entry& e) const
{
  if (keyword_ != e.keyword_) {
    return false;
  } else {
    OStringStream oss1;
    oss1 << *this;
    OStringStream oss2;
    oss2 << e;
    return oss1.str() == oss2.str();
  }
}


bool mousse::entry::operator!=(const entry& e) const
{
  return !operator==(e);
}

