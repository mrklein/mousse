// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolate.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace calcTypes
  {
    defineTypeNameAndDebug(interpolate, 0);
    addToRunTimeSelectionTable(calcType, interpolate, dictionary);
  }
}
// Constructors 
mousse::calcTypes::interpolate::interpolate()
:
  calcType()
{}
// Destructor 
mousse::calcTypes::interpolate::~interpolate()
{}
// Member Functions 
void mousse::calcTypes::interpolate::init()
{
  mousse::argList::validArgs.append("interpolate");
  argList::validArgs.append("fieldName");
}
void mousse::calcTypes::interpolate::preCalc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{}
void mousse::calcTypes::interpolate::calc
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
    writeInterpolateField<scalar>(fieldHeader, mesh, processed);
    writeInterpolateField<vector>(fieldHeader, mesh, processed);
    writeInterpolateField<sphericalTensor>(fieldHeader, mesh, processed);
    writeInterpolateField<symmTensor>(fieldHeader, mesh, processed);
    writeInterpolateField<tensor>(fieldHeader, mesh, processed);
    if (!processed)
    {
      FatalError
        << "Unable to process " << fieldName << nl
        << "No call to interpolate for fields of type "
        << fieldHeader.headerClassName() << nl << nl
        << exit(FatalError);
    }
  }
  else
  {
    Info<< "    No " << fieldName << endl;
  }
}
