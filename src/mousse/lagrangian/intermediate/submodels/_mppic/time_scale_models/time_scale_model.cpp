// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_scale_model.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(TimeScaleModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(TimeScaleModel, dictionary);

}


// Constructors 
mousse::TimeScaleModel::TimeScaleModel
(
  const dictionary& dict
)
:
  alphaPacked_{readScalar(dict.lookup("alphaPacked"))},
  e_{readScalar(dict.lookup("e"))}
{}


mousse::TimeScaleModel::TimeScaleModel
(
  const TimeScaleModel& cm
)
:
  alphaPacked_{cm.alphaPacked_},
  e_{cm.e_}
{}


// Selectors
mousse::autoPtr<mousse::TimeScaleModel> mousse::TimeScaleModel::New
(
  const dictionary& dict
)
{
  word modelType{dict.lookup("type")};
  Info << "Selecting time scale model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "TimeScaleModel::New"
      "("
        "const dictionary&"
      ")"
    )
    << "Unknown time scale model type " << modelType
    << ", constructor not in hash table" << nl << nl
    << "    Valid time scale model types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << abort(FatalError);
  }
  return autoPtr<TimeScaleModel>{cstrIter()(dict)};
}


// Destructor 
mousse::TimeScaleModel::~TimeScaleModel()
{}

