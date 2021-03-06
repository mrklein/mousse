// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ioptr_list.hpp"


// Constructors
template<class T>
template<class INew>
mousse::IOPtrList<T>::IOPtrList(const IOobject& io, const INew& inewt)
:
  regIOobject{io}
{
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    PtrList<T>::read(readStream(typeName), inewt);
    close();
  }
}


template<class T>
mousse::IOPtrList<T>::IOPtrList(const IOobject& io)
:
  regIOobject{io}
{
  if
  (
    (io.readOpt() == IOobject::MUST_READ
     || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
  )
  {
    PtrList<T>::read(readStream(typeName), INew<T>());
    close();
  }
}


template<class T>
mousse::IOPtrList<T>::IOPtrList(const IOobject& io, const label s)
:
  regIOobject{io},
  PtrList<T>{s}
{
  if (io.readOpt() != IOobject::NO_READ) {
    FATAL_ERROR_IN("IOPtrList<T>::IOPtrList(const IOobject&, const label)")
      << "NO_READ must be set if specifying size" << nl
      << exit(FatalError);
  }
}


template<class T>
mousse::IOPtrList<T>::IOPtrList(const IOobject& io, const PtrList<T>& list)
:
  regIOobject{io}
{
  if
  (
    (io.readOpt() == IOobject::MUST_READ
     || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
  ) {
    PtrList<T>::read(readStream(typeName), INew<T>());
    close();
  } else {
    PtrList<T>::operator=(list);
  }
}


template<class T>
mousse::IOPtrList<T>::IOPtrList(const IOobject& io, const Xfer<PtrList<T> >& list)
:
  regIOobject{io}
{
  PtrList<T>::transfer(list());

  if
  (
    (io.readOpt() == IOobject::MUST_READ
     || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
  )
  {
    PtrList<T>::read(readStream(typeName), INew<T>());
    close();
  }
}


// Destructor
template<class T>
mousse::IOPtrList<T>::~IOPtrList()
{}


// Member Functions
template<class T>
bool mousse::IOPtrList<T>::writeData(Ostream& os) const
{
  return (os << *this).good();
}


// Member Operators
template<class T>
void mousse::IOPtrList<T>::operator=(const IOPtrList<T>& rhs)
{
  PtrList<T>::operator=(rhs);
}
