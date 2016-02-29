// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "help_function_object.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace helpTypes
  {
    defineTypeNameAndDebug(helpFunctionObject, 0);
    addNamedToRunTimeSelectionTable
    (
      helpType,
      helpFunctionObject,
      dictionary,
      functionObject
    );
  }
}
// Constructors 
mousse::helpTypes::helpFunctionObject::helpFunctionObject()
{}
// Destructor 
mousse::helpTypes::helpFunctionObject::~helpFunctionObject()
{}
// Member Functions 
void mousse::helpTypes::helpFunctionObject::init()
{
  helpType::init();
  argList::validArgs.append("functionObject");
}
void mousse::helpTypes::helpFunctionObject::execute
(
  const argList& args,
  const fvMesh& mesh
)
{
  word function(word::null);
  if (args.optionReadIfPresent("browse", function))
  {
    displayDoc(function, ".*[fF]unctionObject.*", true);
  }
  else
  {
    displayDocOptions(".*[fF]unctionObject.*", true);
  }
}
