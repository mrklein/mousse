// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_mesh_object.hpp"

// Static Member Data
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(meshObject, 0);

}

// Constructors 
mousse::meshObject::meshObject(const word& typeName, const objectRegistry& obr)
:
  regIOobject
  (
    IOobject
    (
      typeName,
      obr.instance(),
      obr
    )
  )
{}
