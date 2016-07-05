// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_fv_mesh.hpp"
#include "time.hpp"
#include "dl_library_table.hpp"


// Selectors
mousse::autoPtr<mousse::dynamicFvMesh> mousse::dynamicFvMesh::New(const IOobject& io)
{
  // Note: - do not register the dictionary since dynamicFvMeshes themselves
  // do this.
  // - defaultRegion (region0) gets loaded from constant, other ones
  //   get loaded from constant/<regionname>. Normally we'd use
  //   polyMesh::dbDir() but we haven't got a polyMesh yet ...
  IOdictionary dict
  {
    {
      "dynamicMeshDict",
      io.time().constant(),
      (io.name() == polyMesh::defaultRegion ? "" : io.name()),
      io.db(),
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    }
  };
  const word dynamicFvMeshTypeName{dict.lookup("dynamicFvMesh")};
  Info << "Selecting dynamicFvMesh " << dynamicFvMeshTypeName << endl;
  const_cast<Time&>(io.time())
    .libs()
    .open
    (
      dict,
      "dynamicFvMeshLibs",
      IOobjectConstructorTablePtr_
    );
  if (!IOobjectConstructorTablePtr_) {
    FATAL_ERROR_IN
    (
      "dynamicFvMesh::New(const IOobject&)"
    )
    << "dynamicFvMesh table is empty"
    << exit(FatalError);
  }
  IOobjectConstructorTable::iterator cstrIter =
    IOobjectConstructorTablePtr_->find(dynamicFvMeshTypeName);
  if (cstrIter == IOobjectConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "dynamicFvMesh::New(const IOobject&)"
    )
    << "Unknown dynamicFvMesh type "
    << dynamicFvMeshTypeName << nl << nl
    << "Valid dynamicFvMesh types are :" << endl
    << IOobjectConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<dynamicFvMesh>{cstrIter()(io)};
}

