// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reg_ioobject.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "register_switch.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(regIOobject, 0);
template<>
const char* NamedEnum
<
  regIOobject::fileCheckTypes,
  4
>::names[] =
{
  "timeStamp",
  "timeStampMaster",
  "inotify",
  "inotifyMaster"
};

}

int mousse::regIOobject::fileModificationSkew
{
  mousse::debug::optimisationSwitch("fileModificationSkew", 30)
};

REGISTER_OPT_SWITCH
(
  "fileModificationSkew",
  int,
  mousse::regIOobject::fileModificationSkew
);

const mousse::NamedEnum<mousse::regIOobject::fileCheckTypes, 4>
  mousse::regIOobject::fileCheckTypesNames;

// Default fileCheck type
mousse::regIOobject::fileCheckTypes mousse::regIOobject::fileModificationChecking
(
  fileCheckTypesNames.read
  (
    debug::optimisationSwitches().lookup
    (
      "fileModificationChecking"
    )
  )
);


namespace mousse {

// Register re-reader
class addfileModificationCheckingToOpt
:
  public ::mousse::simpleRegIOobject
{
public:
  addfileModificationCheckingToOpt(const char* name)
  :
    ::mousse::simpleRegIOobject(mousse::debug::addOptimisationObject, name)
  {}
  virtual ~addfileModificationCheckingToOpt()
  {}
  virtual void readData(mousse::Istream& is)
  {
    regIOobject::fileModificationChecking =
      regIOobject::fileCheckTypesNames.read(is);
  }
  virtual void writeData(mousse::Ostream& os) const
  {
    os <<  regIOobject::fileCheckTypesNames
      [regIOobject::fileModificationChecking];
  }
};
addfileModificationCheckingToOpt addfileModificationCheckingToOpt_
(
  "fileModificationChecking"
);

}

bool mousse::regIOobject::masterOnlyReading = false;


// Constructors 

// Construct from IOobject
mousse::regIOobject::regIOobject(const IOobject& io, const bool isTime)
:
  IOobject{io},
  registered_{false},
  ownedByRegistry_{false},
  watchIndex_{-1},
  // Do not get event for top level Time database
  eventNo_{isTime ? 0 : db().getEvent()},
  isPtr_{NULL}
{
  // Register with objectRegistry if requested
  if (registerObject()) {
    checkIn();
  }
}


// Construct as copy
mousse::regIOobject::regIOobject(const regIOobject& rio)
:
  IOobject{rio},
  registered_{false},
  ownedByRegistry_{false},
  watchIndex_{rio.watchIndex_},
  eventNo_{db().getEvent()},
  isPtr_{NULL}
{
  // Do not register copy with objectRegistry
}


// Construct as copy, and transfering objectRegistry registration to copy
// if registerCopy is true
mousse::regIOobject::regIOobject(const regIOobject& rio, bool registerCopy)
:
  IOobject{rio},
  registered_{false},
  ownedByRegistry_{false},
  watchIndex_{-1},
  eventNo_{db().getEvent()},
  isPtr_{NULL}
{
  if (registerCopy && rio.registered_) {
    const_cast<regIOobject&>(rio).checkOut();
    checkIn();
  }
}


mousse::regIOobject::regIOobject
(
  const word& newName,
  const regIOobject& rio,
  bool registerCopy
)
:
  IOobject{newName, rio.instance(), rio.local(), rio.db()},
  registered_{false},
  ownedByRegistry_{false},
  watchIndex_{-1},
  eventNo_{db().getEvent()},
  isPtr_{NULL}
{
  if (registerCopy) {
    checkIn();
  }
}


mousse::regIOobject::regIOobject
(
  const IOobject& io,
  const regIOobject&
)
:
  IOobject{io},
  registered_{false},
  ownedByRegistry_{false},
  watchIndex_{-1},
  eventNo_{db().getEvent()},
  isPtr_{NULL}
{
  if (registerObject()) {
    checkIn();
  }
}


// Destructor 
// Delete read stream, checkout from objectRegistry and destroy
mousse::regIOobject::~regIOobject()
{
  if (objectRegistry::debug) {
    Info << "Destroying regIOobject called " << name()
      << " of type " << type()
      << " in directory " << path()
      << endl;
  }
  if (isPtr_) {
    delete isPtr_;
    isPtr_ = NULL;
  }
  // Check out of objectRegistry if not owned by the registry
  if (!ownedByRegistry_) {
    checkOut();
  }
}


// Member Functions 
bool mousse::regIOobject::checkIn()
{
  if (!registered_) {
    // multiple checkin of same object is disallowed - this would mess up
    // any mapping
    registered_ = db().checkIn(*this);
    if (registered_ && readOpt() == MUST_READ_IF_MODIFIED
        && time().runTimeModifiable()) {
      if (watchIndex_ != -1) {
        FATAL_ERROR_IN("regIOobject::checkIn()")
          << "Object " << objectPath()
          << " already watched with index " << watchIndex_
          << abort(FatalError);
      }
      fileName f = filePath();
      if (!f.size()) {
        // We don't have this file but would like to re-read it.
        // Possibly if master-only reading mode.
        f = objectPath();
      }
      watchIndex_ = time().addWatch(f);
    }
    // check-in on defaultRegion is allowed to fail, since subsetted meshes
    // are created with the same name as their originating mesh
    if (!registered_ && debug && name() != polyMesh::defaultRegion) {
      if (debug == 2) {
        // for ease of finding where attempted duplicate check-in
        // originated
        FATAL_ERROR_IN("regIOobject::checkIn()")
          << "failed to register object " << objectPath()
          << " the name already exists in the objectRegistry" << endl
          << "Contents:" << db().sortedToc()
          << abort(FatalError);
      } else {
        WARNING_IN("regIOobject::checkIn()")
          << "failed to register object " << objectPath()
          << " the name already exists in the objectRegistry"
          << endl;
      }
    }
  }
  return registered_;
}


bool mousse::regIOobject::checkOut()
{
  if (registered_) {
    registered_ = false;
    if (watchIndex_ != -1) {
      time().removeWatch(watchIndex_);
      watchIndex_ = -1;
    }
    return db().checkOut(*this);
  }
  return false;
}


bool mousse::regIOobject::upToDate(const regIOobject& a) const
{
  if (a.eventNo() >= eventNo_) {
    return false;
  } else {
    return true;
  }
}


bool mousse::regIOobject::upToDate
(
  const regIOobject& a,
  const regIOobject& b
) const
{
  if (a.eventNo() >= eventNo_ || b.eventNo() >= eventNo_) {
    return false;
  } else {
    return true;
  }
}


bool mousse::regIOobject::upToDate
(
  const regIOobject& a,
  const regIOobject& b,
  const regIOobject& c
) const
{
  if (a.eventNo() >= eventNo_ || b.eventNo() >= eventNo_
      || c.eventNo() >= eventNo_) {
    return false;
  } else {
    return true;
  }
}


bool mousse::regIOobject::upToDate
(
  const regIOobject& a,
  const regIOobject& b,
  const regIOobject& c,
  const regIOobject& d
) const
{
  if (a.eventNo() >= eventNo_ || b.eventNo() >= eventNo_
      || c.eventNo() >= eventNo_ || d.eventNo() >= eventNo_) {
    return false;
  } else {
    return true;
  }
}


//- Flag me as up to date
void mousse::regIOobject::setUpToDate()
{
  eventNo_ = db().getEvent();
}


// Rename object and re-register with objectRegistry under new name
void mousse::regIOobject::rename(const word& newName)
{
  // Check out of objectRegistry
  checkOut();
  IOobject::rename(newName);
  if (registerObject()) {
    // Re-register object with objectRegistry
    checkIn();
  }
}


// Assign to IOobject
void mousse::regIOobject::operator=(const IOobject& io)
{
  if (isPtr_) {
    delete isPtr_;
    isPtr_ = NULL;
  }
  // Check out of objectRegistry
  checkOut();
  IOobject::operator=(io);
  if (registerObject()) {
    // Re-register object with objectRegistry
    checkIn();
  }
}

