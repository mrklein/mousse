// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "engine_mesh.hpp"
#include "dimensioned_scalar.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(engineMesh, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(engineMesh, IOobject);
}
// Constructors 
mousse::engineMesh::engineMesh(const IOobject& io)
:
  fvMesh(io),
  engineDB_(refCast<const engineTime>(time())),
  pistonIndex_(-1),
  linerIndex_(-1),
  cylinderHeadIndex_(-1),
  deckHeight_("deckHeight", dimLength, GREAT),
  pistonPosition_("pistonPosition", dimLength, -GREAT)
{
  bool foundPiston = false;
  bool foundLiner = false;
  bool foundCylinderHead = false;
  FOR_ALL(boundary(), i)
  {
    if (boundary()[i].name() == "piston")
    {
      pistonIndex_ = i;
      foundPiston = true;
    }
    else if (boundary()[i].name() == "liner")
    {
      linerIndex_ = i;
      foundLiner = true;
    }
    else if (boundary()[i].name() == "cylinderHead")
    {
      cylinderHeadIndex_ = i;
      foundCylinderHead = true;
    }
  }
  reduce(foundPiston, orOp<bool>());
  reduce(foundLiner, orOp<bool>());
  reduce(foundCylinderHead, orOp<bool>());
  if (!foundPiston)
  {
    FATAL_ERROR_IN("engineMesh::engineMesh(const IOobject& io)")
      << "cannot find piston patch"
      << exit(FatalError);
  }
  if (!foundLiner)
  {
    FATAL_ERROR_IN("engineMesh::engineMesh(const IOobject& io)")
      << "cannot find liner patch"
      << exit(FatalError);
  }
  if (!foundCylinderHead)
  {
    FATAL_ERROR_IN("engineMesh::engineMesh(const IOobject& io)")
      << "cannot find cylinderHead patch"
      << exit(FatalError);
  }
  {
    if (pistonIndex_ != -1)
    {
      pistonPosition_.value() = -GREAT;
      if (boundary()[pistonIndex_].patch().localPoints().size())
      {
        pistonPosition_.value() =
          max(boundary()[pistonIndex_].patch().localPoints()).z();
      }
    }
    reduce(pistonPosition_.value(), maxOp<scalar>());
    if (cylinderHeadIndex_ != -1)
    {
      deckHeight_.value() = GREAT;
      if (boundary()[cylinderHeadIndex_].patch().localPoints().size())
      {
        deckHeight_.value() = min
        (
          boundary()[cylinderHeadIndex_].patch().localPoints()
        ).z();
      }
    }
    reduce(deckHeight_.value(), minOp<scalar>());
    Info<< "deckHeight: " << deckHeight_.value() << nl
      << "piston position: " << pistonPosition_.value() << endl;
  }
}
// Destructor 
mousse::engineMesh::~engineMesh()
{}
