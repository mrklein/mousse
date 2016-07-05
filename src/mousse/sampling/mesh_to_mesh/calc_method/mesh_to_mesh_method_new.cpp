// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_to_mesh_method.hpp"


// Member Functions 
mousse::autoPtr<mousse::meshToMeshMethod> mousse::meshToMeshMethod::New
(
  const word& methodName,
  const polyMesh& src,
  const polyMesh& tgt
)
{
  if (debug) {
    Info << "Selecting AMIMethod " << methodName << endl;
  }
  componentsConstructorTable::iterator cstrIter =
    componentsConstructorTablePtr_->find(methodName);
  if (cstrIter == componentsConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "mousse::autoPtr<mousse::meshToMeshMethod> mousse::meshToMeshMethod::New"
      "("
        "const word&, "
        "const polyMesh&, "
        "const polyMesh&"
      ")"
    )
    << "Unknown meshToMesh type "
    << methodName << nl << nl
    << "Valid meshToMesh types are:" << nl
    << componentsConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<meshToMeshMethod>{cstrIter()(src, tgt)};
}

