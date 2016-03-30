// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "object_registry.hpp"
#include "time.hpp"
#include "iostreams.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(objectRegistry, 0);

}


// Private Member Functions
bool mousse::objectRegistry::parentNotTime() const
{
  return (&parent_ != dynamic_cast<const objectRegistry*>(&time_));
}


// Constructors
mousse::objectRegistry::objectRegistry
(
  const Time& t,
  const label nIoObjects
)
:
  regIOobject
  {
    {
      string::validate<word>(t.caseName()),
      "",
      t,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE,
      false
    },
    true    // to flag that this is the top-level regIOobject
  },
  HashTable<regIOobject*>{nIoObjects},
  time_{t},
  parent_{t},
  dbDir_{name()},
  event_{1}
{}


mousse::objectRegistry::objectRegistry
(
  const IOobject& io,
  const label nIoObjects
)
:
  regIOobject{io},
  HashTable<regIOobject*>{nIoObjects},
  time_{io.time()},
  parent_{io.db()},
  dbDir_{parent_.dbDir()/local()/name()},
  event_{1}
{
  writeOpt() = IOobject::AUTO_WRITE;
}


// Destructor
mousse::objectRegistry::~objectRegistry()
{
  List<regIOobject*> myObjects{size()};
  label nMyObjects = 0;
  for (iterator iter = begin(); iter != end(); ++iter) {
    if (iter()->ownedByRegistry()) {
      myObjects[nMyObjects++] = iter();
    }
  }
  for (label i=0; i < nMyObjects; i++) {
    checkOut(*myObjects[i]);
  }
}


// Member Functions
mousse::wordList mousse::objectRegistry::names() const
{
  return HashTable<regIOobject*>::toc();
}


mousse::wordList mousse::objectRegistry::sortedNames() const
{
  return HashTable<regIOobject*>::sortedToc();
}


mousse::wordList mousse::objectRegistry::names(const word& ClassName) const
{
  wordList objectNames(size());
  label count=0;
  for (const_iterator iter = cbegin(); iter != cend(); ++iter) {
    if (iter()->type() == ClassName) {
      objectNames[count++] = iter.key();
    }
  }
  objectNames.setSize(count);
  return objectNames;
}


mousse::wordList mousse::objectRegistry::sortedNames(const word& ClassName) const
{
  wordList sortedLst = names(ClassName);
  sort(sortedLst);
  return sortedLst;
}


const mousse::objectRegistry& mousse::objectRegistry::subRegistry
(
  const word& name,
  const bool forceCreate
) const
{
  if (forceCreate && !foundObject<objectRegistry>(name)) {
    objectRegistry* fieldsCachePtr = new objectRegistry
    {
      {
        name,
        time().constant(),
        *this,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      }
    };
    fieldsCachePtr->store();
  }
  return lookupObject<objectRegistry>(name);
}


mousse::label mousse::objectRegistry::getEvent() const
{
  label curEvent = event_++;
  if (event_ == labelMax) {
    if (objectRegistry::debug) {
      WARNING_IN("objectRegistry::getEvent() const")
        << "Event counter has overflowed. "
        << "Resetting counter on all dependent objects." << nl
        << "This might cause extra evaluations." << endl;
    }
    // Reset event counter
    curEvent = 1;
    event_ = 2;
    for (const_iterator iter = begin(); iter != end(); ++iter) {
      const regIOobject& io = *iter();
      if (objectRegistry::debug) {
        Pout << "objectRegistry::getEvent() : "
          << "resetting count on " << iter.key() << endl;
      }
      if (io.eventNo() != 0) {
        const_cast<regIOobject&>(io).eventNo() = curEvent;
      }
    }
  }
  return curEvent;
}


bool mousse::objectRegistry::checkIn(regIOobject& io) const
{
  if (objectRegistry::debug) {
    Pout << "objectRegistry::checkIn(regIOobject&) : "
      << name() << " : checking in " << io.name()
      << endl;
  }
  return const_cast<objectRegistry&>(*this).insert(io.name(), &io);
}


bool mousse::objectRegistry::checkOut(regIOobject& io) const
{
  iterator iter = const_cast<objectRegistry&>(*this).find(io.name());
  if (iter != end()) {
    if (objectRegistry::debug) {
      Pout << "objectRegistry::checkOut(regIOobject&) : "
        << name() << " : checking out " << iter.key()
        << endl;
    }
    if (iter() != &io) {
      if (objectRegistry::debug) {
        WARNING_IN("objectRegistry::checkOut(regIOobject&)")
          << name() << " : attempt to checkOut copy of "
          << iter.key()
          << endl;
      }
      return false;
    } else {
      regIOobject* object = iter();
      bool hasErased = const_cast<objectRegistry&>(*this).erase(iter);
      if (io.ownedByRegistry()) {
        delete object;
      }
      return hasErased;
    }
  } else {
    if (objectRegistry::debug) {
      Pout << "objectRegistry::checkOut(regIOobject&) : "
        << name() << " : could not find " << io.name()
        << " in registry " << name()
        << endl;
    }
  }
  return false;
}


void mousse::objectRegistry::rename(const word& newName)
{
  regIOobject::rename(newName);
  // adjust dbDir_ as well
  string::size_type i = dbDir_.rfind('/');
  if (i == string::npos) {
    dbDir_ = newName;
  } else {
    dbDir_.replace(i+1, string::npos, newName);
  }
}


bool mousse::objectRegistry::modified() const
{
  FOR_ALL_CONST_ITER(HashTable<regIOobject*>, *this, iter) {
    if (iter()->modified()) {
      return true;
    }
  }
  return false;
}


void mousse::objectRegistry::readModifiedObjects()
{
  for (iterator iter = begin(); iter != end(); ++iter) {
    if (objectRegistry::debug) {
      Pout << "objectRegistry::readModifiedObjects() : "
        << name() << " : Considering reading object "
        << iter.key() << endl;
    }
    iter()->readIfModified();
  }
}


bool mousse::objectRegistry::readIfModified()
{
  readModifiedObjects();
  return true;
}


bool mousse::objectRegistry::writeObject
(
  IOstream::streamFormat fmt,
  IOstream::versionNumber ver,
  IOstream::compressionType cmp
) const
{
  bool ok = true;
  FOR_ALL_CONST_ITER(HashTable<regIOobject*>, *this, iter) {
    if (objectRegistry::debug) {
      Pout << "objectRegistry::write() : "
        << name() << " : Considering writing object "
        << iter.key()
        << " with writeOpt " << iter()->writeOpt()
        << " to file " << iter()->objectPath()
        << endl;
    }
    if (iter()->writeOpt() != NO_WRITE) {
      ok = iter()->writeObject(fmt, ver, cmp) && ok;
    }
  }
  return ok;
}
