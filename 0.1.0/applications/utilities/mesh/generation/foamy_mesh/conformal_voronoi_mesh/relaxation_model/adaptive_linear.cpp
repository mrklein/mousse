// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "adaptive_linear.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(adaptiveLinear, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(relaxationModel, adaptiveLinear, dictionary);


// Constructors 
adaptiveLinear::adaptiveLinear
(
  const dictionary& relaxationDict,
  const Time& runTime
)
:
  relaxationModel{typeName, relaxationDict, runTime},
  relaxationStart_{readScalar(coeffDict().lookup("relaxationStart"))},
  relaxationEnd_{readScalar(coeffDict().lookup("relaxationEnd"))},
  lastTimeValue_{runTime_.time().timeOutputValue()},
  relaxation_{relaxationStart_}
{}


// Member Functions 
scalar adaptiveLinear::relaxation()
{
  if (runTime_.time().timeOutputValue() > lastTimeValue_) {
    scalar currentRelaxation = relaxation_;
    relaxation_ -=
      (relaxation_ - relaxationEnd_)
      /(
        (
          runTime_.time().endTime().value() - runTime_.time().timeOutputValue()
        )
        /(runTime_.time().timeOutputValue() - lastTimeValue_)
        + 1
      );
    lastTimeValue_ = runTime_.time().timeOutputValue();
    return currentRelaxation;
  }
  return relaxation_;
}

}  // namespace mousse

