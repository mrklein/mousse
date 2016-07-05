// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iofield.hpp"


// Constructors 
template<class Type>
mousse::IOField<Type>::IOField(const IOobject& io)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOField::IOField(const IOobject&)")
      << "IOField " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
      " but IOField does not support automatic rereading."
      << endl;
  }
  if ((io.readOpt() == IOobject::MUST_READ
       || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  }
}


template<class Type>
mousse::IOField<Type>::IOField(const IOobject& io, const label size)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOField::IOField(const IOobject&, const label)")
      << "IOField " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
      " but IOField does not support automatic rereading."
      << endl;
  }
  if((io.readOpt() == IOobject::MUST_READ
      || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  } else {
    Field<Type>::setSize(size);
  }
}


template<class Type>
mousse::IOField<Type>::IOField(const IOobject& io, const Field<Type>& f)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN("IOField::IOField(const IOobject&, const Field<Type>&)")
      << "IOField " << name()
      << " constructed with IOobject::MUST_READ_IF_MODIFIED"
      " but IOField does not support automatic rereading."
      << endl;
  }
  if((io.readOpt() == IOobject::MUST_READ
      || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  } else {
    Field<Type>::operator=(f);
  }
}


template<class Type>
mousse::IOField<Type>::IOField(const IOobject& io, const Xfer<Field<Type> >& f)
:
  regIOobject{io}
{
  // Temporary warning
  if (io.readOpt() == IOobject::MUST_READ_IF_MODIFIED) {
    WARNING_IN
    (
      "IOField::IOField(const IOobject&, const Xfer<Field<Type> >&)"
    )
    << "IOField " << name()
    << " constructed with IOobject::MUST_READ_IF_MODIFIED"
    " but IOField does not support automatic rereading."
    << endl;
  }
  Field<Type>::transfer(f());
  if((io.readOpt() == IOobject::MUST_READ
      || io.readOpt() == IOobject::MUST_READ_IF_MODIFIED)
     || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())) {
    readStream(typeName) >> *this;
    close();
  }
}


// Destructor 
template<class Type>
mousse::IOField<Type>::~IOField()
{}


// Member Functions 
template<class Type>
bool mousse::IOField<Type>::writeData(Ostream& os) const
{
  return (os << static_cast<const Field<Type>&>(*this)).good();
}


// Member Operators 
template<class Type>
void mousse::IOField<Type>::operator=(const IOField<Type>& rhs)
{
  Field<Type>::operator=(rhs);
}


template<class Type>
void mousse::IOField<Type>::operator=(const Field<Type>& rhs)
{
  Field<Type>::operator=(rhs);
}
