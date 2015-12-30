// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_edge_mesh.hpp"
// Static Data Members
namespace mousse
{
  defineRunTimeSelectionTable(extendedEdgeMesh, fileExtension);
}
// Member Functions 
mousse::autoPtr<mousse::extendedEdgeMesh> mousse::extendedEdgeMesh::New
(
  const fileName& name,
  const word& ext
)
{
  fileExtensionConstructorTable::iterator cstrIter =
    fileExtensionConstructorTablePtr_->find(ext);
  if (cstrIter == fileExtensionConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "extendedEdgeMesh::New(const fileName&, const word&) : "
      "constructing extendedEdgeMesh"
    )   << "Unknown file extension " << ext
      << " for file " << name << nl << nl
      << "Valid extensions are :" << nl
      << fileExtensionConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<extendedEdgeMesh>(cstrIter()(name));
}
mousse::autoPtr<mousse::extendedEdgeMesh> mousse::extendedEdgeMesh::New
(
  const fileName& name
)
{
  word ext = name.ext();
  if (ext == "gz")
  {
    ext = name.lessExt().ext();
  }
  return New(name, ext);
}
