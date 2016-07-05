// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_mapper.hpp"
#include "demand_driven_data.hpp"
#include "poly_mesh.hpp"
#include "map_poly_mesh.hpp"


// Private Member Functions 
void mousse::faceMapper::calcAddressing() const
{
  if (directAddrPtr_ != nullptr
      || interpolationAddrPtr_ != nullptr
      || weightsPtr_ != nullptr
      || insertedFaceLabelsPtr_ != nullptr) {
    FATAL_ERROR_IN("void faceMapper::calcAddressing() const")
      << "Addressing already calculated."
      << abort(FatalError);
  }
  if (direct()) {
    // Direct addressing, no weights
    directAddrPtr_ = new labelList{mpm_.faceMap()};
    labelList& directAddr = *directAddrPtr_;
    // Reset the size of addressing list to contain only live faces
    directAddr.setSize(mesh_.nFaces());
    insertedFaceLabelsPtr_ = new labelList{mesh_.nFaces()};
    labelList& insertedFaces = *insertedFaceLabelsPtr_;
    label nInsertedFaces = 0;
    FOR_ALL(directAddr, faceI) {
      if (directAddr[faceI] < 0) {
        // Found inserted face
        directAddr[faceI] = 0;
        insertedFaces[nInsertedFaces] = faceI;
        nInsertedFaces++;
      }
    }
    insertedFaces.setSize(nInsertedFaces);
  } else {
    // Interpolative addressing
    interpolationAddrPtr_ = new labelListList{mesh_.nFaces()};
    labelListList& addr = *interpolationAddrPtr_;
    weightsPtr_ = new scalarListList{mesh_.nFaces()};
    scalarListList& w = *weightsPtr_;
    const List<objectMap>& ffp = mpm_.facesFromPointsMap();
    FOR_ALL(ffp, ffpI) {
      // Get addressing
      const labelList& mo = ffp[ffpI].masterObjects();
      label faceI = ffp[ffpI].index();
      if (addr[faceI].size()) {
        FATAL_ERROR_IN("void faceMapper::calcAddressing() const")
          << "Master face " << faceI
          << " mapped from point faces " << mo
          << " already destination of mapping." << abort(FatalError);
      }
      // Map from masters, uniform weights
      addr[faceI] = mo;
      w[faceI] = scalarList(mo.size(), 1.0/mo.size());
    }
    const List<objectMap>& ffe = mpm_.facesFromEdgesMap();
    FOR_ALL(ffe, ffeI) {
      // Get addressing
      const labelList& mo = ffe[ffeI].masterObjects();
      label faceI = ffe[ffeI].index();
      if (addr[faceI].size()) {
        FATAL_ERROR_IN("void faceMapper::calcAddressing() const")
          << "Master face " << faceI
          << " mapped from edge faces " << mo
          << " already destination of mapping." << abort(FatalError);
      }
      // Map from masters, uniform weights
      addr[faceI] = mo;
      w[faceI] = scalarList(mo.size(), 1.0/mo.size());
    }
    const List<objectMap>& fff = mpm_.facesFromFacesMap();
    FOR_ALL(fff, fffI) {
      // Get addressing
      const labelList& mo = fff[fffI].masterObjects();
      label faceI = fff[fffI].index();
      if (addr[faceI].size()) {
        FATAL_ERROR_IN("void faceMapper::calcAddressing() const")
          << "Master face " << faceI
          << " mapped from face faces " << mo
          << " already destination of mapping." << abort(FatalError);
      }
      // Map from masters, uniform weights
      addr[faceI] = mo;
      w[faceI] = scalarList(mo.size(), 1.0/mo.size());
    }
    // Do mapped faces. Note that can already be set from facesFromFaces
    // so check if addressing size still zero.
    const labelList& fm = mpm_.faceMap();
    FOR_ALL(fm, faceI) {
      if (fm[faceI] > -1 && addr[faceI].empty()) {
        // Mapped from a single face
        addr[faceI] = labelList(1, fm[faceI]);
        w[faceI] = scalarList(1, 1.0);
      }
    }
    // Grab inserted faces (for them the size of addressing is still zero)
    insertedFaceLabelsPtr_ = new labelList{mesh_.nFaces()};
    labelList& insertedFaces = *insertedFaceLabelsPtr_;
    label nInsertedFaces = 0;
    FOR_ALL(addr, faceI) {
      if (addr[faceI].empty()) {
        // Mapped from a dummy face
        addr[faceI] = labelList(1, label(0));
        w[faceI] = scalarList(1, 1.0);
        insertedFaces[nInsertedFaces] = faceI;
        nInsertedFaces++;
      }
    }
    insertedFaces.setSize(nInsertedFaces);
  }
}


void mousse::faceMapper::clearOut()
{
  deleteDemandDrivenData(directAddrPtr_);
  deleteDemandDrivenData(interpolationAddrPtr_);
  deleteDemandDrivenData(weightsPtr_);
  deleteDemandDrivenData(insertedFaceLabelsPtr_);
}


// Constructors 

// Construct from components
mousse::faceMapper::faceMapper(const mapPolyMesh& mpm)
:
  mesh_{mpm.mesh()},
  mpm_{mpm},
  insertedFaces_{true},
  direct_{false},
  directAddrPtr_{nullptr},
  interpolationAddrPtr_{nullptr},
  weightsPtr_{nullptr},
  insertedFaceLabelsPtr_{nullptr}
{
  // Check for possibility of direct mapping
  direct_ = (mpm_.facesFromPointsMap().empty()
             && mpm_.facesFromEdgesMap().empty()
             && mpm_.facesFromFacesMap().empty());

  // Check for inserted faces
  if (direct_ && (mpm_.faceMap().empty() || min(mpm_.faceMap()) > -1)) {
    insertedFaces_ = false;
  } else {
    // Need to check all 3 lists to see if there are inserted faces
    // with no owner
    // Make a copy of the face map, add the entries for faces from points
    // and faces from edges and check for left-overs
    labelList fm{mesh_.nFaces(), -1};
    const List<objectMap>& ffp = mpm_.facesFromPointsMap();
    FOR_ALL(ffp, ffpI) {
      fm[ffp[ffpI].index()] = 0;
    }
    const List<objectMap>& ffe = mpm_.facesFromEdgesMap();
    FOR_ALL(ffe, ffeI) {
      fm[ffe[ffeI].index()] = 0;
    }
    const List<objectMap>& fff = mpm_.facesFromFacesMap();
    FOR_ALL(fff, fffI) {
      fm[fff[fffI].index()] = 0;
    }
    if (min(fm) < 0) {
      insertedFaces_ = true;
    }
  }
}


// Destructor 
mousse::faceMapper::~faceMapper()
{
  clearOut();
}


// Member Functions 
mousse::label mousse::faceMapper::size() const
{
  return mesh_.nFaces();
}


mousse::label mousse::faceMapper::sizeBeforeMapping() const
{
  return mpm_.nOldFaces();
}


mousse::label mousse::faceMapper::internalSizeBeforeMapping() const
{
  return mpm_.nOldInternalFaces();
}


const mousse::labelUList& mousse::faceMapper::directAddressing() const
{
  if (!direct()) {
    FATAL_ERROR_IN
    (
      "const labelUList& faceMapper::directAddressing() const"
    )
    << "Requested direct addressing for an interpolative mapper."
    << abort(FatalError);
  }
  if (!insertedObjects()) {
    // No inserted faces.  Re-use faceMap
    return mpm_.faceMap();
  } else {
    if (directAddrPtr_ == nullptr) {
      calcAddressing();
    }
    return *directAddrPtr_;
  }
}


const mousse::labelListList& mousse::faceMapper::addressing() const
{
  if (direct()) {
    FATAL_ERROR_IN
    (
      "const labelListList& faceMapper::addressing() const"
    )
    << "Requested interpolative addressing for a direct mapper."
    << abort(FatalError);
  }
  if (interpolationAddrPtr_ == nullptr) {
    calcAddressing();
  }
  return *interpolationAddrPtr_;
}


const mousse::scalarListList& mousse::faceMapper::weights() const
{
  if (direct()) {
    FATAL_ERROR_IN
    (
      "const scalarListList& faceMapper::weights() const"
    )
    << "Requested interpolative weights for a direct mapper."
    << abort(FatalError);
  }
  if (weightsPtr_ == nullptr) {
    calcAddressing();
  }
  return *weightsPtr_;
}


const mousse::labelList& mousse::faceMapper::insertedObjectLabels() const
{
  if (insertedFaceLabelsPtr_ == nullptr) {
    if (!insertedObjects()) {
      // There are no inserted faces
      insertedFaceLabelsPtr_ = new labelList{0};
    } else {
      calcAddressing();
    }
  }
  return *insertedFaceLabelsPtr_;
}


const mousse::labelHashSet& mousse::faceMapper::flipFaceFlux() const
{
  return mpm_.flipFaceFlux();
}


mousse::label mousse::faceMapper::nOldInternalFaces() const
{
  return mpm_.nOldInternalFaces();
}


const mousse::labelList& mousse::faceMapper::oldPatchStarts() const
{
  return mpm_.oldPatchStarts();
}


const mousse::labelList& mousse::faceMapper::oldPatchSizes() const
{
  return mpm_.oldPatchSizes();
}

