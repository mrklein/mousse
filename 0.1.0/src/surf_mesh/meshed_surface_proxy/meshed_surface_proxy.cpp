// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "meshed_surface_proxy.hpp"
#include "time.hpp"
#include "surf_mesh.hpp"
#include "ofstream.hpp"
#include "list_ops.hpp"
// Static Member Functions
template<class Face>
mousse::wordHashSet mousse::MeshedSurfaceProxy<Face>::writeTypes()
{
  return wordHashSet(*writefileExtensionMemberFunctionTablePtr_);
}
template<class Face>
bool mousse::MeshedSurfaceProxy<Face>::canWriteType
(
  const word& ext,
  const bool verbose
)
{
  return fileFormats::surfaceFormatsCore::checkSupport
  (
    writeTypes(), ext, verbose, "writing"
  );
}
template<class Face>
void mousse::MeshedSurfaceProxy<Face>::write
(
  const fileName& name,
  const MeshedSurfaceProxy& surf
)
{
  if (debug)
  {
    Info<< "MeshedSurfaceProxy::write"
      "(const fileName&, const MeshedSurfaceProxy&) : "
      "writing to " << name
      << endl;
  }
  word ext = name.ext();
  typename writefileExtensionMemberFunctionTable::iterator mfIter =
    writefileExtensionMemberFunctionTablePtr_->find(ext);
  if (mfIter == writefileExtensionMemberFunctionTablePtr_->end())
  {
    FatalErrorIn
    (
      "MeshedSurfaceProxy::write(const fileName&)"
    )   << "Unknown file extension " << ext << nl << nl
      << "Valid types are :" << endl
      << writeTypes()
      << exit(FatalError);
  }
  mfIter()(name, surf);
}
template<class Face>
void mousse::MeshedSurfaceProxy<Face>::write
(
  const Time& t,
  const word& surfName
) const
{
  // the surface name to be used
  word name(surfName.size() ? surfName : surfaceRegistry::defaultName);
  if (debug)
  {
    Info<< "MeshedSurfaceProxy::write"
      "(const Time&, const word&) : "
      "writing to " << name
      << endl;
  }
  // the local location
  const fileName objectDir
  (
    t.timePath()/surfaceRegistry::prefix/name/surfMesh::meshSubDir
  );
  if (!isDir(objectDir))
  {
    mkDir(objectDir);
  }
  // write surfMesh/points
  {
    pointIOField io
    (
      IOobject
      (
        "points",
        t.timeName(),
        surfMesh::meshSubDir,
        t,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      )
    );
    OFstream os
    (
      objectDir/io.name(),
      t.writeFormat(),
      IOstream::currentVersion,
      t.writeCompression()
    );
    io.writeHeader(os);
    os  << this->points();
    io.writeEndDivider(os);
  }
  // write surfMesh/faces
  {
    faceCompactIOList io
    (
      IOobject
      (
        "faces",
        t.timeName(),
        surfMesh::meshSubDir,
        t,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      )
    );
    OFstream os
    (
      objectDir/io.name(),
      t.writeFormat(),
      IOstream::currentVersion,
      t.writeCompression()
    );
    io.writeHeader(os);
    if (this->useFaceMap())
    {
      // this is really a bit annoying (and wasteful) but no other way
      os  << reorder(this->faceMap(), this->faces());
    }
    else
    {
      os  << this->faces();
    }
    io.writeEndDivider(os);
  }
  // write surfMesh/surfZones
  {
    surfZoneIOList io
    (
      IOobject
      (
        "surfZones",
        t.timeName(),
        surfMesh::meshSubDir,
        t,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      )
    );
    // write as ascii
    OFstream os(objectDir/io.name());
    io.writeHeader(os);
    os  << this->surfZones();
    io.writeEndDivider(os);
  }
}
// Constructors 
template<class Face>
mousse::MeshedSurfaceProxy<Face>::MeshedSurfaceProxy
(
  const pointField& pointLst,
  const List<Face>& faceLst,
  const List<surfZone>& zoneLst,
  const List<label>& faceMap
)
:
  points_(pointLst),
  faces_(faceLst),
  zones_(zoneLst),
  faceMap_(faceMap)
{}
// Destructor 
template<class Face>
mousse::MeshedSurfaceProxy<Face>::~MeshedSurfaceProxy()
{}
