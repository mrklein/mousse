// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "correction_limiting_method.hpp"
namespace mousse
{
  defineTypeNameAndDebug(CorrectionLimitingMethod, 0);
  defineRunTimeSelectionTable(CorrectionLimitingMethod, dictionary);
}
// Constructors 
mousse::CorrectionLimitingMethod::CorrectionLimitingMethod
(
  const dictionary& dict
)
{}
mousse::CorrectionLimitingMethod::CorrectionLimitingMethod
(
  const CorrectionLimitingMethod& cl
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
    FatalErrorIn
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
