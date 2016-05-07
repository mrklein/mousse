// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_io_option_list.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"


// Private Member Functions
mousse::IOobject mousse::fv::IOoptionList::createIOobject
(
  const fvMesh& mesh
) const
{
  IOobject io
  {
    "fvOptions",
    mesh.time().constant(),
    mesh,
    IOobject::MUST_READ,
    IOobject::NO_WRITE
  };
  if (io.headerOk()) {
    Info << "Creating finite volume options from "
      << io.instance()/io.name() << nl
      << endl;
    io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
    return io;
  } else {
    // Check if the fvOptions file is in system
    io.instance() = mesh.time().system();
    if (io.headerOk()) {
      Info << "Creating finite volume options from "
        << io.instance()/io.name() << nl
        << endl;
      io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
      return io;
    } else {
      Info << "No finite volume options present" << nl << endl;
      io.readOpt() = IOobject::NO_READ;
      return io;
    }
  }
}


// Constructors
mousse::fv::IOoptionList::IOoptionList
(
  const fvMesh& mesh
)
:
  IOdictionary{createIOobject(mesh)},
  optionList{mesh, *this}
{}


bool mousse::fv::IOoptionList::read()
{
  if (regIOobject::read()) {
    optionList::read(*this);
    return true;
  }
  return false;
}
