// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "classname.hpp"
// Static Data Members
const dataType mousse::CLASSNAME::staticData();
// Static Member Functions
// Private Member Functions 
// Protected Member Functions 
// Constructors 
mousse::CLASSNAME::CLASSNAME()
:
  baseClassName(),
  data_()
{}
mousse::CLASSNAME::CLASSNAME(const dataType& data)
:
  baseClassName(),
  data_(data)
{}
mousse::CLASSNAME::CLASSNAME(const CLASSNAME&)
:
  baseClassName(),
  data_()
{}
// Selectors
mousse::autoPtr<mousse::CLASSNAME>
mousse::CLASSNAME::New()
{
  return autoPtr<CLASSNAME>(new CLASSNAME);
}
// Destructor 
mousse::CLASSNAME::~CLASSNAME()
{}
// Member Functions 
// Member Operators 
void mousse::CLASSNAME::operator=(const CLASSNAME& rhs)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FatalErrorIn("mousse::CLASSNAME::operator=(const mousse::CLASSNAME&)")
      << "Attempted assignment to self"
      << abort(FatalError);
  }
}
// Friend Functions 
// Friend Operators
