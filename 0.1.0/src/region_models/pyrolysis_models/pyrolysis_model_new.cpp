// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pyrolysis_model.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
namespace mousse
{
namespace regionModels
{
namespace pyrolysisModels
{
// Selectors
autoPtr<pyrolysisModel> pyrolysisModel::New
(
  const fvMesh& mesh,
  const word& regionType
)
{
  // get model name, but do not register the dictionary
  const word modelType
  (
    IOdictionary
    (
      IOobject
      (
        regionType + "Properties",
        mesh.time().constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      )
    ).lookup("pyrolysisModel")
  );
  Info<< "Selecting pyrolysisModel " << modelType << endl;
  meshConstructorTable::iterator cstrIter =
    meshConstructorTablePtr_->find(modelType);
  if (cstrIter == meshConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("pyrolysisModel::New(const fvMesh&, const word&)")
      << "Unknown pyrolysisModel type " << modelType
      << nl << nl << "Valid pyrolisisModel types are:" << nl
      << meshConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<pyrolysisModel>(cstrIter()(modelType, mesh, regionType));
}
autoPtr<pyrolysisModel> pyrolysisModel::New
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& regionType
)
{
  const word modelType = dict.lookup("pyrolysisModel");
  Info<< "Selecting pyrolysisModel " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "pyrolysisModel::New"
      "("
        "const fvMesh&, "
        "const dictionary&, "
        "const word&"
      ")"
    )
      << "Unknown pyrolysisModel type " << modelType
      << nl << nl << "Valid pyrolisisModel types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<pyrolysisModel>
  (
    cstrIter()
    (
      modelType,
      mesh,
      dict,
      regionType
    )
  );
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
