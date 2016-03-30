// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ioobject_list.hpp"
#include "time.hpp"
#include "os_specific.hpp"
#include "word_re_list_matcher.hpp"


// Constructors
mousse::IOobjectList::IOobjectList(const label nIoObjects)
:
  HashPtrTable<IOobject>{nIoObjects}
{}


mousse::IOobjectList::IOobjectList
(
  const objectRegistry& db,
  const fileName& instance,
  const fileName& local,
  IOobject::readOption r,
  IOobject::writeOption w,
  bool registerObject
)
:
  HashPtrTable<IOobject>{}
{
  word newInstance = instance;
  if (!isDir(db.path(instance))) {
    newInstance = db.time().findInstancePath(instant(instance));
    if (newInstance.empty()) {
      return;
    }
  }
  // Create a list of file names in this directory
  fileNameList ObjectNames =
    readDir(db.path(newInstance, db.dbDir()/local), fileName::FILE);
  FOR_ALL(ObjectNames, i) {
    IOobject* objectPtr = new IOobject
    {
      ObjectNames[i],
      newInstance,
      local,
      db,
      r,
      w,
      registerObject
    };
    if (objectPtr->headerOk()) {
      insert(ObjectNames[i], objectPtr);
    } else {
      delete objectPtr;
    }
  }
}


mousse::IOobjectList::IOobjectList(const IOobjectList& ioOL)
:
  HashPtrTable<IOobject>{ioOL}
{}


// Destructor
mousse::IOobjectList::~IOobjectList()
{}


// Member Functions
bool mousse::IOobjectList::add(IOobject& io)
{
  return insert(io.name(), &io);
}


bool mousse::IOobjectList::remove(IOobject& io)
{
  HashPtrTable<IOobject>::iterator iter =
    HashPtrTable<IOobject>::find(io.name());
  if (iter != end()) {
    return erase(iter);
  } else {
    return false;
  }
}


mousse::IOobject* mousse::IOobjectList::lookup(const word& name) const
{
  HashPtrTable<IOobject>::const_iterator iter = find(name);
  if (iter != end()) {
    if (IOobject::debug) {
      Info << "IOobjectList::lookup : found "
        << name << endl;
    }
    return const_cast<IOobject*>(*iter);
  } else {
    if (IOobject::debug) {
      Info << "IOobjectList::lookup : could not find "
        << name << endl;
    }
    return NULL;
  }
}


mousse::IOobjectList mousse::IOobjectList::lookup(const wordRe& name) const
{
  IOobjectList objectsOfName{size()};
  FOR_ALL_CONST_ITER(HashPtrTable<IOobject>, *this, iter) {
    if (name.match(iter()->name())) {
      if (IOobject::debug) {
        Info << "IOobjectList::lookupRe : found " << iter.key() << endl;
      }
      objectsOfName.insert(iter.key(), new IOobject(*iter()));
    }
  }
  return objectsOfName;
}


mousse::IOobjectList mousse::IOobjectList::lookup(const wordReList& patterns) const
{
  wordReListMatcher names{patterns};
  IOobjectList objectsOfName{size()};
  FOR_ALL_CONST_ITER(HashPtrTable<IOobject>, *this, iter) {
    if (names.match(iter()->name())) {
      if (IOobject::debug) {
        Info << "IOobjectList::lookupRe : found " << iter.key() << endl;
      }
      objectsOfName.insert(iter.key(), new IOobject(*iter()));
    }
  }
  return objectsOfName;
}


mousse::IOobjectList mousse::IOobjectList::lookupClass(const word& ClassName) const
{
  IOobjectList objectsOfClass{size()};
  FOR_ALL_CONST_ITER(HashPtrTable<IOobject>, *this, iter) {
    if (iter()->headerClassName() == ClassName) {
      if (IOobject::debug) {
        Info << "IOobjectList::lookupClass : found "
          << iter.key() << endl;
      }
      objectsOfClass.insert(iter.key(), new IOobject(*iter()));
    }
  }
  return objectsOfClass;
}


mousse::wordList mousse::IOobjectList::names() const
{
  return HashPtrTable<IOobject>::toc();
}


mousse::wordList mousse::IOobjectList::sortedNames() const
{
  return HashPtrTable<IOobject>::sortedToc();
}


mousse::wordList mousse::IOobjectList::names(const word& ClassName) const
{
  wordList objectNames{size()};
  label count = 0;
  FOR_ALL_CONST_ITER(HashPtrTable<IOobject>, *this, iter) {
    if (iter()->headerClassName() == ClassName) {
      objectNames[count++] = iter.key();
    }
  }
  objectNames.setSize(count);
  return objectNames;
}


mousse::wordList mousse::IOobjectList::sortedNames(const word& ClassName) const
{
  wordList sortedLst = names(ClassName);
  sort(sortedLst);
  return sortedLst;
}
