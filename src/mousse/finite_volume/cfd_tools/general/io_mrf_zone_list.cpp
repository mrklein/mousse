// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "io_mrf_zone_list.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"


// Private Member Functions 
mousse::IOobject mousse::IOMRFZoneList::createIOobject
(
  const fvMesh& mesh
) const
{
  IOobject io
  {
    "MRFProperties",
    mesh.time().constant(),
    mesh,
    IOobject::MUST_READ,
    IOobject::NO_WRITE
  };
  if (io.headerOk()) {
    Info << "Creating MRF zone list from " << io.name() << endl;
    io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
    return io;
  } else {
    Info << "No MRF models present" << nl << endl;
    io.readOpt() = IOobject::NO_READ;
    return io;
  }
}


// Constructors 
mousse::IOMRFZoneList::IOMRFZoneList
(
  const fvMesh& mesh
)
:
  IOdictionary{createIOobject(mesh)},
  MRFZoneList{mesh, *this}
{}


bool mousse::IOMRFZoneList::read()
{
  if (regIOobject::read()) {
    MRFZoneList::read(*this);
    return true;
  } else {
    return false;
  }
}
