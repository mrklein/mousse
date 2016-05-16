// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_correction_limiting.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace CorrectionLimitingMethods {

DEFINE_TYPE_NAME_AND_DEBUG(noCorrectionLimiting, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  CorrectionLimitingMethod,
  noCorrectionLimiting,
  dictionary
);

}
}


// Constructors 
mousse::CorrectionLimitingMethods::noCorrectionLimiting::noCorrectionLimiting
(
  const dictionary& dict
)
:
  CorrectionLimitingMethod{dict}
{}


mousse::CorrectionLimitingMethods::noCorrectionLimiting::noCorrectionLimiting
(
  const noCorrectionLimiting& cl
)
:
  CorrectionLimitingMethod{cl}
{}


// Destructor 
mousse::CorrectionLimitingMethods::noCorrectionLimiting::~noCorrectionLimiting()
{}


// Member Functions 
mousse::vector
mousse::CorrectionLimitingMethods::noCorrectionLimiting::limitedVelocity
(
  const vector /*uP*/,
  const vector dU,
  const vector /*uMean*/
) const
{
  return dU;
}

