// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_type.hpp"


// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(calcType, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(calcType, dictionary);


// Constructors 
mousse::calcType::calcType()
{}


// Destructor 
mousse::calcType::~calcType()
{}


// Protected Member Functions 
void mousse::calcType::init()
{
  // Do nothing
}


void mousse::calcType::preCalc
(
  const argList& /*args*/,
  const Time& /*runTime*/,
  const fvMesh& /*mesh*/
)
{
  // Do nothing
}


void mousse::calcType::calc
(
  const argList& /*args*/,
  const Time& /*runTime*/,
  const fvMesh& /*mesh*/
)
{
  // Do nothing
}


void mousse::calcType::postCalc
(
  const argList& /*args*/,
  const Time& /*runTime*/,
  const fvMesh& /*mesh*/
)
{
  // Do nothing
}


// Member Functions 
void mousse::calcType::tryInit()
{
  FatalIOError.throwExceptions();
  try {
    init();
  } catch(IOerror& err) {
    Warning << err << endl;
  }
}


void mousse::calcType::tryPreCalc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{
  FatalIOError.throwExceptions();
  try {
    preCalc(args, runTime, mesh);
  } catch(IOerror& err) {
    Warning << err << endl;
  }
}


void mousse::calcType::tryCalc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{
  FatalIOError.throwExceptions();
  try {
    calc(args, runTime, mesh);
  } catch(IOerror& err) {
    Warning << err << endl;
  }
}


void mousse::calcType::tryPostCalc
(
  const argList& args,
  const Time& runTime,
  const fvMesh& mesh
)
{
  FatalIOError.throwExceptions();
  try {
    postCalc(args, runTime, mesh);
  } catch(IOerror& err) {
    Warning << err << endl;
  }
}

