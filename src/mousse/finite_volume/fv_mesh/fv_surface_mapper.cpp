// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_surface_mapper.hpp"
#include "fv_mesh.hpp"
#include "map_poly_mesh.hpp"
#include "face_mapper.hpp"
#include "demand_driven_data.hpp"


// Private Member Functions 
void mousse::fvSurfaceMapper::calcAddressing() const
{
  if (directAddrPtr_ || interpolationAddrPtr_ || weightsPtr_
      || insertedObjectLabelsPtr_) {
    FATAL_ERROR_IN("void fvSurfaceMapper::calcAddressing() const)")
      << "Addressing already calculated"
      << abort(FatalError);
  }
  // Mapping
  const label oldNInternal = faceMap_.nOldInternalFaces();
  // Assemble the maps
  if (direct()) {
    // Direct mapping - slice to size
    directAddrPtr_ =
      new labelList{labelList::subList(faceMap_.directAddressing(), size())};
    labelList& addr = *directAddrPtr_;
    // Adjust for creation of an internal face from a boundary face
    FOR_ALL(addr, faceI) {
      if (addr[faceI] > oldNInternal) {
        addr[faceI] = 0;
      }
    }
  } else {
    // Interpolative mapping - slice to size
    interpolationAddrPtr_ =
      new labelListList{labelListList::subList(faceMap_.addressing(), size())};
    labelListList& addr = *interpolationAddrPtr_;
    weightsPtr_ =
      new scalarListList{scalarListList::subList(faceMap_.weights(), size())};
    scalarListList& w = *weightsPtr_;
    // Adjust for creation of an internal face from a boundary face
    FOR_ALL(addr, faceI) {
      if (max(addr[faceI]) >= oldNInternal) {
        addr[faceI] = labelList{1, label(0)};
        w[faceI] = scalarList{1, 1.0};
      }
    }
  }
  // Inserted objects
  // If there are, assemble the labels
  if (insertedObjects()) {
    const labelList& insFaces = faceMap_.insertedObjectLabels();
    insertedObjectLabelsPtr_ = new labelList{insFaces.size()};
    labelList& ins = *insertedObjectLabelsPtr_;
    label nIns = 0;
    FOR_ALL(insFaces, faceI) {
      // If the face is internal, keep it here
      if (insFaces[faceI] < size()) {
        ins[nIns] = insFaces[faceI];
        nIns++;
      }
    }
    ins.setSize(nIns);
  } else {
    // No inserted objects
    insertedObjectLabelsPtr_ = new labelList{0};
  }
}


void mousse::fvSurfaceMapper::clearOut()
{
  deleteDemandDrivenData(directAddrPtr_);
  deleteDemandDrivenData(interpolationAddrPtr_);
  deleteDemandDrivenData(weightsPtr_);
  deleteDemandDrivenData(insertedObjectLabelsPtr_);
}


// Constructors 

// Construct from components
mousse::fvSurfaceMapper::fvSurfaceMapper
(
  const fvMesh& mesh,
  const faceMapper& fMapper
)
:
  mesh_{mesh},
  faceMap_{fMapper},
  directAddrPtr_{NULL},
  interpolationAddrPtr_{NULL},
  weightsPtr_{NULL},
  insertedObjectLabelsPtr_{NULL}
{}


// Destructor 
mousse::fvSurfaceMapper::~fvSurfaceMapper()
{
  clearOut();
}


// Member Functions 
const mousse::labelUList& mousse::fvSurfaceMapper::directAddressing() const
{
  if (!direct()) {
    FATAL_ERROR_IN
    (
      "const labelUList& fvSurfaceMapper::"
      "directAddressing() const"
    )
    << "Requested direct addressing for an interpolative mapper."
    << abort(FatalError);
  }
  if (!directAddrPtr_) {
    calcAddressing();
  }
  return *directAddrPtr_;
}


const mousse::labelListList& mousse::fvSurfaceMapper::addressing() const
{
  if (direct()) {
    FATAL_ERROR_IN
    (
      "const labelListList& fvSurfaceMapper::addressing() const"
    )
    << "Requested interpolative addressing for a direct mapper."
    << abort(FatalError);
  }
  if (!interpolationAddrPtr_) {
    calcAddressing();
  }
  return *interpolationAddrPtr_;
}


const mousse::scalarListList& mousse::fvSurfaceMapper::weights() const
{
  if (direct()) {
    FATAL_ERROR_IN
    (
      "const scalarListList& fvSurfaceMapper::weights() const"
    )
    << "Requested interpolative weights for a direct mapper."
    << abort(FatalError);
  }
  if (!weightsPtr_) {
    calcAddressing();
  }
  return *weightsPtr_;
}


const mousse::labelList& mousse::fvSurfaceMapper::insertedObjectLabels() const
{
  if (!insertedObjectLabelsPtr_) {
    calcAddressing();
  }
  return *insertedObjectLabelsPtr_;
}

