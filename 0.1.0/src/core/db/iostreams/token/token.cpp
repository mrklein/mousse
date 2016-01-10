// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "token.hpp"
#include "error.hpp"

// Static Data Members
namespace mousse
{
const char* const token::typeName = "token";
token token::undefinedToken;
typedef token::compound tokenCompound;
DEFINE_TYPE_NAME_AND_DEBUG(tokenCompound, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(tokenCompound, Istream);
}

// Private Member Functions
void mousse::token::parseError(const char* expected) const
{
  FatalIOError
    << "Parse error, expected a " << expected
    << ", found \n    " << info() << endl;
}

// Destructor
mousse::token::compound::~compound()
{}


// Selectors
mousse::autoPtr<mousse::token::compound> mousse::token::compound::New
(
  const word& compoundType,
  Istream& is
)
{
  IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(compoundType);
  if (cstrIter == IstreamConstructorTablePtr_->end())
  {
    FATAL_IO_ERROR_IN("token::compound::New(const word&, Istream&)", is)
      << "Unknown compound type " << compoundType << nl << nl
      << "Valid compound types:" << endl
      << IstreamConstructorTablePtr_->sortedToc()
      << abort(FatalIOError);
  }
  return autoPtr<mousse::token::compound>(cstrIter()(is));
}


// Member Functions
bool mousse::token::compound::isCompound(const word& name)
{
  return
  (
    IstreamConstructorTablePtr_
    && IstreamConstructorTablePtr_->found(name)
  );
}


mousse::token::compound& mousse::token::transferCompoundToken(const Istream& is)
{
  if (type_ == COMPOUND)
  {
    if (compoundTokenPtr_->empty())
    {
      FATAL_IO_ERROR_IN
      (
        "token::transferCompoundToken(const Istream& is)",
        is
      )
      << "compound has already been transfered from token\n    "
      << info() << abort(FatalIOError);
    }
    else
    {
      compoundTokenPtr_->empty() = true;
    }
    return *compoundTokenPtr_;
  }
  else
  {
    parseError("compound");
    return *compoundTokenPtr_;
  }
}
