// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mag_grad.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace calcTypes
  {
    defineTypeNameAndDebug(magGrad, 0);
    addToRunTimeSelectionTable(calcType, magGrad, dictionary);
  }
}
// Constructors 
mousse::calcTypes::magGrad::magGrad()
:
  calcType()
{}
// Destructor 
mousse::calcTypes::magGrad::~magGrad()
{}
// Member Functions 
void mousse::calcTypes::magGrad::init()
{
  argList::validArgs.append("magGrad");
  argList::validArgs.append("fieldName");
}
void mousse::calcTypes::magGrad::preCalc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{}
void mousse::calcTypes::magGrad::calc
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
    writeMagGradField<scalar>(fieldHeader, mesh, processed);
    writeMagGradField<vector>(fieldHeader, mesh, processed);
    if (!processed)
    {
      FatalError
        << "Unable to process " << fieldName << nl
        << "No call to magGrad for fields of type "
        << fieldHeader.headerClassName() << nl << nl
        << exit(FatalError);
    }
  }
  else
  {
    Info<< "    No " << fieldName << endl;
  }
}
