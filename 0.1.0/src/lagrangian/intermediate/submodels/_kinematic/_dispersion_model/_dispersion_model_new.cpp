// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_dispersion_model.hpp"
// Constructors 
template<class CloudType>
mousse::autoPtr<mousse::DispersionModel<CloudType> >
mousse::DispersionModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("dispersionModel"));
  Info<< "Selecting dispersion model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "DispersionModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown dispersion model type "
      << modelType << nl << nl
      << "Valid dispersion model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<DispersionModel<CloudType> >(cstrIter()(dict, owner));
}
