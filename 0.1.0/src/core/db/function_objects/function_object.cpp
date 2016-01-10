// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "function_object.hpp"
#include "dictionary.hpp"
#include "dl_library_table.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{

DEFINE_DEBUG_SWITCH_WITH_NAME(functionObject, "functionObject", 0);
DEFINE_RUN_TIME_SELECTION_TABLE(functionObject, dictionary);

}
// Constructors 
mousse::functionObject::functionObject(const word& name)
:
  name_(name)
{}
// Selectors
mousse::autoPtr<mousse::functionObject> mousse::functionObject::New
(
  const word& name,
  const Time& t,
  const dictionary& functionDict
)
{
  const word functionType(functionDict.lookup("type"));
  if (debug)
  {
    Info<< "Selecting function " << functionType << endl;
  }
  const_cast<Time&>(t).libs().open
  (
    functionDict,
    "functionObjectLibs",
    dictionaryConstructorTablePtr_
  );
  if (!dictionaryConstructorTablePtr_)
  {
    FATAL_ERROR_IN
    (
      "functionObject::New"
      "(const word& name, const Time&, const dictionary&)"
    )   << "Unknown function type "
      << functionType << nl << nl
      << "Table of functionObjects is empty" << endl
      << exit(FatalError);
  }
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(functionType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "functionObject::New"
      "(const word& name, const Time&, const dictionary&)"
    )   << "Unknown function type "
      << functionType << nl << nl
      << "Valid functions are : " << nl
      << dictionaryConstructorTablePtr_->sortedToc() << endl
      << exit(FatalError);
  }
  return autoPtr<functionObject>(cstrIter()(name, t, functionDict));
}
// Destructor 
mousse::functionObject::~functionObject()
{}
// Member Functions 
const mousse::word& mousse::functionObject::name() const
{
  return name_;
}
bool mousse::functionObject::end()
{
  return execute(false);
}
bool mousse::functionObject::timeSet()
{
  return false;
}
bool mousse::functionObject::adjustTimeStep()
{
  return false;
}
mousse::autoPtr<mousse::functionObject> mousse::functionObject::iNew::operator()
(
  const word& name,
  Istream& is
) const
{
  dictionary dict(is);
  return functionObject::New(name, time_, dict);
}
