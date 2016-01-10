// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hash_table.hpp"
#include "ulabel.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(HashTableCore, 0);
}
const mousse::label mousse::HashTableCore::maxTableSize
(
  mousse::HashTableCore::canonicalSize
  (
    mousse::labelMax/2
  )
);
// Member Functions 
mousse::label mousse::HashTableCore::canonicalSize(const label size)
{
  if (size < 1)
  {
    return 0;
  }
  // enforce power of two
  uLabel goodSize = size;
  if (goodSize & (goodSize - 1))
  {
    // brute-force is fast enough
    goodSize = 1;
    while (goodSize < unsigned(size))
    {
      goodSize <<= 1;
    }
  }
  return goodSize;
}
