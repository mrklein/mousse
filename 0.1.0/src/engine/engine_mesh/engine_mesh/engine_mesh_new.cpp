// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "engine_mesh.hpp"
#include "time.hpp"
// Selectors
mousse::autoPtr<mousse::engineMesh> mousse::engineMesh::New
(
  const mousse::IOobject& io
)
{
  // get model name, but do not register the dictionary
  // otherwise it is registered in the database twice
  const word modelType
  (
    IOdictionary
    (
      IOobject
      (
        "engineGeometry",
        io.time().constant(),
        io.db(),
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
      )
    ).lookup("engineMesh")
  );
  Info<< "Selecting engineMesh " << modelType << endl;
  IOobjectConstructorTable::iterator cstrIter =
    IOobjectConstructorTablePtr_->find(modelType);
  if (cstrIter == IOobjectConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "engineMesh::New(const IOobject&)"
    )   << "Unknown engineMesh type "
      << modelType << nl << nl
      << "Valid engineMesh types are :" << endl
      << IOobjectConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<engineMesh>(cstrIter()(io));
}
