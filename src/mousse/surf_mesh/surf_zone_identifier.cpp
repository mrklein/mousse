// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surf_zone_identifier.hpp"
#include "dictionary.hpp"


// Constructors 
mousse::surfZoneIdentifier::surfZoneIdentifier()
:
  name_{word::null},
  index_{0},
  geometricType_{word::null}
{}


mousse::surfZoneIdentifier::surfZoneIdentifier
(
  const word& name,
  const label index,
  const word& geometricType
)
:
  name_{name},
  index_{index},
  geometricType_{geometricType}
{}


mousse::surfZoneIdentifier::surfZoneIdentifier
(
  const word& name,
  const dictionary& dict,
  const label index
)
:
  name_{name},
  index_{index}
{
  dict.readIfPresent("geometricType", geometricType_);
}


mousse::surfZoneIdentifier::surfZoneIdentifier
(
  const surfZoneIdentifier& p,
  const label index
)
:
  name_{p.name()},
  index_{index},
  geometricType_{p.geometricType()}
{}


// Destructor 
mousse::surfZoneIdentifier::~surfZoneIdentifier()
{}


// Member Functions 
void mousse::surfZoneIdentifier::write(Ostream& os) const
{
  if (geometricType_.size()) {
    os.writeKeyword("geometricType") << geometricType_
      << token::END_STATEMENT << nl;
  }
}


mousse::Ostream& mousse::operator<<(Ostream& os, const surfZoneIdentifier& p)
{
  p.write(os);
  os.check
  (
    "Ostream& operator<<(Ostream&, const surfZoneIdentifier&)"
  );
  return os;
}
