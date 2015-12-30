// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_film_model.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Selectors
autoPtr<surfaceFilmModel> surfaceFilmModel::New
(
  const fvMesh& mesh,
  const dimensionedVector& g,
  const word& regionType
)
{
  word modelType;
  {
    IOdictionary surfaceFilmPropertiesDict
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
    );
    surfaceFilmPropertiesDict.lookup("surfaceFilmModel") >> modelType;
  }
  Info<< "Selecting surfaceFilmModel " << modelType << endl;
  meshConstructorTable::iterator cstrIter =
    meshConstructorTablePtr_->find(modelType);
  if (cstrIter == meshConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "surfaceFilmModel::New(const fvMesh&, const dimensionedVector&)"
    )   << "Unknown surfaceFilmModel type " << modelType
      << nl << nl << "Valid surfaceFilmModel types are:" << nl
      << meshConstructorTablePtr_->toc()
      << exit(FatalError);
  }
  return autoPtr<surfaceFilmModel>
  (
    cstrIter()
    (
      modelType,
      mesh,
      g,
      regionType
    )
  );
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
