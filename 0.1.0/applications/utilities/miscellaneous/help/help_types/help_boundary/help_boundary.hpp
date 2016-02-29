#ifndef UTILITIES_MISCELLANEOUS_HELP_HELP_TYPES_HELP_BOUNDARY_HELP_BOUNDARY_HPP_
#define UTILITIES_MISCELLANEOUS_HELP_HELP_TYPES_HELP_BOUNDARY_HELP_BOUNDARY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::helpBoundary
// Description
//   This class provides help for boundary conditions (patch fields).  When no
//   additional arguments are given, the utility outputs all known boundary
//   conditions.
//   Usage:
//   \plaintable
//     \c -browse \<patch type\> | open documentation for patch in browser
//     \c -field \<name\>        | boundary conditions for a given field
//     \c -constraint            | list constraint boundary conditions
//     \c -fixedValue            | list \c fixedValue type boundary conditions
//   \endplaintable
// Note
//   To use the \c -fixedValue option, the \c -field option should also be used
// SeeAlso
//   mousse::helpType
//   mousse::fvPatchField
// SourceFiles
//   help_boundary.cpp
//   help_boundary_templates.cpp
#include "help_type.hpp"
#include "ioobject.hpp"
namespace mousse
{
namespace helpTypes
{
class helpBoundary
:
  public helpType
{
protected:
  // Protected Member Functions
    //- Return/output the available boundary conditions for fields of Type
    template<class Type>
    wordList fieldConditions(const IOobject& io, const bool write) const;
    //- Output the available fixed boundary conditions for fields of Type
    template<class Type>
    void fixedValueFieldConditions(const IOobject& io) const;
public:
  //- Runtime type information
  TypeName("helpBoundary");
  //- Constructor
  helpBoundary();
  //- Destructor
  virtual ~helpBoundary();
  // Member Functions
    //- Initialise - typically setting static variables,
    //  e.g. command line arguments
    virtual void init();
    //- Execute the help
    virtual void execute(const argList& args, const fvMesh& mesh);
};
}  // namespace helpTypes
}  // namespace mousse
#ifdef NoRepository
  #include "help_boundary_templates.cpp"
#endif
#endif
