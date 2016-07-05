// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_mapper.hpp"
#include "demand_driven_data.hpp"
#include "poly_mesh.hpp"
#include "map_poly_mesh.hpp"


// Private Member Functions 

void mousse::cellMapper::calcAddressing() const
{
  if (directAddrPtr_ != nullptr
      || interpolationAddrPtr_ != nullptr
      || weightsPtr_ != nullptr
      || insertedCellLabelsPtr_ != nullptr) {
    FATAL_ERROR_IN("void cellMapper::calcAddressing() const")
      << "Addressing already calculated."
      << abort(FatalError);
  }
  if (direct()) {
    // Direct addressing, no weights
    directAddrPtr_ = new labelList{mpm_.cellMap()};
    labelList& directAddr = *directAddrPtr_;
    // Not necessary to resize the list as there are no retired cells
    // directAddr.setSize(mesh_.nCells());
    insertedCellLabelsPtr_ = new labelList{mesh_.nCells()};
    labelList& insertedCells = *insertedCellLabelsPtr_;
    label nInsertedCells = 0;
    FOR_ALL(directAddr, cellI) {
      if (directAddr[cellI] < 0) {
        // Found inserted cell
        directAddr[cellI] = 0;
        insertedCells[nInsertedCells] = cellI;
        nInsertedCells++;
      }
    }
    insertedCells.setSize(nInsertedCells);
  } else {
    // Interpolative addressing
    interpolationAddrPtr_ = new labelListList{mesh_.nCells()};
    labelListList& addr = *interpolationAddrPtr_;
    weightsPtr_ = new scalarListList{mesh_.nCells()};
    scalarListList& w = *weightsPtr_;
    const List<objectMap>& cfp = mpm_.cellsFromPointsMap();
    FOR_ALL(cfp, cfpI) {
      // Get addressing
      const labelList& mo = cfp[cfpI].masterObjects();
      label cellI = cfp[cfpI].index();
      if (addr[cellI].size()) {
        FATAL_ERROR_IN("void cellMapper::calcAddressing() const")
          << "Master cell " << cellI
          << " mapped from point cells " << mo
          << " already destination of mapping." << abort(FatalError);
      }
      // Map from masters, uniform weights
      addr[cellI] = mo;
      w[cellI] = scalarList(mo.size(), 1.0/mo.size());
    }
    const List<objectMap>& cfe = mpm_.cellsFromEdgesMap();
    FOR_ALL(cfe, cfeI) {
      // Get addressing
      const labelList& mo = cfe[cfeI].masterObjects();
      label cellI = cfe[cfeI].index();
      if (addr[cellI].size()) {
        FATAL_ERROR_IN("void cellMapper::calcAddressing() const")
          << "Master cell " << cellI
          << " mapped from edge cells " << mo
          << " already destination of mapping." << abort(FatalError);
      }
      // Map from masters, uniform weights
      addr[cellI] = mo;
      w[cellI] = scalarList(mo.size(), 1.0/mo.size());
    }
    const List<objectMap>& cff = mpm_.cellsFromFacesMap();
    FOR_ALL(cff, cffI) {
      // Get addressing
      const labelList& mo = cff[cffI].masterObjects();
      label cellI = cff[cffI].index();
      if (addr[cellI].size()) {
        FATAL_ERROR_IN("void cellMapper::calcAddressing() const")
          << "Master cell " << cellI
          << " mapped from face cells " << mo
          << " already destination of mapping." << abort(FatalError);
      }
      // Map from masters, uniform weights
      addr[cellI] = mo;
      w[cellI] = scalarList(mo.size(), 1.0/mo.size());
    }
    // Volume conservative mapping if possible
    const List<objectMap>& cfc = mpm_.cellsFromCellsMap();
    FOR_ALL(cfc, cfcI) {
      // Get addressing
      const labelList& mo = cfc[cfcI].masterObjects();
      label cellI = cfc[cfcI].index();
      if (addr[cellI].size()) {
        FATAL_ERROR_IN("void cellMapper::calcAddressing() const")
          << "Master cell " << cellI
          << " mapped from cell cells " << mo
          << " already destination of mapping."
          << abort(FatalError);
      }
      // Map from masters
      addr[cellI] = mo;
    }
    if (mpm_.hasOldCellVolumes()) {
      // Volume weighted
      const scalarField& V = mpm_.oldCellVolumes();
      if (V.size() != sizeBeforeMapping()) {
        FATAL_ERROR_IN("void cellMapper::calcAddressing() const")
          << "cellVolumes size " << V.size()
          << " is not the old number of cells " << sizeBeforeMapping()
          << ". Are your cellVolumes already mapped?"
          << " (new number of cells " << size() << ")"
          << abort(FatalError);
      }
      FOR_ALL(cfc, cfcI) {
        const labelList& mo = cfc[cfcI].masterObjects();
        label cellI = cfc[cfcI].index();
        w[cellI].setSize(mo.size());
        if (mo.size()) {
          scalar sumV = 0;
          FOR_ALL(mo, ci) {
            w[cellI][ci] = V[mo[ci]];
            sumV += V[mo[ci]];
          }
          if (sumV > VSMALL) {
            FOR_ALL(mo, ci) {
              w[cellI][ci] /= sumV;
            }
          } else {
            // Exception: zero volume. Use uniform mapping
            w[cellI] = scalarList{mo.size(), 1.0/mo.size()};
          }
        }
      }
    } else {
      // Uniform weighted
      FOR_ALL(cfc, cfcI) {
        const labelList& mo = cfc[cfcI].masterObjects();
        label cellI = cfc[cfcI].index();
        w[cellI] = scalarList{mo.size(), 1.0/mo.size()};
      }
    }
    // Do mapped faces. Note that can already be set from cellsFromCells
    // so check if addressing size still zero.
    const labelList& cm = mpm_.cellMap();
    FOR_ALL(cm, cellI) {
      if (cm[cellI] > -1 && addr[cellI].empty()) {
        // Mapped from a single cell
        addr[cellI] = labelList(1, cm[cellI]);
        w[cellI] = scalarList(1, 1.0);
      }
    }
    // Grab inserted points (for them the size of addressing is still zero)
    insertedCellLabelsPtr_ = new labelList{mesh_.nCells()};
    labelList& insertedCells = *insertedCellLabelsPtr_;
    label nInsertedCells = 0;
    FOR_ALL(addr, cellI) {
      if (addr[cellI].empty()) {
        // Mapped from a dummy cell
        addr[cellI] = labelList{1, label(0)};
        w[cellI] = scalarList{1, 1.0};
        insertedCells[nInsertedCells] = cellI;
        nInsertedCells++;
      }
    }
    insertedCells.setSize(nInsertedCells);
  }
}


void mousse::cellMapper::clearOut()
{
  deleteDemandDrivenData(directAddrPtr_);
  deleteDemandDrivenData(interpolationAddrPtr_);
  deleteDemandDrivenData(weightsPtr_);
  deleteDemandDrivenData(insertedCellLabelsPtr_);
}


// Constructors 

// Construct from components
mousse::cellMapper::cellMapper(const mapPolyMesh& mpm)
:
  mesh_{mpm.mesh()},
  mpm_{mpm},
  insertedCells_{true},
  direct_{false},
  directAddrPtr_{nullptr},
  interpolationAddrPtr_{nullptr},
  weightsPtr_{nullptr},
  insertedCellLabelsPtr_{nullptr}
{
  // Check for possibility of direct mapping
  if (mpm_.cellsFromPointsMap().empty()
      && mpm_.cellsFromEdgesMap().empty()
      && mpm_.cellsFromFacesMap().empty()
      && mpm_.cellsFromCellsMap().empty()) {
    direct_ = true;
  } else {
    direct_ = false;
  }
  // Check for inserted cells
  if (direct_ && (mpm_.cellMap().empty() || min(mpm_.cellMap()) > -1)) {
    insertedCells_ = false;
  } else {
    // Need to check all 3 lists to see if there are inserted cells
    // with no owner
    // Make a copy of the cell map, add the entried for cells from points,
    // cells from edges and cells from faces and check for left-overs
    labelList cm{mesh_.nCells(), -1};
    const List<objectMap>& cfp = mpm_.cellsFromPointsMap();
    FOR_ALL(cfp, cfpI) {
      cm[cfp[cfpI].index()] = 0;
    }
    const List<objectMap>& cfe = mpm_.cellsFromEdgesMap();
    FOR_ALL(cfe, cfeI) {
      cm[cfe[cfeI].index()] = 0;
    }
    const List<objectMap>& cff = mpm_.cellsFromFacesMap();
    FOR_ALL(cff, cffI) {
      cm[cff[cffI].index()] = 0;
    }
    const List<objectMap>& cfc = mpm_.cellsFromCellsMap();
    FOR_ALL(cfc, cfcI) {
      cm[cfc[cfcI].index()] = 0;
    }
    if (min(cm) < 0) {
      insertedCells_ = true;
    }
  }
}


// Destructor 
mousse::cellMapper::~cellMapper()
{
  clearOut();
}


// Member Functions 
mousse::label mousse::cellMapper::size() const
{
  return mpm_.cellMap().size();
}


mousse::label mousse::cellMapper::sizeBeforeMapping() const
{
  return mpm_.nOldCells();
}


const mousse::labelUList& mousse::cellMapper::directAddressing() const
{
  if (!direct()) {
    FATAL_ERROR_IN
    (
      "const labelUList& cellMapper::directAddressing() const"
    )
    << "Requested direct addressing for an interpolative mapper."
    << abort(FatalError);
  }
  if (!insertedObjects()) {
    // No inserted cells.  Re-use cellMap
    return mpm_.cellMap();
  } else {
    if (directAddrPtr_ == nullptr) {
      calcAddressing();
    }
    return *directAddrPtr_;
  }
}


const mousse::labelListList& mousse::cellMapper::addressing() const
{
  if (direct()) {
    FATAL_ERROR_IN
    (
      "const labelListList& cellMapper::addressing() const"
    )
    << "Requested interpolative addressing for a direct mapper."
    << abort(FatalError);
  }
  if (interpolationAddrPtr_ == nullptr) {
    calcAddressing();
  }
  return *interpolationAddrPtr_;
}


const mousse::scalarListList& mousse::cellMapper::weights() const
{
  if (direct()) {
    FATAL_ERROR_IN
    (
      "const scalarListList& cellMapper::weights() const"
    )
    << "Requested interpolative weights for a direct mapper."
    << abort(FatalError);
  }
  if (weightsPtr_ == nullptr) {
    calcAddressing();
  }
  return *weightsPtr_;
}


const mousse::labelList& mousse::cellMapper::insertedObjectLabels() const
{
  if (insertedCellLabelsPtr_ == nullptr) {
    if (!insertedObjects()) {
      // There are no inserted cells
      insertedCellLabelsPtr_ = new labelList{0};
    } else {
      calcAddressing();
    }
  }
  return *insertedCellLabelsPtr_;
}

