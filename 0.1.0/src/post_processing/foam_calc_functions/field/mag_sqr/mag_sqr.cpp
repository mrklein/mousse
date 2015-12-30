// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mag_sqr.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace calcTypes
  {
    defineTypeNameAndDebug(magSqr, 0);
    addToRunTimeSelectionTable(calcType, magSqr, dictionary);
  }
}
// Constructors 
mousse::calcTypes::magSqr::magSqr()
:
  calcType()
{}
// Destructor 
mousse::calcTypes::magSqr::~magSqr()
{}
// Member Functions 
void mousse::calcTypes::magSqr::init()
{
  argList::validArgs.append("magSqr");
  argList::validArgs.append("fieldName");
}
void mousse::calcTypes::magSqr::preCalc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{}
void mousse::calcTypes::magSqr::calc
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
    writeMagSqrField<scalar>(fieldHeader, mesh, processed);
    writeMagSqrField<vector>(fieldHeader, mesh, processed);
    writeMagSqrField<sphericalTensor>(fieldHeader, mesh, processed);
    writeMagSqrField<symmTensor>(fieldHeader, mesh, processed);
    writeMagSqrField<tensor>(fieldHeader, mesh, processed);
    if (!processed)
    {
      FatalError
        << "Unable to process " << fieldName << nl
        << "No call to magSqr for fields of type "
        << fieldHeader.headerClassName() << nl << nl
       << exit(FatalError);
    }
  }
  else
  {
    Info<< "    No " << fieldName << endl;
  }
}
