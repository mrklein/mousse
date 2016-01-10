// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coordinate_systems.hpp"
#include "ioptr_list.hpp"
#include "time.hpp"
#include "string_list_ops.hpp"
// Static Data Members
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(coordinateSystems, 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(IOPtrList<coordinateSystem>, 0);

}
// Constructors 
mousse::coordinateSystems::coordinateSystems(const IOobject& io)
:
  IOPtrList<coordinateSystem>(io)
{}
mousse::coordinateSystems::coordinateSystems
(
  const IOobject& io,
  const PtrList<coordinateSystem>& lst
)
:
  IOPtrList<coordinateSystem>(io, lst)
{}
mousse::coordinateSystems::coordinateSystems
(
  const IOobject& io,
  const Xfer<PtrList<coordinateSystem> >& lst
)
:
  IOPtrList<coordinateSystem>(io, lst)
{}
// Selectors
// Read construct from registry, or return previously registered
const mousse::coordinateSystems& mousse::coordinateSystems::New
(
  const objectRegistry& obr
)
{
  if (obr.foundObject<coordinateSystems>(typeName))
  {
    return obr.lookupObject<coordinateSystems>(typeName);
  }
  else
  {
    return obr.store
    (
      new coordinateSystems
      (
        IOobject
        (
          typeName,
          obr.time().constant(),
          obr,
          IOobject::READ_IF_PRESENT,
          IOobject::NO_WRITE
        )
      )
    );
  }
}
// Member Functions 
mousse::label mousse::coordinateSystems::find(const keyType& key) const
{
  return findIndex(key);
}
mousse::labelList mousse::coordinateSystems::findIndices(const keyType& key) const
{
  labelList indices;
  if (key.isPattern())
  {
    indices = findStrings(key, toc());
  }
  else
  {
    indices.setSize(size());
    label nFound = 0;
    FOR_ALL(*this, i)
    {
      if (key == operator[](i).name())
      {
        indices[nFound++] = i;
      }
    }
    indices.setSize(nFound);
  }
  return indices;
}
mousse::label mousse::coordinateSystems::findIndex(const keyType& key) const
{
  if (key.isPattern())
  {
    labelList indices = findIndices(key);
    // return first element
    if (!indices.empty())
    {
      return indices[0];
    }
  }
  else
  {
    FOR_ALL(*this, i)
    {
      if (key == operator[](i).name())
      {
        return i;
      }
    }
  }
  return -1;
}
bool mousse::coordinateSystems::found(const keyType& key) const
{
  return findIndex(key) != -1;
}
mousse::wordList mousse::coordinateSystems::toc() const
{
  wordList keywords(size());
  FOR_ALL(*this, i)
  {
    keywords[i] = operator[](i).name();
  }
  return keywords;
}
bool mousse::coordinateSystems::writeData(Ostream& os) const
{
  os << nl << size() << nl << token::BEGIN_LIST;
  FOR_ALL(*this, i)
  {
    os << nl;
    operator[](i).writeDict(os, true);
  }
  os << token::END_LIST << nl;
  return os.good();
}
