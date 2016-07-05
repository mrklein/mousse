// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "function_object_list.hpp"
#include "time.hpp"
#include "map_poly_mesh.hpp"


// Private Member Functions 
mousse::functionObject* mousse::functionObjectList::remove
(
  const word& key,
  label& oldIndex
)
{
  functionObject* ptr = 0;
  // Find index of existing functionObject
  HashTable<label>::iterator fnd = indices_.find(key);
  if (fnd != indices_.end()) {
    oldIndex = fnd();
    // retrieve the pointer and remove it from the old list
    ptr = this->set(oldIndex, 0).ptr();
    indices_.erase(fnd);
  } else {
    oldIndex = -1;
  }
  return ptr;
}


// Constructors 
mousse::functionObjectList::functionObjectList
(
  const Time& t,
  const bool execution
)
:
  PtrList<functionObject>{},
  digests_{},
  indices_{},
  time_{t},
  parentDict_{t.controlDict()},
  execution_{execution},
  updated_{false}
{}


mousse::functionObjectList::functionObjectList
(
  const Time& t,
  const dictionary& parentDict,
  const bool execution
)
:
  PtrList<functionObject>{},
  digests_{},
  indices_{},
  time_{t},
  parentDict_{parentDict},
  execution_{execution},
  updated_{false}
{}


// Destructor 
mousse::functionObjectList::~functionObjectList()
{}


// Member Functions 
void mousse::functionObjectList::clear()
{
  PtrList<functionObject>::clear();
  digests_.clear();
  indices_.clear();
  updated_ = false;
}


mousse::label mousse::functionObjectList::findObjectID(const word& name) const
{
  FOR_ALL(*this, objectI) {
    if (operator[](objectI).name() == name) {
      return objectI;
    }
  }
  return -1;
}


void mousse::functionObjectList::on()
{
  execution_ = true;
}


void mousse::functionObjectList::off()
{
  // for safety, also force a read() when execution is turned back on
  updated_ = execution_ = false;
}


bool mousse::functionObjectList::status() const
{
  return execution_;
}


bool mousse::functionObjectList::start()
{
  return read();
}


bool mousse::functionObjectList::execute(const bool forceWrite)
{
  bool ok = true;
  if (execution_) {
    if (!updated_) {
      read();
    }
    FOR_ALL(*this, objectI) {
      ok = operator[](objectI).execute(forceWrite) && ok;
    }
  }
  return ok;
}


bool mousse::functionObjectList::end()
{
  bool ok = true;
  if (execution_) {
    if (!updated_) {
      read();
    }
    FOR_ALL(*this, objectI) {
      ok = operator[](objectI).end() && ok;
    }
  }
  return ok;
}


bool mousse::functionObjectList::timeSet()
{
  bool ok = true;
  if (execution_) {
    if (!updated_) {
      read();
    }
    FOR_ALL(*this, objectI) {
      ok = operator[](objectI).timeSet() && ok;
    }
  }
  return ok;
}


bool mousse::functionObjectList::adjustTimeStep()
{
  bool ok = true;
  if (execution_) {
    if (!updated_) {
      read();
    }
    FOR_ALL(*this, objectI) {
      ok = operator[](objectI).adjustTimeStep() && ok;
    }
  }
  return ok;
}


bool mousse::functionObjectList::read()
{
  bool ok = true;
  updated_ = execution_;
  // avoid reading/initializing if execution is off
  if (!execution_) {
    return ok;
  }
  // Update existing and add new functionObjects
  const entry* entryPtr = parentDict_.lookupEntryPtr
  (
    "functions",
    false,
    false
  );
  if (entryPtr) {
    PtrList<functionObject> newPtrs;
    List<SHA1Digest> newDigs;
    HashTable<label> newIndices;
    label nFunc = 0;
    if (entryPtr->isDict()) {
      // a dictionary of functionObjects
      const dictionary& functionDicts = entryPtr->dict();
      newPtrs.setSize(functionDicts.size());
      newDigs.setSize(functionDicts.size());
      FOR_ALL_CONST_ITER(dictionary, functionDicts, iter) {
        // safety:
        if (!iter().isDict()) {
          continue;
        }
        const word& key = iter().keyword();
        const dictionary& dict = iter().dict();
        newDigs[nFunc] = dict.digest();
        label oldIndex;
        functionObject* objPtr = remove(key, oldIndex);
        if (objPtr) {
          // an existing functionObject, and dictionary changed
          if (newDigs[nFunc] != digests_[oldIndex]) {
            ok = objPtr->read(dict) && ok;
          }
        } else {
          // new functionObject
          objPtr = functionObject::New(key, time_, dict).ptr();
          ok = objPtr->start() && ok;
        }
        newPtrs.set(nFunc, objPtr);
        newIndices.insert(key, nFunc);
        nFunc++;
      }
    } else {
      // a list of functionObjects
      PtrList<entry> functionDicts{entryPtr->stream()};
      newPtrs.setSize(functionDicts.size());
      newDigs.setSize(functionDicts.size());
      FOR_ALL_ITER(PtrList<entry>, functionDicts, iter) {
        // safety:
        if (!iter().isDict()) {
          continue;
        }
        const word& key = iter().keyword();
        const dictionary& dict = iter().dict();
        newDigs[nFunc] = dict.digest();
        label oldIndex;
        functionObject* objPtr = remove(key, oldIndex);
        if (objPtr) {
          // an existing functionObject, and dictionary changed
          if (newDigs[nFunc] != digests_[oldIndex]) {
            ok = objPtr->read(dict) && ok;
          }
        } else {
          // new functionObject
          objPtr = functionObject::New(key, time_, dict).ptr();
          ok = objPtr->start() && ok;
        }
        newPtrs.set(nFunc, objPtr);
        newIndices.insert(key, nFunc);
        nFunc++;
      }
    }
    // safety:
    newPtrs.setSize(nFunc);
    newDigs.setSize(nFunc);
    // updating the PtrList of functionObjects also deletes any existing,
    // but unused functionObjects
    PtrList<functionObject>::transfer(newPtrs);
    digests_.transfer(newDigs);
    indices_.transfer(newIndices);
  } else {
    PtrList<functionObject>::clear();
    digests_.clear();
    indices_.clear();
  }
  return ok;
}


void mousse::functionObjectList::updateMesh(const mapPolyMesh& mpm)
{
  if (execution_) {
    FOR_ALL(*this, objectI) {
      operator[](objectI).updateMesh(mpm);
    }
  }
}


void mousse::functionObjectList::movePoints(const polyMesh& mesh)
{
  if (execution_) {
    FOR_ALL(*this, objectI) {
      operator[](objectI).movePoints(mesh);
    }
  }
}

