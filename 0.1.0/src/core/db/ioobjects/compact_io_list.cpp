// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "compact_io_list.hpp"
#include "label_list.hpp"

// Private Member Functions 
template<class T, class BaseType>
void mousse::CompactIOList<T, BaseType>::readFromStream()
{
  Istream& is = readStream(word::null);
  if (headerClassName() == IOList<T>::typeName)
  {
    is >> static_cast<List<T>&>(*this);
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
      "CompactIOList<T, BaseType>::readFromStream()",
      is
    )
    << "unexpected class name " << headerClassName()
    << " expected " << typeName << " or " << IOList<T>::typeName
    << endl
    << "    while reading object " << name()
    << exit(FatalIOError);
  }
}


// Constructors 
template<class T, class BaseType>
mousse::CompactIOList<T, BaseType>::CompactIOList(const IOobject& io)
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
mousse::CompactIOList<T, BaseType>::CompactIOList
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
    List<T>::setSize(size);
  }
}
template<class T, class BaseType>
mousse::CompactIOList<T, BaseType>::CompactIOList
(
  const IOobject& io,
  const List<T>& list
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
    List<T>::operator=(list);
  }
}


template<class T, class BaseType>
mousse::CompactIOList<T, BaseType>::CompactIOList
(
  const IOobject& io,
  const Xfer<List<T> >& list
)
:
  regIOobject{io}
{
  List<T>::transfer(list());
  if
  (
    io.readOpt() == IOobject::MUST_READ
  || (io.readOpt() == IOobject::READ_IF_PRESENT && headerOk())
  )
  {
    readFromStream();
  }
}


// Destructor 
template<class T, class BaseType>
mousse::CompactIOList<T, BaseType>::~CompactIOList()
{}


// Member Functions 
template<class T, class BaseType>
bool mousse::CompactIOList<T, BaseType>::writeObject
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
    const_cast<word&>(typeName) = IOList<T>::typeName;
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
bool mousse::CompactIOList<T, BaseType>::writeData(Ostream& os) const
{
  return (os << *this).good();
}


// Member Operators 
template<class T, class BaseType>
void mousse::CompactIOList<T, BaseType>::operator=
(
  const CompactIOList<T, BaseType>& rhs
)
{
  List<T>::operator=(rhs);
}


template<class T, class BaseType>
void mousse::CompactIOList<T, BaseType>::operator=(const List<T>& rhs)
{
  List<T>::operator=(rhs);
}


// Friend Operators 
template<class T, class BaseType>
mousse::Istream& mousse::operator>>
(
  mousse::Istream& is,
  mousse::CompactIOList<T, BaseType>& L
)
{
  // Read compact
  const labelList start(is);
  const List<BaseType> elems(is);
  // Convert
  L.setSize(start.size()-1);
  FOR_ALL(L, i)
  {
    T& subList = L[i];
    label index = start[i];
    subList.setSize(start[i+1] - index);
    FOR_ALL(subList, j)
    {
      subList[j] = elems[index++];
    }
  }
  return is;
}


template<class T, class BaseType>
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::CompactIOList<T, BaseType>& L
)
{
  // Keep ascii writing same.
  if (os.format() == IOstream::ASCII)
  {
    os << static_cast<const List<T>&>(L);
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
    List<BaseType> elems(start[start.size()-1]);
    label elemI = 0;
    FOR_ALL(L, i)
    {
      const T& subList = L[i];
      FOR_ALL(subList, j)
      {
        elems[elemI++] = subList[j];
      }
    }
    os << start << elems;
  }
  return os;
}
