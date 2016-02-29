#ifndef UTILITIES_MISCELLANEOUS_HELP_HELP_TYPES_HELP_TYPE_HELP_TYPE_HPP_
#define UTILITIES_MISCELLANEOUS_HELP_HELP_TYPES_HELP_TYPE_HELP_TYPE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::helpType
// Description
//   Base class for foam help classes
// SourceFiles
//   help_type.cpp
//   help_type_new.cpp
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "arg_list.hpp"
namespace mousse
{
// forward declaration of classes
class fvMesh;
class helpType
{
protected:
  //- Return file path to the Doxygen sources (if available)
  fileName doxygenPath() const;
  //- Display the list of documentation options
  void displayDocOptions
  (
    const string& searchStr,
    const bool exactMatch
  ) const;
  //- Display the help documentation in a browser
  void displayDoc
  (
    const word& className,
    const string& searchStr,
    const bool exactMatch
  ) const;
public:
  //- Runtime type information
  TypeName("helpType");
  // Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    helpType,
    dictionary,
    (),
    ()
  );
  //- Constructor
  helpType();
  //- Selector
  static autoPtr<helpType> New(const word& helpTypeName);
  //- Destructor
  virtual ~helpType();
  // Member Functions
    //- Initialise - typically setting static variables,
    //  e.g. command line arguments
    virtual void init();
    //- Execute the help
    virtual void execute(const argList& args, const fvMesh& mesh) = 0;
};
}  // namespace mousse
#endif
