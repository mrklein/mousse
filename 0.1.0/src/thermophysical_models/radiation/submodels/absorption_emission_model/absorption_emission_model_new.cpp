// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "absorption_emission_model.hpp"
// Constructors 
mousse::autoPtr<mousse::radiation::absorptionEmissionModel>
mousse::radiation::absorptionEmissionModel::New
(
  const dictionary& dict,
  const fvMesh& mesh
)
{
  const word modelType(dict.lookup("absorptionEmissionModel"));
  Info<< "Selecting absorptionEmissionModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "absorptionEmissionModel::New(const dictionary&, const fvMesh&)"
    )   << "Unknown absorptionEmissionModel type "
      << modelType << nl << nl
      << "Valid absorptionEmissionModel types are :" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<absorptionEmissionModel>(cstrIter()(dict, mesh));
}
