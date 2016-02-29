#ifndef UTILITIES_MISCELLANEOUS_HELP_HELP_TYPES_HELP_FUNCTION_OBJECT_HELP_FUNCTION_OBJECT_HPP_
#define UTILITIES_MISCELLANEOUS_HELP_HELP_TYPES_HELP_FUNCTION_OBJECT_HELP_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::helpFunctionObject
// Description
//   This class provides help for functionObjects.  When no additional arguments
//   are given, the utility outputs all known function objects.
//   Usage:
//   \plaintable
//     \c -browse \<patch type\> | open documentation for object in browser
//   \endplaintable
// SeeAlso
//   mousse::helpType
//   mousse::functionObject
// SourceFiles
//   help_function_object.cpp
#include "help_type.hpp"
namespace mousse
{
namespace helpTypes
{
class helpFunctionObject
:
  public helpType
{
public:
  //- Runtime type information
  TypeName("helpFunctionObject");
  //- Constructor
  helpFunctionObject();
  //- Destructor
  virtual ~helpFunctionObject();
  // Member Functions
    //- Initialise - typically setting static variables,
    //  e.g. command line arguments
    virtual void init();
    //- Execute the help
    virtual void execute(const argList& args, const fvMesh& mesh);
};
}  // namespace helpTypes
}  // namespace mousse
#endif
