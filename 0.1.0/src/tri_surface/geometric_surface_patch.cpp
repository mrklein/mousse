// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_surface_patch.hpp"
#include "dictionary.hpp"


namespace mousse {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(geometricSurfacePatch, 0);


// Constructors 
// Construct null
geometricSurfacePatch::geometricSurfacePatch()
:
  geometricType_{"empty"},
  name_{"patch"},
  index_{0}
{}


// Construct from components
geometricSurfacePatch::geometricSurfacePatch
(
  const word& geometricType,
  const word& name,
  const label index
)
:
  geometricType_{geometricType},
  name_{name},
  index_{index}
{
  if (geometricType_.empty()) {
    geometricType_ = "empty";
  }
}


// Construct from Istream
geometricSurfacePatch::geometricSurfacePatch(Istream& is, const label index)
:
  geometricType_{is},
  name_{is},
  index_{index}
{
  if (geometricType_.empty()) {
    geometricType_ = "empty";
  }
}


// Construct from dictionary
geometricSurfacePatch::geometricSurfacePatch
(
  const word& name,
  const dictionary& dict,
  const label index
)
:
  geometricType_{dict.lookup("geometricType")},
  name_{name},
  index_{index}
{
  if (geometricType_.empty()) {
    geometricType_ = "empty";
  }
}


// Member Functions 
// Write
void geometricSurfacePatch::write(Ostream& os) const
{
  os << nl << name_
    << nl << geometricType_;
}


void geometricSurfacePatch::writeDict(Ostream& os) const
{
  os << "    geometricType " << geometricType_ << ';' << nl;
}


// Member Operators 
bool mousse::geometricSurfacePatch::operator!=(const geometricSurfacePatch& p)
  const
{
  return !(*this == p);
}


bool mousse::geometricSurfacePatch::operator==(const geometricSurfacePatch& p)
  const
{
  return
  (
    (geometricType() == p.geometricType()) && (name() == p.name())
  );
}


// Friend Operators 
Istream& operator>>(Istream& is, geometricSurfacePatch& gp)
{
  is >> gp.name_ >> gp.geometricType_;
  return is;
}


Ostream& operator<<(Ostream& os, const geometricSurfacePatch& gp)
{
  gp.write(os);
  os.check
  (
    "Ostream& operator<<(Ostream& f, const geometricSurfacePatch& gp)"
  );
  return os;
}

}  // namespace mousse
