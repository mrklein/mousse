// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "div.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace calcTypes
  {
    DEFINE_TYPE_NAME_AND_DEBUG(div, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE(calcType, div, dictionary);
  }
}
// Constructors 
mousse::calcTypes::div::div()
:
  calcType()
{}
// Destructor 
mousse::calcTypes::div::~div()
{}
// Member Functions 
void mousse::calcTypes::div::init()
{
  argList::validArgs.append("div");
  argList::validArgs.append("fieldName");
}
void mousse::calcTypes::div::preCalc
(
  const argList& /*args*/,
  const Time& /*runTime*/,
  const fvMesh& /*mesh*/
)
{}
void mousse::calcTypes::div::calc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{
  const word fieldName = args[2];
  IOobject fieldHeader
  (
    fieldName,
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  );
  // Check field exists
  if (fieldHeader.headerOk())
  {
    bool processed = false;
    writeDivField<surfaceScalarField>(fieldHeader, mesh, processed);
    writeDivField<volVectorField>(fieldHeader, mesh, processed);
    if (!processed)
    {
        FatalError
          << "Unable to process " << fieldName << nl
          << "No call to div for fields of type "
          << fieldHeader.headerClassName() << nl << nl
          << exit(FatalError);
    }
  }
  else
  {
    Info<< "    No " << fieldName << endl;
  }
}
