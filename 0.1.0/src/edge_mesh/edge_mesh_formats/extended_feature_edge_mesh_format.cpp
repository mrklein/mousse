// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_feature_edge_mesh_format.hpp"
#include "edge_mesh_format.hpp"
#include "ifstream.hpp"
// Constructors 
mousse::fileFormats::extendedFeatureEdgeMeshFormat::extendedFeatureEdgeMeshFormat
(
  const fileName& filename
)
{
  read(filename);
}
// Member Functions 
bool mousse::fileFormats::extendedFeatureEdgeMeshFormat::read
(
  const fileName& filename
)
{
  clear();
  IFstream is(filename);
  if (!is.good())
  {
    FatalErrorIn
    (
      "fileFormats::extendedFeatureEdgeMeshFormat::read(const fileName&)"
    )
      << "Cannot read file " << filename
      << exit(FatalError);
  }
  return fileFormats::edgeMeshFormat::read
  (
    is,
    this->storedPoints(),
    this->storedEdges()
  );
}
