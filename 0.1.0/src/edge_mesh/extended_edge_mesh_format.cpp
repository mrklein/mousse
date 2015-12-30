// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extended_edge_mesh_format.hpp"
#include "edge_mesh_format.hpp"
#include "ifstream.hpp"
#include "time.hpp"
// Constructors 
mousse::fileFormats::extendedEdgeMeshFormat::extendedEdgeMeshFormat
(
  const fileName& filename
)
{
  read(filename);
}
// Member Functions 
bool mousse::fileFormats::extendedEdgeMeshFormat::read
(
  const fileName& filename
)
{
  clear();
  fileName dir = filename.path();
  fileName caseName = dir.name();
  fileName rootPath = dir.path();
  // Construct dummy time to use as an objectRegistry
  Time dummyTime
  (
    ".",        //rootPath,
    ".",        //caseName,
    "system",   //systemName,
    "constant", //constantName,
    false       //enableFunctionObjects
  );
  // Construct IOobject to re-use the headerOk & readHeader
  // (so we can read ascii and binary)
  IOobject io
  (
    filename,
    dummyTime,
    IOobject::NO_READ,
    IOobject::NO_WRITE,
    false
  );
  if (!io.headerOk())
  {
    FatalErrorIn
    ("fileFormats::extendedEdgeMeshFormat::read(const fileName&)")
      << "Cannot read file " << filename
      << exit(FatalError);
  }
  autoPtr<IFstream> isPtr(new IFstream(io.filePath()));
  bool ok = false;
  if (isPtr().good())
  {
    Istream& is = isPtr();
    ok = io.readHeader(is);
    if (ok)
    {
      // Use extendedEdgeMesh IO
      is >> *this;
      ok = is.good();
    }
  }
  return ok;
}
