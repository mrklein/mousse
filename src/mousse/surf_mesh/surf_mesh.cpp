// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surf_mesh.hpp"
#include "meshed_surface_proxy.hpp"
#include "time.hpp"
#include "os_specific.hpp"
#include "meshed_surface.hpp"
#include "demand_driven_data.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(surfMesh, 0);

}


mousse::word mousse::surfMesh::meshSubDir = "surfMesh";

// Constructors 
mousse::surfMesh::surfMesh(const IOobject& io, const word& surfName)
:
  surfaceRegistry{io.db(), (surfName.size() ? surfName : io.name())},
  Allocator
  {
    {
      "points",
      time().findInstance(meshDir(), "points"),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    {
      "faces",
      time().findInstance(meshDir(), "faces"),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    {
      "surfZones",
      time().findInstance(meshDir(), "surfZones"),
      meshSubDir,
      *this,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  },
  MeshReference{this->storedIOFaces(), this->storedIOPoints()}
{}


mousse::surfMesh::surfMesh
(
  const IOobject& io,
  const Xfer<pointField>& pointLst,
  const Xfer<faceList>& faceLst,
  const word& surfName
)
:
  surfaceRegistry{io.db(), (surfName.size() ? surfName : io.name())},
  Allocator
  {
    {
      "points",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    pointLst,
    {
      "faces",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    faceLst,
    {
      "surfZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    Xfer<surfZoneList>()
  },
  MeshReference{this->storedIOFaces(), this->storedIOPoints()}
{}


mousse::surfMesh::surfMesh
(
  const IOobject& io,
  const Xfer<MeshedSurface<face> >& surf,
  const word& surfName
)
:
  surfaceRegistry{io.db(), (surfName.size() ? surfName : io.name())},
  Allocator
  {
    {
      "points",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    pointField(),
    {
      "faces",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    faceList(),
    {
      "surfZones",
      instance(),
      meshSubDir,
      *this,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    surfZoneList{}
  },
  MeshReference{this->storedIOFaces(), this->storedIOPoints()}
{
  if (debug) {
    Info <<"IOobject: " << io.path() << nl << " name: " << io.name()
      <<" instance: " << io.instance() <<" local: " << io.local()
      <<" dbDir: " << io.db().dbDir() << endl;
    Info <<"creating surfMesh at instance " << instance() << endl;
    Info <<"timeName: " << instance() << endl;
  }
  // We can also send Xfer<..>::null just to initialize without allocating
  if (notNull(surf)) {
    transfer(surf());
  }
}


// Destructor 
mousse::surfMesh::~surfMesh()
{}


// Member Functions 
void mousse::surfMesh::updatePointsRef()
{
  // assign the reference to the points (this is truly ugly)
  reinterpret_cast<SubField<point>&>
  (
    const_cast<Field<point>&>(MeshReference::points())
  ) = reinterpret_cast<SubField<point>&>(this->storedPoints());
}


void mousse::surfMesh::updateFacesRef()
{
  // assign the reference to the faces
  static_cast<UList<face>&>(*this) = this->storedFaces();
}


void mousse::surfMesh::updateRefs()
{
  this->updatePointsRef();
  this->updateFacesRef();
}


void mousse::surfMesh::resetPrimitives
(
  const Xfer<pointField>& points,
  const Xfer<faceList>& faces,
  const Xfer<surfZoneList>& zones,
  const bool validate
)
{
  // Clear addressing.
  MeshReference::clearGeom();
  Allocator::reset(points, faces, zones);
  this->updateRefs();
  if (validate) {
    checkZones();
  }
}


void mousse::surfMesh::transfer
(
  MeshedSurface<face>& surf
)
{
  // Clear addressing.
  MeshReference::clearGeom();
  this->storedIOPoints().transfer(surf.storedPoints());
  this->storedIOFaces().transfer(surf.storedFaces());
  this->storedIOZones().transfer(surf.storedZones());
  this->updateRefs();
}


mousse::Xfer<mousse::MeshedSurface<mousse::face> > mousse::surfMesh::xfer()
{
  Xfer<MeshedSurface<face> > xf;
  xf().storedPoints().transfer(this->storedPoints());
  xf().storedFaces().transfer(this->storedFaces());
  xf().storedZones().transfer(this->storedZones());
  // is this needed?
  this->updateRefs();
  // Clear addressing.
  MeshReference::clearGeom();
  return xf;
}


mousse::fileName mousse::surfMesh::meshDir() const
{
  return dbDir()/meshSubDir;
}


const mousse::fileName& mousse::surfMesh::pointsInstance() const
{
  return this->storedIOPoints().instance();
}


const mousse::fileName& mousse::surfMesh::facesInstance() const
{
  return this->storedIOFaces().instance();
}


mousse::label mousse::surfMesh::nPoints() const
{
  return this->points().size();
}


mousse::label mousse::surfMesh::nFaces() const
{
  return this->faces().size();
}


const mousse::pointField& mousse::surfMesh::points() const
{
  return this->storedIOPoints();
}


const mousse::faceList& mousse::surfMesh::faces() const
{
  return this->storedIOFaces();
}


void mousse::surfMesh::checkZones()
{
  // extra safety, ensure we have at some zones
  // and they cover all the faces - fix start silently
  surfZoneList& zones = Allocator::storedIOZones();
  if (zones.size() <= 1) {
    removeZones();
  } else {
    label count = 0;
    FOR_ALL(zones, zoneI) {
      zones[zoneI].start() = count;
      count += zones[zoneI].size();
    }
    if (count < nFaces()) {
      WARNING_IN
      (
        "surfMesh::checkZones()\n"
      )
      << "more faces " << nFaces() << " than zones " << count
      << " ... extending final zone"
      << endl;
      zones.last().size() += count - nFaces();
    } else if (count > size()) {
      FATAL_ERROR_IN
      (
        "surfMesh::checkZones()\n"
      )
      << "more zones " << count << " than faces " << nFaces()
      << exit(FatalError);
    }
  }
}


// Add boundary patches. Constructor helper
void mousse::surfMesh::addZones
(
  const surfZoneList& srfZones,
  const bool validate
)
{
  surfZoneList& zones = Allocator::storedIOZones();
  FOR_ALL(zones, zoneI) {
    zones[zoneI] = surfZone(srfZones[zoneI], zoneI);
  }
  if (validate) {
    checkZones();
  }
}


// Remove all files and some subdirs (eg, sets)
void mousse::surfMesh::removeFiles(const fileName& instanceDir) const
{
  fileName meshFilesPath = db().path()/instanceDir/meshSubDir;
  rm(meshFilesPath/"points");
  rm(meshFilesPath/"faces");
  rm(meshFilesPath/"surfZones");
}


void mousse::surfMesh::removeFiles() const
{
  removeFiles(instance());
}


void mousse::surfMesh::write(const fileName& name, const surfMesh& surf)
{
  MeshedSurfaceProxy<face>
  (
    surf.points(),
    surf.faces(),
    surf.surfZones()
  ).write(name);
}


void mousse::surfMesh::write(const fileName& name)
{
  write(name, *this);
}

