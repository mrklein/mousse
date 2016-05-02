// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "absolute.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace CorrectionLimitingMethods {

DEFINE_TYPE_NAME_AND_DEBUG(absolute, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  CorrectionLimitingMethod,
  absolute,
  dictionary
);

}
}


// Constructors 
mousse::CorrectionLimitingMethods::absolute::absolute(const dictionary& dict)
:
  CorrectionLimitingMethod{dict},
  e_{readScalar(dict.lookup("e"))}
{}


mousse::CorrectionLimitingMethods::absolute::absolute(const absolute& cl)
:
  CorrectionLimitingMethod{cl},
  e_{cl.e_}
{}


// Destructor 
mousse::CorrectionLimitingMethods::absolute::~absolute()
{}


// Member Functions 
mousse::vector mousse::CorrectionLimitingMethods::absolute::limitedVelocity
(
  const vector uP,
  const vector dU,
  const vector uMean
) const
{
  const vector uRelative = uP - uMean;
  return
    minMod
    (
      dU,
      -(1.0 + this->e_)*uRelative*mag(uP)/max(mag(uRelative), SMALL)
    );
}

