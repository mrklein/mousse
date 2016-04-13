// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_identifier.hpp"
#include "dictionary.hpp"
#include "list_ops.hpp"


// Constructors 
mousse::patchIdentifier::patchIdentifier
(
  const word& name,
  const label index,
  const word& physicalType,
  const wordList& inGroups
)
:
  name_{name},
  index_{index},
  physicalType_{physicalType},
  inGroups_{inGroups}
{}


mousse::patchIdentifier::patchIdentifier
(
  const word& name,
  const dictionary& dict,
  const label index
)
:
  name_{name},
  index_{index}
{
  dict.readIfPresent("physicalType", physicalType_);
  dict.readIfPresent("inGroups", inGroups_);
}


mousse::patchIdentifier::patchIdentifier
(
  const patchIdentifier& p,
  const label index
)
:
  name_{p.name_},
  index_{index},
  physicalType_{p.physicalType_},
  inGroups_{p.inGroups_}
{}


// Destructor 
mousse::patchIdentifier::~patchIdentifier()
{}


// Member Functions 
bool mousse::patchIdentifier::inGroup(const word& name) const
{
  return findIndex(inGroups_, name) != -1;
}


void mousse::patchIdentifier::write(Ostream& os) const
{
  if (physicalType_.size()) {
    os.writeKeyword("physicalType") << physicalType_
      << token::END_STATEMENT << nl;
  }
  if (inGroups_.size()) {
    os.writeKeyword("inGroups") << inGroups_
      << token::END_STATEMENT << nl;
  }
}


// Friend Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const patchIdentifier& pi)
{
  pi.write(os);
  os.check("Ostream& operator<<(Ostream&, const patchIdentifier&)");
  return os;
}
