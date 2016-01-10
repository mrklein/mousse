// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge_mesh.hpp"
// Member Functions 
mousse::autoPtr<mousse::edgeMesh> mousse::edgeMesh::New
(
  const fileName& name,
  const word& ext
)
{
  fileExtensionConstructorTable::iterator cstrIter =
    fileExtensionConstructorTablePtr_->find(ext);
  if (cstrIter == fileExtensionConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "edgeMesh<Face>::New(const fileName&, const word&) : "
      "constructing edgeMesh"
    )   << "Unknown file extension " << ext
      << " for file " << name << nl << nl
      << "Valid extensions are :" << nl
      << fileExtensionConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<edgeMesh>(cstrIter()(name));
}
mousse::autoPtr<mousse::edgeMesh> mousse::edgeMesh::New(const fileName& name)
{
  word ext = name.ext();
  if (ext == "gz")
  {
    ext = name.lessExt().ext();
  }
  return New(name, ext);
}
