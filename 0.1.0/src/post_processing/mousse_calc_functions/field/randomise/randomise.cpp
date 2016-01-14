// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "randomise.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace calcTypes
  {
    DEFINE_TYPE_NAME_AND_DEBUG(randomise, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE(calcType, randomise, dictionary);
  }
}
// Constructors 
mousse::calcTypes::randomise::randomise()
:
  calcType()
{}
// Destructor 
mousse::calcTypes::randomise::~randomise()
{}
// Member Functions 
void mousse::calcTypes::randomise::init()
{
  argList::validArgs.append("randomise");
  argList::validArgs.append("perturbation");
  argList::validArgs.append("fieldName");
}
void mousse::calcTypes::randomise::preCalc
(
  const argList& /*args*/,
  const Time& /*runTime*/,
  const fvMesh& /*mesh*/
)
{}
void mousse::calcTypes::randomise::calc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{
  const scalar pertMag = args.argRead<scalar>(2);
  const word fieldName = args[3];
  Random rand(1234567);
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
    writeRandomField<vector>
    (
      fieldHeader,
      pertMag,
      rand,
      mesh,
      processed
    );
    writeRandomField<sphericalTensor>
    (
      fieldHeader,
      pertMag,
      rand,
      mesh,
      processed
    );
    writeRandomField<symmTensor>
    (
      fieldHeader,
      pertMag,
      rand,
      mesh,
      processed
    );
    writeRandomField<tensor>
    (
      fieldHeader,
      pertMag,
      rand,
      mesh,
      processed
    );
    if (!processed)
    {
      FatalError
        << "Unable to process " << fieldName << nl
        << "No call to randomise for fields of type "
        << fieldHeader.headerClassName() << nl << nl
        << exit(FatalError);
    }
  }
  else
  {
    Info<< "    No " << fieldName << endl;
  }
}
