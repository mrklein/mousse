// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iolist.hpp"


// Constructors
template<class T>
mousse::IOList<T>::IOList(const IOobject& io)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOList::IOList(const IOobject&)")
      << "IOList " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
      " but IOList does not support automatic rereading."
      << endl;
  }
  if((io.readOpt() == IOobject::MUST_READ
      || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  }
}


template<class T>
mousse::IOList<T>::IOList(const IOobject& io, const label size)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOList::IOList(const IOobject&, const label)")
      << "IOList " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
      " but IOList does not support automatic rereading."
      << endl;
  }

  if((io.readOpt() == IOobject::MUST_READ
      || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  } else {
    List<T>::setSize(size);
  }
}


template<class T>
mousse::IOList<T>::IOList(const IOobject& io, const List<T>& list)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOList::IOList(const IOobject&, const List<T>&)")
      << "IOList " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
      " but IOList does not support automatic rereading."
      << endl;
  }
  if((io.readOpt() == IOobject::MUST_READ
      || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  } else {
    List<T>::operator=(list);
  }
}


template<class T>
mousse::IOList<T>::IOList(const IOobject& io, const Xfer<List<T> >& list)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN
    (
      "IOList::IOList(const IOobject&, const Xfer<List<T> >&)"
    )
    << "IOList " << name()
    << " constructed with IOobject::MUST_READ_IF_MODIFIED"
       " but IOList does not support automatic rereading."
    << endl;
  }
  List<T>::transfer(list());
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  }
}


// Destructor
template<class T>
mousse::IOList<T>::~IOList()
{}


// Member Functions
template<class T>
bool mousse::IOList<T>::writeData(Ostream& os) const
{
  return (os << *this).good();
}


// Member Operators
template<class T>
void mousse::IOList<T>::operator=(const IOList<T>& rhs)
{
  List<T>::operator=(rhs);
}


template<class T>
void mousse::IOList<T>::operator=(const List<T>& rhs)
{
  List<T>::operator=(rhs);
}
