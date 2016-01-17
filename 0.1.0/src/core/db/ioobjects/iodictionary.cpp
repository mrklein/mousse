// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iodictionary.hpp"
#include "object_registry.hpp"
#include "pstream.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(IOdictionary, 0);
bool IOdictionary::writeDictionaries
(
  debug::infoSwitch("writeDictionaries", 0)
);
}
// Constructors 
mousse::IOdictionary::IOdictionary(const IOobject& io)
:
  regIOobject(io)
{
  // Temporary warning
  if (debug && io.readOpt() == IOobject::MUST_READ)
  {
    WARNING_IN("IOdictionary::IOdictionary(const IOobject&)")
      << "Dictionary " << name()
      << " constructed with IOobject::MUST_READ"
      " instead of IOobject::MUST_READ_IF_MODIFIED." << nl
      << "Use MUST_READ_IF_MODIFIED if you need automatic rereading."
      << endl;
  }
  // Everyone check or just master
  bool masterOnly =
    regIOobject::fileModificationChecking == timeStampMaster
  || regIOobject::fileModificationChecking == inotifyMaster;
  // Check if header is ok for READ_IF_PRESENT
  bool isHeaderOk = false;
  if (io.readOpt() == IOobject::READ_IF_PRESENT)
  {
    if (masterOnly)
    {
      if (Pstream::master())
      {
        isHeaderOk = headerOk();
      }
      Pstream::scatter(isHeaderOk);
    }
    else
    {
      isHeaderOk = headerOk();
    }
  }
  if
  (
    (
      io.readOpt() == IOobject::MUST_READ
    || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
    )
  || isHeaderOk
  )
  {
    readFile(masterOnly);
  }
  dictionary::name() = IOobject::objectPath();
}
mousse::IOdictionary::IOdictionary(const IOobject& io, const dictionary& dict)
:
  regIOobject(io)
{
  // Temporary warning
  if (debug && io.readOpt() == IOobject::MUST_READ)
  {
    WARNING_IN
    (
      "IOdictionary::IOdictionary(const IOobject& const dictionary&)"
    )   << "Dictionary " << name()
      << " constructed with IOobject::MUST_READ"
      " instead of IOobject::MUST_READ_IF_MODIFIED." << nl
      << "Use MUST_READ_IF_MODIFIED if you need automatic rereading."
      << endl;
  }
  // Everyone check or just master
  bool masterOnly =
    regIOobject::fileModificationChecking == timeStampMaster
  || regIOobject::fileModificationChecking == inotifyMaster;
  // Check if header is ok for READ_IF_PRESENT
  bool isHeaderOk = false;
  if (io.readOpt() == IOobject::READ_IF_PRESENT)
  {
    if (masterOnly)
    {
      if (Pstream::master())
      {
        isHeaderOk = headerOk();
      }
      Pstream::scatter(isHeaderOk);
    }
    else
    {
      isHeaderOk = headerOk();
    }
  }
  if
  (
    (
      io.readOpt() == IOobject::MUST_READ
    || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED
    )
  || isHeaderOk
  )
  {
    readFile(masterOnly);
  }
  else
  {
    dictionary::operator=(dict);
  }
  dictionary::name() = IOobject::objectPath();
}
mousse::IOdictionary::IOdictionary(const IOobject& io, Istream& is)
:
  regIOobject(io)
{
  dictionary::name() = IOobject::objectPath();
  // Note that we do construct the dictionary null and read in afterwards
  // so that if there is some fancy massaging due to a functionEntry in
  // the dictionary at least the type information is already complete.
  is  >> *this;
}
// Destructor 
mousse::IOdictionary::~IOdictionary()
{}
// Members Functions
const mousse::word& mousse::IOdictionary::name() const
{
  return regIOobject::name();
}
// Member Operators 
void mousse::IOdictionary::operator=(const IOdictionary& rhs)
{
  dictionary::operator=(rhs);
}
