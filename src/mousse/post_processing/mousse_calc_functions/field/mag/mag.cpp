// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mag.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace calcTypes {

DEFINE_TYPE_NAME_AND_DEBUG(mag, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(calcType, mag, dictionary);

}
}


// Constructors 
mousse::calcTypes::mag::mag()
:
  calcType{}
{}


// Destructor 
mousse::calcTypes::mag::~mag()
{}


// Member Functions 
void mousse::calcTypes::mag::init()
{
  argList::validArgs.append("mag");
  argList::validArgs.append("fieldName");
}


void mousse::calcTypes::mag::preCalc
(
  const argList& /*args*/,
  const Time& /*runTime*/,
  const fvMesh& /*mesh*/
)
{}


void mousse::calcTypes::mag::calc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{
  const word fieldName = args[2];
  IOobject fieldHeader
  {
    fieldName,
    runTime.timeName(),
    mesh,
    IOobject::MUST_READ
  };
  // Check field exists
  if (fieldHeader.headerOk()) {
    bool processed = false;
    writeMagField<scalar>(fieldHeader, mesh, processed);
    writeMagField<vector>(fieldHeader, mesh, processed);
    writeMagField<sphericalTensor>(fieldHeader, mesh, processed);
    writeMagField<symmTensor>(fieldHeader, mesh, processed);
    writeMagField<tensor>(fieldHeader, mesh, processed);
    if (!processed) {
      FatalError
        << "Unable to process " << fieldName << nl
        << "No call to mag for fields of type "
        << fieldHeader.headerClassName() << nl << nl
        << exit(FatalError);
    }
  } else {
    Info << "    No " << fieldName << endl;
  }
}

