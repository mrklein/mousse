// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "help_boundary.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace helpTypes
  {
    defineTypeNameAndDebug(helpBoundary, 0);
    addNamedToRunTimeSelectionTable
    (
      helpType,
      helpBoundary,
      dictionary,
      boundary
    );
  }
}
// Constructors 
mousse::helpTypes::helpBoundary::helpBoundary()
{}
// Destructor 
mousse::helpTypes::helpBoundary::~helpBoundary()
{}
// Member Functions 
void mousse::helpTypes::helpBoundary::init()
{
  helpType::init();
  argList::validArgs.append("boundary");
  argList::addOption
  (
    "field",
    "word",
    "list available conditions for field"
  );
  argList::addBoolOption
  (
    "constraint",
    "list constraint patches"
  );
  argList::addBoolOption
  (
    "fixedValue",
    "list fixed value patches (use with -field option)"
  );
}
void mousse::helpTypes::helpBoundary::execute
(
  const argList& args,
  const fvMesh& mesh
)
{
  bool abortVar(env("FOAM_ABORT"));
  if (abortVar)
  {
    FatalErrorIn
    (
      "void mousse::helpTypes::helpBoundary::execute"
      "("
        "const argList&, "
        "const fvMesh&"
      ")"
    )
      << "Please unset FOAM_ABORT to use this utlity"
      << exit(FatalError);
  }
  word condition(word::null);
  word fieldName(word::null);
  if (args.optionReadIfPresent("browse", condition))
  {
    // TODO: strip scoping info if present?
    // e.g. conditions with leading "compressible::" will not be found
    // ".*[fF]vPatchField.*" + className + ".*"
    displayDoc(condition, ".*[fF]vPatchField.*", false);
  }
  else if (args.optionFound("constraint"))
  {
    HashSet<word> constraintTypes(fvPatch::constraintTypes());
    Info<< "Constraint types:" << nl;
    forAllConstIter(HashSet<word>, constraintTypes, iter)
    {
      Info<< "    " << iter.key() << nl;
    }
    Info<< endl;
  }
  else if (args.optionReadIfPresent("field", fieldName))
  {
    IOobject fieldHeader
    (
      fieldName,
      mesh.time().timeName(),
      mesh,
      IOobject::MUST_READ
    );
    if (fieldHeader.headerOk())
    {
      if (args.optionFound("fixedValue"))
      {
        fixedValueFieldConditions<scalar>(fieldHeader);
        fixedValueFieldConditions<vector>(fieldHeader);
        fixedValueFieldConditions<sphericalTensor>(fieldHeader);
        fixedValueFieldConditions<symmTensor>(fieldHeader);
        fixedValueFieldConditions<tensor>(fieldHeader);
      }
      else
      {
        (void)fieldConditions<scalar>(fieldHeader, true);
        (void)fieldConditions<vector>(fieldHeader, true);
        (void)fieldConditions<sphericalTensor>(fieldHeader, true);
        (void)fieldConditions<symmTensor>(fieldHeader, true);
        (void)fieldConditions<tensor>(fieldHeader, true);
      }
    }
    else
    {
      FatalErrorIn
      (
        "void mousse::helpTypes::helpBoundary::execute"
        "("
          "const argList&, "
          "const fvMesh&"
        ")"
      )
        << "Unable to read field " << fieldName << exit(FatalError);
    }
  }
  else if (args.optionReadIfPresent("fixedValue", fieldName))
  {
    FatalErrorIn
    (
      "void mousse::helpTypes::helpBoundary::execute"
      "("
        "const argList&, "
        "const fvMesh&"
      ")"
    )
      << "-field option must be specified when using the -fixedValue "
      << "option" << exit(FatalError);
  }
  else
  {
    // TODO: strip scoping info if present?
    // e.g. conditions with leading "compressible::" will not be found
    // ".*[fF]vPatchField.*" + className + ".*"
    displayDocOptions(".*[fF]vPatchField.*", false);
  }
}
