// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermal_baffle_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace thermalBaffleModels
{
// Selectors
autoPtr<thermalBaffleModel> thermalBaffleModel::New(const fvMesh& mesh)
{
  word modelType;
  {
    IOdictionary thermalBafflePropertiesDict
    (
      IOobject
      (
        "thermalBaffleProperties",
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      )
    );
    word modelType =
      thermalBafflePropertiesDict.lookupOrDefault<word>
      (
        "thermalBaffleModel",
        "thermalBaffle"
      );
  }
  meshConstructorTable::iterator cstrIter =
    meshConstructorTablePtr_->find(modelType);
  if (cstrIter == meshConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("thermalBaffleModel::New(const fvMesh&)")
      << "Unknown thermalBaffleModel type " << modelType
      << nl << nl
      <<  "Valid thermalBaffleModel types are:" << nl
      << meshConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<thermalBaffleModel>(cstrIter()(modelType, mesh));
}
autoPtr<thermalBaffleModel> thermalBaffleModel::New
(
  const fvMesh& mesh,
  const dictionary& dict
)
{
  word modelType =
    dict.lookupOrDefault<word>("thermalBaffleModel", "thermalBaffle");
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "thermalBaffleModel::New(const fvMesh&, const dictionary&)"
    )   << "Unknown thermalBaffleModel type " << modelType
      << nl << nl
      <<  "Valid thermalBaffleModel types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<thermalBaffleModel>(cstrIter()(modelType, mesh, dict));
}
}  // namespace thermalBaffleModels
}  // namespace regionModels
}  // namespace mousse
