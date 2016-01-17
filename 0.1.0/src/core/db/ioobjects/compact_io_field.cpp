// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "compact_io_field.hpp"
#include "label_list.hpp"

// Private Member Functions 
template<class T, class BaseType>
void mousse::CompactIOField<T, BaseType>::readFromStream()
{
  Istream& is = readStream(word::null);
  if (headerClassName() == IOField<T>::typeName)
  {
    is >> static_cast<Field<T>&>(*this);
    close();
  }
  else if (headerClassName() == typeName)
  {
    is >> *this;
    close();
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "CompactIOField<T, BaseType>::readFromStream()",
      is
    )
    << "unexpected class name " << headerClassName()
    << " expected " << typeName << " or " << IOField<T>::typeName
    << endl
    << "    while reading object " << name()
    << exit(FatalIOError);
  }
}


// Constructors 
template<class T, class BaseType>
mousse::CompactIOField<T, BaseType>::CompactIOField(const IOobject& io)
:
  regIOobject{io}
{
  if (io.readOpt() == IOobject::MUST_READ
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk()))
  {
    readFromStream();
  }
}


template<class T, class BaseType>
mousse::CompactIOField<T, BaseType>::CompactIOField
(
  const IOobject& io,
  const label size
)
:
  regIOobject{io}
{
  if (io.readOpt() == IOobject::MUST_READ
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk()))
  {
    readFromStream();
  }
  else
  {
    Field<T>::setSize(size);
  }
}


template<class T, class BaseType>
mousse::CompactIOField<T, BaseType>::CompactIOField
(
  const IOobject& io,
  const Field<T>& list
)
:
  regIOobject{io}
{
  if (io.readOpt() == IOobject::MUST_READ
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk()))
  {
    readFromStream();
  }
  else
  {
    Field<T>::operator=(list);
  }
}


template<class T, class BaseType>
mousse::CompactIOField<T, BaseType>::CompactIOField
(
  const IOobject& io,
  const Xfer<Field<T> >& list
)
:
  regIOobject{io}
{
  Field<T>::transfer(list());
  if (io.readOpt() == IOobject::MUST_READ
      || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk()))
  {
    readFromStream();
  }
}


// Destructor 
template<class T, class BaseType>
mousse::CompactIOField<T, BaseType>::~CompactIOField()
{}


// Member Functions 
template<class T, class BaseType>
bool mousse::CompactIOField<T, BaseType>::writeObject
(
  IOstream::streamFormat fmt,
  IOstream::versionNumber ver,
  IOstream::compressionType cmp
) const
{
  if (fmt == IOstream::ASCII)
  {
    // Change type to be non-compact format type
    const word oldTypeName = typeName;
    const_cast<word&>(typeName) = IOField<T>::typeName;
    bool good = regIOobject::writeObject(fmt, ver, cmp);
    // Change type back
    const_cast<word&>(typeName) = oldTypeName;
    return good;
  }
  else
  {
    return regIOobject::writeObject(fmt, ver, cmp);
  }
}


template<class T, class BaseType>
bool mousse::CompactIOField<T, BaseType>::writeData(Ostream& os) const
{
  return (os << *this).good();
}
// Member Operators 
template<class T, class BaseType>
void mousse::CompactIOField<T, BaseType>::operator=
(
  const CompactIOField<T, BaseType>& rhs
)
{
  Field<T>::operator=(rhs);
}


template<class T, class BaseType>
void mousse::CompactIOField<T, BaseType>::operator=(const Field<T>& rhs)
{
  Field<T>::operator=(rhs);
}


// Friend Operators 
template<class T, class BaseType>
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::CompactIOField<T, BaseType>& L
)
{
  // Read compact
  const labelList start(is);
  const Field<BaseType> elems(is);
  // Convert
  L.setSize(start.size()-1);
  FOR_ALL(L, i)
  {
    T& subField = L[i];
    label index = start[i];
    subField.setSize(start[i+1] - index);
    FOR_ALL(subField, j)
    {
      subField[j] = elems[index++];
    }
  }
  return is;
}


template<class T, class BaseType>
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::CompactIOField<T, BaseType>& L
)
{
  // Keep ascii writing same.
  if (os.format() == IOstream::ASCII)
  {
    os << static_cast<const Field<T>&>(L);
  }
  else
  {
    // Convert to compact format
    labelList start(L.size()+1);
    start[0] = 0;
    for (label i = 1; i < start.size(); i++)
    {
      start[i] = start[i-1]+L[i-1].size();
    }
    Field<BaseType> elems(start[start.size()-1]);
    label elemI = 0;
    FOR_ALL(L, i)
    {
      const T& subField = L[i];
      FOR_ALL(subField, j)
      {
        elems[elemI++] = subField[j];
      }
    }
    os << start << elems;
  }
  return os;
}
