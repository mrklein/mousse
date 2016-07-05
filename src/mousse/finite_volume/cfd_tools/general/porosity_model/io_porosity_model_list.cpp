// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "io_porosity_model_list.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"


// Private Member Functions 
mousse::IOobject mousse::IOporosityModelList::createIOobject
(
  const fvMesh& mesh
) const
{
  IOobject io
  {
    "porosityProperties",
    mesh.time().constant(),
    mesh,
    IOobject::MUST_READ,
    IOobject::NO_WRITE
  };
  if (io.headerOk()) {
    Info << "Creating porosity model list from " << io.name() << nl << endl;
    io.readOpt() = IOobject::MUST_READ_IF_MODIFIED;
    return io;
  } else {
    Info << "No porosity models present" << nl << endl;
    io.readOpt() = IOobject::NO_READ;
    return io;
  }
}


// Constructors 
mousse::IOporosityModelList::IOporosityModelList
(
  const fvMesh& mesh
)
:
  IOdictionary{createIOobject(mesh)},
  porosityModelList{mesh, *this}
{}


bool mousse::IOporosityModelList::read()
{
  if (regIOobject::read()) {
    porosityModelList::read(*this);
    return true;
  } else {
    return false;
  }
}

