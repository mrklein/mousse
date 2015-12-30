// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "classname.hpp"
// Static Data Members
template<TemplateClassArgument>
const dataType mousse::CLASSNAME<TemplateArgument>::staticData();
// Static Member Functions
// Constructors 
template<TemplateClassArgument>
mousse::CLASSNAME<TemplateArgument>::CLASSNAME()
:
  baseClassName(),
  data_()
{}
template<TemplateClassArgument>
mousse::CLASSNAME<TemplateArgument>::CLASSNAME(const dataType& data)
:
  baseClassName(),
  data_(data)
{}
template<TemplateClassArgument>
mousse::CLASSNAME<TemplateArgument>::CLASSNAME
(
  const CLASSNAME<TemplateArgument>&
)
:
  baseCLASSNAME(),
  data_()
{}
// Selectors
template<TemplateClassArgument>
mousse::autoPtr<mousse::CLASSNAME<TemplateArgument> >
mousse::CLASSNAME<TemplateArgument>::New()
{
  return autoPtr<CLASSNAME<TemplateArgument> >
  (
    new CLASSNAME<TemplateArgument>
  );
}
// Destructor 
template<TemplateClassArgument>
mousse::CLASSNAME<TemplateArgument>::~CLASSNAME()
{}
// Private Member Functions 
// Protected Member Functions 
// Member Functions 
// Member Operators 
template<TemplateClassArgument>
void mousse::CLASSNAME<TemplateArgument>::operator=
(
  const CLASSNAME<TemplateArgument>& rhs
)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FatalErrorIn
    (
      "mousse::CLASSNAME<TemplateArgument>::operator="
      "(const mousse::CLASSNAME<TemplateArgument>&)"
    )   << "Attempted assignment to self"
      << abort(FatalError);
  }
}
// Friend Functions 
// Friend Operators
