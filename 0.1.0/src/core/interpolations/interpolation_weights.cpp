// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation_weights.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"

namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(interpolationWeights, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(interpolationWeights, word);

// Constructors 
interpolationWeights::interpolationWeights
(
  const scalarField& samples
)
:
  samples_{samples}
{}

// Selectors
autoPtr<interpolationWeights> interpolationWeights::New
(
  const word& type,
  const scalarField& samples
)
{
  if (debug)
  {
    INFO_IN("interpolationWeights::New")
      << "Selecting interpolationWeights "
      << type << endl;
  }
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(type);
  if (cstrIter == wordConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "interpolationWeights::New(const word&, "
      "const scalarField&)"
    )
    << "Unknown interpolationWeights type "
    << type
    << endl << endl
    << "Valid interpolationWeights types are :" << endl
    << wordConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<interpolationWeights>(cstrIter()(samples));
}

// Destructor 
interpolationWeights::~interpolationWeights()
{}

}  // namespace mousse
