// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ldu_addressing.hpp"
#include "demand_driven_data.hpp"
#include "scalar_field.hpp"


// Private Member Functions
void mousse::lduAddressing::calcLosort() const
{
  if (losortPtr_) {
    FATAL_ERROR_IN("lduAddressing::calcLosort() const")
      << "losort already calculated"
      << abort(FatalError);
  }
  // Scan the neighbour list to find out how many times the cell
  // appears as a neighbour of the face. Done this way to avoid guessing
  // and resizing list
  labelList nNbrOfFace{size(), 0};
  const labelUList& nbr = upperAddr();
  FOR_ALL(nbr, nbrI) {
    nNbrOfFace[nbr[nbrI]]++;
  }
  // Create temporary neighbour addressing
  labelListList cellNbrFaces{size()};
  FOR_ALL(cellNbrFaces, cellI) {
    cellNbrFaces[cellI].setSize(nNbrOfFace[cellI]);
  }
  // Reset the list of number of neighbours to zero
  nNbrOfFace = 0;
  // Scatter the neighbour faces
  FOR_ALL(nbr, nbrI) {
    cellNbrFaces[nbr[nbrI]][nNbrOfFace[nbr[nbrI]]] = nbrI;
    nNbrOfFace[nbr[nbrI]]++;
  }
  // Gather the neighbours into the losort array
  losortPtr_ = new labelList{nbr.size(), -1};
  labelList& lst = *losortPtr_;
  // Set counter for losort
  label lstI = 0;
  FOR_ALL(cellNbrFaces, cellI) {
    const labelList& curNbr = cellNbrFaces[cellI];
    FOR_ALL(curNbr, curNbrI) {
      lst[lstI] = curNbr[curNbrI];
      lstI++;
    }
  }
}


void mousse::lduAddressing::calcOwnerStart() const
{
  if (ownerStartPtr_) {
    FATAL_ERROR_IN("lduAddressing::calcOwnerStart() const")
      << "owner start already calculated"
      << abort(FatalError);
  }
  const labelList& own = lowerAddr();
  ownerStartPtr_ = new labelList{size() + 1, own.size()};
  labelList& ownStart = *ownerStartPtr_;
  // Set up first lookup by hand
  ownStart[0] = 0;
  label nOwnStart = 0;
  label i = 1;
  FOR_ALL(own, faceI) {
    label curOwn = own[faceI];
    if (curOwn > nOwnStart) {
      while (i <= curOwn) {
        ownStart[i++] = faceI;
      }
      nOwnStart = curOwn;
    }
  }
}


void mousse::lduAddressing::calcLosortStart() const
{
  if (losortStartPtr_) {
    FATAL_ERROR_IN("lduAddressing::calcLosortStart() const")
      << "losort start already calculated"
      << abort(FatalError);
  }
  losortStartPtr_ = new labelList{size() + 1, 0};
  labelList& lsrtStart = *losortStartPtr_;
  const labelList& nbr = upperAddr();
  const labelList& lsrt = losortAddr();
  // Set up first lookup by hand
  lsrtStart[0] = 0;
  label nLsrtStart = 0;
  label i = 0;
  FOR_ALL(lsrt, faceI) {
    // Get neighbour
    const label curNbr = nbr[lsrt[faceI]];
    if (curNbr > nLsrtStart) {
      while (i <= curNbr) {
        lsrtStart[i++] = faceI;
      }
      nLsrtStart = curNbr;
    }
  }
  // Set up last lookup by hand
  lsrtStart[size()] = nbr.size();
}


// Destructor
mousse::lduAddressing::~lduAddressing()
{
  deleteDemandDrivenData(losortPtr_);
  deleteDemandDrivenData(ownerStartPtr_);
  deleteDemandDrivenData(losortStartPtr_);
}


// Member Functions
const mousse::labelUList& mousse::lduAddressing::losortAddr() const
{
  if (!losortPtr_) {
    calcLosort();
  }
  return *losortPtr_;
}


const mousse::labelUList& mousse::lduAddressing::ownerStartAddr() const
{
  if (!ownerStartPtr_) {
    calcOwnerStart();
  }
  return *ownerStartPtr_;
}


const mousse::labelUList& mousse::lduAddressing::losortStartAddr() const
{
  if (!losortStartPtr_) {
    calcLosortStart();
  }
  return *losortStartPtr_;
}


// Return edge index given owner and neighbour label
mousse::label mousse::lduAddressing::triIndex(const label a, const label b) const
{
  label own = min(a, b);
  label nbr = max(a, b);
  label startLabel = ownerStartAddr()[own];
  label endLabel = ownerStartAddr()[own + 1];
  const labelUList& neighbour = upperAddr();
  for (label i = startLabel; i < endLabel; i++) {
    if (neighbour[i] == nbr) {
      return i;
    }
  }
  // If neighbour has not been found, something has gone seriously
  // wrong with the addressing mechanism
  FATAL_ERROR_IN
  (
    "lduAddressing::triIndex(const label owner, const label nbr) const"
  )
  << "neighbour " << nbr << " not found for owner " << own << ". "
  << "Problem with addressing"
  << abort(FatalError);
  return -1;
}


mousse::Tuple2<mousse::label, mousse::scalar> mousse::lduAddressing::band() const
{
  const labelUList& owner = lowerAddr();
  const labelUList& neighbour = upperAddr();
  labelList cellBandwidth(size(), 0);
  FOR_ALL(neighbour, faceI) {
    label own = owner[faceI];
    label nei = neighbour[faceI];
    // Note: mag not necessary for correct (upper-triangular) ordering.
    label diff = nei-own;
    cellBandwidth[nei] = max(cellBandwidth[nei], diff);
  }
  label bandwidth = max(cellBandwidth);
  // Do not use field algebra because of conversion label to scalar
  scalar profile = 0.0;
  FOR_ALL(cellBandwidth, cellI) {
    profile += 1.0*cellBandwidth[cellI];
  }
  return Tuple2<label, scalar>{bandwidth, profile};
}
