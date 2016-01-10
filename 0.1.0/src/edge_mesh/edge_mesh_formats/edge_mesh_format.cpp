// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge_mesh_format.hpp"
#include "ioobject.hpp"
#include "ifstream.hpp"
#include "clock.hpp"
#include "time.hpp"
#include "feature_edge_mesh.hpp"
// Constructors 
mousse::fileFormats::edgeMeshFormat::edgeMeshFormat
(
  const fileName& filename
)
{
  read(filename);
}
// Member Functions 
bool mousse::fileFormats::edgeMeshFormat::read
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
    FATAL_ERROR_IN("fileFormats::edgeMeshFormat::read(const fileName&)")
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
      ok = read(is, this->storedPoints(), this->storedEdges());
    }
  }
  return ok;
}
bool mousse::fileFormats::edgeMeshFormat::read
(
  Istream& is,
  pointField& pointLst,
  edgeList& edgeLst
)
{
  if (!is.good())
  {
    FATAL_ERROR_IN
    (
      "fileFormats::edgeMeshFormat::read"
      "(Istream&, pointField&, edgeList&)"
    )
      << "read error "
      << exit(FatalError);
  }
  // read points:
  is  >> pointLst;
  // read edges:
  is  >> edgeLst;
  return true;
}
mousse::Ostream& mousse::fileFormats::edgeMeshFormat::write
(
  Ostream& os,
  const pointField& pointLst,
  const edgeList& edgeLst
)
{
  if (!os.good())
  {
    FATAL_ERROR_IN
    (
      "fileFormats::edgeMeshFormat::write"
      "(Ostream&, const fileName&, const edgeMesh&)"
    )
      << "bad output stream " << os.name()
      << exit(FatalError);
  }
  os  << "\n// points:" << nl << pointLst << nl
    << "\n// edges:" << nl << edgeLst << nl;
  IOobject::writeDivider(os);
  // Check state of Ostream
  os.check
  (
    "edgeMeshFormat::write"
    "(Ostream&, const pointField&, const edgeList&)"
  );
  return os;
}
void mousse::fileFormats::edgeMeshFormat::write
(
  const fileName& filename,
  const edgeMesh& mesh
)
{
  // Construct dummy time to use as an objectRegistry
  Time dummyTime
  (
    ".",        //rootPath,
    ".",        //caseName,
    "system",   //systemName,
    "constant", //constantName,
    false       //enableFunctionObjects
  );
  // Construct IOobject to re-use the writeHeader
  IOobject io
  (
    filename,
    dummyTime,
    IOobject::NO_READ,
    IOobject::NO_WRITE,
    false
  );
  io.note() = "written " + clock::dateTime();
  // Note: always write ascii
  autoPtr<OFstream> osPtr(new OFstream(filename));
  if (!osPtr().good())
  {
    FATAL_IO_ERROR_IN
    (
      "fileFormats::edgeMeshFormat::write"
      "(const fileName&, const edgeMesh&)",
      osPtr()
    )   << "Cannot open file for writing " << filename
      << exit(FatalIOError);
  }
  OFstream& os = osPtr();
  bool ok = io.writeHeader(os, featureEdgeMesh::typeName);
  if (!ok)
  {
    FATAL_IO_ERROR_IN
    (
      "fileFormats::edgeMeshFormat::write"
      "(const fileName&, const edgeMesh&)",
      os
    )   << "Cannot write header"
      << exit(FatalIOError);
  }
  write(os, mesh.points(), mesh.edges());
  // Check state of Ostream
  os.check("edgeMeshFormat::write(Ostream&)");
}
