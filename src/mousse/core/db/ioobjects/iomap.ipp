// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iomap.hpp"


// Constructors 
template<class T>
mousse::IOMap<T>::IOMap(const IOobject& io)
:
  regIOobject{io}
{
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  }
}


template<class T>
mousse::IOMap<T>::IOMap(const IOobject& io, const label size)
:
  regIOobject{io}
{
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED )
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  } else {
    Map<T>::setSize(size);
  }
}


template<class T>
mousse::IOMap<T>::IOMap(const IOobject& io, const Map<T>& map)
:
  regIOobject{io}
{
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED )
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  } else {
    Map<T>::operator=(map);
  }
}


template<class T>
mousse::IOMap<T>::IOMap(const IOobject& io, const Xfer<Map<T> >& map)
:
  regIOobject{io}
{
  Map<T>::transfer(map());
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  }
}


// Destructor 
template<class T>
mousse::IOMap<T>::~IOMap()
{}


// Member Functions 
template<class T>
bool mousse::IOMap<T>::writeData(Ostream& os) const
{
  return (os << *this).good();
}


// Member Operators 
template<class T>
void mousse::IOMap<T>::operator=(const IOMap<T>& rhs)
{
  Map<T>::operator=(rhs);
}


template<class T>
void mousse::IOMap<T>::operator=(const Map<T>& rhs)
{
  Map<T>::operator=(rhs);
}

