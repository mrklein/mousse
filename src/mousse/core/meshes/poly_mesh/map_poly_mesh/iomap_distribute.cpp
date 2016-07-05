// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iomap_distribute.hpp"


// Static Member Data
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(IOmapDistribute, 0);

}


// Constructors 
mousse::IOmapDistribute::IOmapDistribute(const IOobject& io)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOmapDistribute::IOmapDistribute(const IOobject&)")
      << "IOmapDistribute " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
         " but IOmapDistribute does not support automatic rereading."
      << endl;
  }
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  }
}


mousse::IOmapDistribute::IOmapDistribute
(
  const IOobject& io,
  const mapDistribute& map
)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOmapDistribute::IOmapDistribute(const IOobject&)")
      << "IOmapDistribute " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
      " but IOmapDistribute does not support automatic rereading."
      << endl;
  }
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  } else {
    mapDistribute::operator=(map);
  }
}


mousse::IOmapDistribute::IOmapDistribute
(
  const IOobject& io,
  const Xfer<mapDistribute>& map
)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOmapDistribute::IOmapDistribute(const IOobject&)")
      << "IOmapDistribute " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
      " but IOmapDistribute does not support automatic rereading."
      << endl;
  }
  mapDistribute::transfer(map());
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  }
}


// Destructor 
mousse::IOmapDistribute::~IOmapDistribute()
{}


// Member Functions 
bool mousse::IOmapDistribute::readData(Istream& is)
{
  return (is >> *this).good();
}


bool mousse::IOmapDistribute::writeData(Ostream& os) const
{
  return (os << *this).good();
}

