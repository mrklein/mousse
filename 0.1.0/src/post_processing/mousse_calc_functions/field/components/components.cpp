// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "components.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace calcTypes
  {
    DEFINE_TYPE_NAME_AND_DEBUG(components, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE(calcType, components, dictionary);
  }
}
// Constructors 
mousse::calcTypes::components::components()
:
  calcType()
{}
// Destructor 
mousse::calcTypes::components::~components()
{}
// Member Functions 
void mousse::calcTypes::components::init()
{
  argList::validArgs.append("components");
  argList::validArgs.append("fieldName");
}
void mousse::calcTypes::components::preCalc
(
  const argList& /*args*/,
  const Time& /*runTime*/,
  const fvMesh& /*mesh*/
)
{}
void mousse::calcTypes::components::calc
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
    writeComponentFields<vector>(fieldHeader, mesh, processed);
    writeComponentFields<sphericalTensor>(fieldHeader, mesh, processed);
    writeComponentFields<symmTensor>(fieldHeader, mesh, processed);
    writeComponentFields<tensor>(fieldHeader, mesh, processed);
    if (!processed)
    {
      FatalError
        << "Unable to process " << fieldName << nl
        << "No call to components for fields of type "
        << fieldHeader.headerClassName() << nl << nl
        << exit(FatalError);
    }
  }
  else
  {
    Info<< "    No " << fieldName << endl;
  }
}
