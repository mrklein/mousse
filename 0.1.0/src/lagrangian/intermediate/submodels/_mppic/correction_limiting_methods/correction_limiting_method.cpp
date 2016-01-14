// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "correction_limiting_method.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(CorrectionLimitingMethod, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(CorrectionLimitingMethod, dictionary);
}
// Constructors 
mousse::CorrectionLimitingMethod::CorrectionLimitingMethod
(
  const dictionary&
)
{}
mousse::CorrectionLimitingMethod::CorrectionLimitingMethod
(
  const CorrectionLimitingMethod& /*cl*/
)
{}
// Selectors
mousse::autoPtr<mousse::CorrectionLimitingMethod>
mousse::CorrectionLimitingMethod::New
(
  const dictionary& dict
)
{
  word modelType(dict.lookup("type"));
  Info<< "Selecting correction limiter " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "CorrectionLimitingMethod::New"
      "("
        "const dictionary&"
      ")"
    )   << "Unknown correction limiter type " << modelType
      << ", constructor not in hash table" << nl << nl
      << "    Valid correction limiter types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << abort(FatalError);
  }
  return autoPtr<CorrectionLimitingMethod>(cstrIter()(dict));
}
// Destructor 
mousse::CorrectionLimitingMethod::~CorrectionLimitingMethod()
{}
