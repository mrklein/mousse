// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_atomization_model.hpp"

template<class CloudType>
mousse::autoPtr<mousse::AtomizationModel<CloudType>>
mousse::AtomizationModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  word AtomizationModelType(dict.lookup("atomizationModel"));
  Info<< "Selecting AtomizationModel " << AtomizationModelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(AtomizationModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "AtomizationModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )
    << "Unknown AtomizationModelType type "
    << AtomizationModelType
    << ", constructor not in hash table" << nl << nl
    << "    Valid AtomizationModel types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<AtomizationModel<CloudType>>{cstrIter()(dict, owner)};
}
