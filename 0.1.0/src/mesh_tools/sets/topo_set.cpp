// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "topo_set.hpp"
#include "map_poly_mesh.hpp"
#include "poly_mesh.hpp"
#include "bound_box.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(topoSet, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(topoSet, word);
DEFINE_RUN_TIME_SELECTION_TABLE(topoSet, size);
DEFINE_RUN_TIME_SELECTION_TABLE(topoSet, set);

}


mousse::autoPtr<mousse::topoSet> mousse::topoSet::New
(
  const word& setType,
  const polyMesh& mesh,
  const word& name,
  readOption r,
  writeOption w
)
{
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(setType);
  if (cstrIter == wordConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "topoSet::New(const word&, "
      "const polyMesh&, const word&, readOption, writeOption)"
    )
    << "Unknown set type " << setType
    << endl << endl
    << "Valid set types : " << endl
    << wordConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<topoSet>{cstrIter()(mesh, name, r, w)};
}


mousse::autoPtr<mousse::topoSet> mousse::topoSet::New
(
  const word& setType,
  const polyMesh& mesh,
  const word& name,
  const label size,
  writeOption w
)
{
  sizeConstructorTable::iterator cstrIter =
    sizeConstructorTablePtr_->find(setType);
  if (cstrIter == sizeConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "topoSet::New(const word&, "
      "const polyMesh&, const word&, const label, writeOption)"
    )
    << "Unknown set type " << setType
    << endl << endl
    << "Valid set types : " << endl
    << sizeConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<topoSet>{cstrIter()(mesh, name, size, w)};
}


mousse::autoPtr<mousse::topoSet> mousse::topoSet::New
(
  const word& setType,
  const polyMesh& mesh,
  const word& name,
  const topoSet& set,
  writeOption w
)
{
  setConstructorTable::iterator cstrIter =
    setConstructorTablePtr_->find(setType);
  if (cstrIter == setConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "topoSet::New(const word&, "
      "const polyMesh&, const word&, const topoSet&, writeOption)"
    )
    << "Unknown set type " << setType
    << endl << endl
    << "Valid set types : " << endl
    << setConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<topoSet>{cstrIter()(mesh, name, set, w)};
}


mousse::fileName mousse::topoSet::localPath
(
  const polyMesh& mesh,
  const word& name
)
{
  return mesh.facesInstance()/mesh.dbDir()/polyMesh::meshSubDir/"sets"/name;
}


// Private Member Functions 
// Update stored cell numbers using map.
// Do in two passes to prevent allocation if nothing changed.
void mousse::topoSet::updateLabels(const labelList& map)
{
  // Iterate over map to see if anything changed
  bool changed = false;
  FOR_ALL_CONST_ITER(labelHashSet, *this, iter) {
    if ((iter.key() < 0) || (iter.key() > map.size())) {
      FATAL_ERROR_IN
      (
        "topoSet::updateLabels(const labelList&, labelHashSet)"
      )
      << "Illegal content " << iter.key() << " of set:" << name()
      << " of type " << type() << endl
      << "Value should be between 0 and " << map.size()-1
      << abort(FatalError);
    }
    const label newCellI = map[iter.key()];
    if (newCellI != iter.key()) {
      changed = true;
      break;
    }
  }
  // Relabel (use second Map to prevent overlapping)
  if (changed) {
    labelHashSet newSet(2*size());
    FOR_ALL_CONST_ITER(labelHashSet, *this, iter) {
      const label newCellI = map[iter.key()];
      if (newCellI >= 0) {
        newSet.insert(newCellI);
      }
    }
    transfer(newSet);
  }
}


void mousse::topoSet::check(const label maxLabel)
{
  FOR_ALL_CONST_ITER(topoSet, *this, iter) {
    if ((iter.key() < 0) || (iter.key() > maxLabel)) {
      FATAL_ERROR_IN("topoSet::check(const label)")
        << "Illegal content " << iter.key() << " of set:" << name()
        << " of type " << type() << endl
        << "Value should be between 0 and " << maxLabel
        << abort(FatalError);
    }
  }
}


// Write maxElem elements, starting at iter. Updates iter and elemI.
void mousse::topoSet::writeDebug
(
  Ostream& os,
  const label maxElem,
  topoSet::const_iterator& iter,
  label& elemI
) const
{
  label n = 0;
  for (; (iter != end()) && (n < maxElem); ++iter) {
    if ((n != 0) && ((n % 10) == 0)) {
      os << endl;
    }
    os << iter.key() << ' ';
    n++;
    elemI++;
  }
}


// Write maxElem elements, starting at iter. Updates iter and elemI.
void mousse::topoSet::writeDebug
(
  Ostream& os,
  const pointField& coords,
  const label maxElem,
  topoSet::const_iterator& iter,
  label& elemI
) const
{
  label n = 0;
  for (; (iter != end()) && (n < maxElem); ++iter) {
    if ((n != 0) && ((n % 3) == 0)) {
      os << endl;
    }
    os << iter.key() << coords[iter.key()] << ' ';
    n++;
    elemI++;
  }
}


void mousse::topoSet::writeDebug
(
  Ostream& os,
  const pointField& coords,
  const label maxLen
) const
{
  // Bounding box of contents.
  boundBox bb{pointField{coords, toc()}, true};
  os << "Set bounding box: min = "
    << bb.min() << "    max = " << bb.max() << " metres. " << endl << endl;
  label n = 0;
  topoSet::const_iterator iter = begin();
  if (size() <= maxLen) {
    writeDebug(os, coords, maxLen, iter, n);
  } else {
    label halfLen = maxLen/2;
    os << "Size larger than " << maxLen << ". Printing first and last "
      << halfLen << " elements:" << endl << endl;
    writeDebug(os, coords, halfLen, iter, n);
    os << nl << "  .." << nl << endl;
    for (; n < size() - halfLen; ++n) {
      ++iter;
    }
    writeDebug(os, coords, halfLen, iter, n);
  }
}


// Constructors 
mousse::topoSet::topoSet(const IOobject& obj, const word& wantedType)
:
  regIOobject{obj}
{
  if (readOpt() == IOobject::MUST_READ
      || readOpt() == IOobject::MUST_READ_IF_MODIFIED
      || (readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    if (readStream(wantedType).good()) {
      readStream(wantedType) >> static_cast<labelHashSet&>(*this);
      close();
    }
  }
}


mousse::topoSet::topoSet
(
  const polyMesh& mesh,
  const word& wantedType,
  const word& name,
  readOption r,
  writeOption w
)
:
  regIOobject
  {
    {
      name,
      mesh.time().findInstance
      (
        mesh.dbDir()/polyMesh::meshSubDir/"sets",
        word::null,
        r,  //IOobject::MUST_READ,
        mesh.facesInstance()
      ),
      polyMesh::meshSubDir/"sets",
      mesh,
      r,
      w
    }
  }
{
  if (readOpt() == IOobject::MUST_READ
      || readOpt() == IOobject::MUST_READ_IF_MODIFIED
      || (readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    if (readStream(wantedType).good()) {
      readStream(wantedType) >> static_cast<labelHashSet&>(*this);
      close();
    }
  }
}


mousse::topoSet::topoSet
(
  const polyMesh& mesh,
  const word& name,
  const label size,
  writeOption w
)
:
  regIOobject
  {
    {
      name,
      mesh.time().findInstance
      (
        mesh.dbDir()/polyMesh::meshSubDir/"sets",
        word::null,
        IOobject::NO_READ,
        mesh.facesInstance()
      ),
      polyMesh::meshSubDir/"sets",
      mesh,
      IOobject::NO_READ,
      w
    }
  },
  labelHashSet{size}
{}


mousse::topoSet::topoSet
(
  const polyMesh& mesh,
  const word& name,
  const labelHashSet& set,
  writeOption w
)
:
  regIOobject
  {
    {
      name,
      mesh.time().findInstance
      (
        mesh.dbDir()/polyMesh::meshSubDir/"sets",
        word::null,
        IOobject::NO_READ,
        mesh.facesInstance()
      ),
      polyMesh::meshSubDir/"sets",
      mesh,
      IOobject::NO_READ,
      w
    }
  },
  labelHashSet{set}
{}


mousse::topoSet::topoSet(const IOobject& obj, const label size)
:
  regIOobject{obj},
  labelHashSet{size}
{}


mousse::topoSet::topoSet(const IOobject& obj, const labelHashSet& set)
:
  regIOobject{obj},
  labelHashSet{set}
{}


// Destructor 
mousse::topoSet::~topoSet()
{}


// Member Functions 
void mousse::topoSet::invert(const label maxLen)
{
  // Keep copy of current set.
  labelHashSet currentSet{*this};
  clear();
  resize(2*(maxLen - currentSet.size()));
  for (label cellI = 0; cellI < maxLen; cellI++) {
    if (!currentSet.found(cellI)) {
      insert(cellI);
    }
  }
}


void mousse::topoSet::subset(const topoSet& set)
{
  // Keep copy of current set.
  labelHashSet currentSet{*this};
  clear();
  resize(2*min(currentSet.size(), set.size()));
  FOR_ALL_CONST_ITER(labelHashSet, currentSet, iter) {
    if (set.found(iter.key())) {
      // element present in both currentSet and set.
      insert(iter.key());
    }
  }
}


void mousse::topoSet::addSet(const topoSet& set)
{
  FOR_ALL_CONST_ITER(topoSet, set, iter) {
    insert(iter.key());
  }
}


void mousse::topoSet::deleteSet(const topoSet& set)
{
  FOR_ALL_CONST_ITER(topoSet, set, iter) {
    erase(iter.key());
  }
}


void mousse::topoSet::sync(const polyMesh&)
{
  NOT_IMPLEMENTED("topoSet::sync(const polyMesh&)");
}


void mousse::topoSet::writeDebug(Ostream& os, const label maxLen) const
{
  label n = 0;
  topoSet::const_iterator iter = begin();
  if (size() <= maxLen) {
    writeDebug(os, maxLen, iter, n);
  } else {
    label halfLen = maxLen/2;
    os << "Size larger than " << maxLen << ". Printing first and last "
      << halfLen << " elements:" << endl << endl;
    writeDebug(os, halfLen, iter, n);
    os << nl << "  .." << nl << endl;
    for (; n < size() - halfLen; ++n) {
      ++iter;
    }
    writeDebug(os, halfLen, iter, n);
  }
}


bool mousse::topoSet::writeData(Ostream& os) const
{
  return (os << *this).good();
}


void mousse::topoSet::updateMesh(const mapPolyMesh&)
{
  NOT_IMPLEMENTED("topoSet::updateMesh(const mapPolyMesh&)");
}


// Member Operators 
void mousse::topoSet::operator=(const topoSet& rhs)
{
  labelHashSet::operator=(rhs);
}

