// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge_mesh_formats_core.hpp"
#include "time.hpp"
#include "ifstream.hpp"
#include "ofstream.hpp"
#include "edge_mesh.hpp"
// Static Data Members
mousse::word mousse::fileFormats::edgeMeshFormatsCore::nativeExt("eMesh");
// Static Member Functions
mousse::string mousse::fileFormats::edgeMeshFormatsCore::getLineNoComment
(
  IFstream& is
)
{
  string line;
  do
  {
    is.getLine(line);
  }
  while ((line.empty() || line[0] == '#') && is.good());
  return line;
}
#if 0
mousse::fileName mousse::fileFormats::edgeMeshFormatsCore::localMeshFileName
(
  const word& meshName
)
{
  const word name(meshName.size() ? meshName : surfaceRegistry::defaultName);
  return fileName
  (
    surfaceRegistry::prefix/name/surfMesh::meshSubDir
   / name + "." + nativeExt
  );
}
mousse::fileName mousse::fileFormats::edgeMeshFormatsCore::findMeshInstance
(
  const Time& t,
  const word& meshName
)
{
  fileName localName = localMeshFileName(meshName);
  // Search back through the time directories list to find the time
  // closest to and lower than current time
  instantList ts = t.times();
  label instanceI;
  for (instanceI = ts.size()-1; instanceI >= 0; --instanceI)
  {
    if (ts[instanceI].value() <= t.timeOutputValue())
    {
      break;
    }
  }
  // Noting that the current directory has already been searched
  // for mesh data, start searching from the previously stored time directory
  if (instanceI >= 0)
  {
    for (label i = instanceI; i >= 0; --i)
    {
      if (isFile(t.path()/ts[i].name()/localName))
      {
        return ts[i].name();
      }
    }
  }
  return t.constant();
}
mousse::fileName mousse::fileFormats::edgeMeshFormatsCore::findMeshFile
(
  const Time& t,
  const word& meshName
)
{
  fileName localName = localMeshFileName(meshName);
  // Search back through the time directories list to find the time
  // closest to and lower than current time
  instantList ts = t.times();
  label instanceI;
  for (instanceI = ts.size()-1; instanceI >= 0; --instanceI)
  {
    if (ts[instanceI].value() <= t.timeOutputValue())
    {
      break;
    }
  }
  // Noting that the current directory has already been searched
  // for mesh data, start searching from the previously stored time directory
  if (instanceI >= 0)
  {
    for (label i = instanceI; i >= 0; --i)
    {
      fileName testName(t.path()/ts[i].name()/localName);
      if (isFile(testName))
      {
        return testName;
      }
    }
  }
  // fallback to "constant"
  return t.path()/t.constant()/localName;
}
#endif
bool mousse::fileFormats::edgeMeshFormatsCore::checkSupport
(
  const wordHashSet& available,
  const word& ext,
  const bool verbose,
  const word& functionName
)
{
  if (available.found(ext))
  {
    return true;
  }
  else if (verbose)
  {
    wordList known = available.sortedToc();
    Info<<"Unknown file extension for " << functionName
      << " : " << ext << nl
      <<"Valid types: (";
    // compact output:
    FOR_ALL(known, i)
    {
      Info<<" " << known[i];
    }
    Info<<" )" << endl;
  }
  return false;
}
// Constructors 
mousse::fileFormats::edgeMeshFormatsCore::edgeMeshFormatsCore()
{}
// Destructor 
mousse::fileFormats::edgeMeshFormatsCore::~edgeMeshFormatsCore()
{}
