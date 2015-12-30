// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "relative.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
namespace CorrectionLimitingMethods
{
  defineTypeNameAndDebug(relative, 0);
  addToRunTimeSelectionTable
  (
    CorrectionLimitingMethod,
    relative,
    dictionary
  );
}
}
// Constructors 
mousse::CorrectionLimitingMethods::relative::relative(const dictionary& dict)
:
  CorrectionLimitingMethod(dict),
  e_(readScalar(dict.lookup("e")))
{}
mousse::CorrectionLimitingMethods::relative::relative(const relative& cl)
:
  CorrectionLimitingMethod(cl),
  e_(cl.e_)
{}
// Destructor 
mousse::CorrectionLimitingMethods::relative::~relative()
{}
// Member Functions 
mousse::vector mousse::CorrectionLimitingMethods::relative::limitedVelocity
(
  const vector uP,
  const vector dU,
  const vector uMean
) const
{
  const vector uRelative = uP - uMean;
  return minMod
  (
    dU,
   - (1.0 + this->e_)*uRelative
  );
}
